#include "measurementform.h"
#include "ui_measurementform.h"

MeasurementForm::MeasurementForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MeasurementForm)
{
    ui->setupUi(this);

    wave1List.clear();
    wave2List.clear();

    amplLimit = 0;
    samplingFreq = 192000;
    samplingNum = 1000;
    integralSamples = 10000;
    integralStep = 200;
    followParameter = 3900;//10000;
    triggerLevel = 0.1; //mm
    triggerTime = 1.0; //s
    joinNum = 1000; //ennyi jelet fűz össze
    plottingNum = 10; //ennyi signalonként plottol

    seconds = 0;
    qqq = 0;
    calib1_inf = 0;
    calib1_6 = 0;
    calib1_3 = 0;
    calib2_inf = 0;
    calib2_6 = 0;
    calib2_3 = 0;

    c1 = 0;
    c2 = 0;
    correction1 = 0;
    correction2 = 0;

    ui->stopPushButton->setDisabled(true);
    ui->savePushButton->setDisabled(true);

    isMeasureStopped = false;

    follow1 = false;
    autofollow1 = false;
    follow2 = false;
    autofollow2 = false;

   // arduino.setSerial(serial);



}

MeasurementForm::~MeasurementForm()
{
    arduino.closeSerial(serial);
    delete sc;
    delete ui;
}

void MeasurementForm::slotGetCalibInfo(QString info)
{
    QFile outFile;
    QTextStream out;

    if(info == "1_inf"){
        calib1_inf = 0;
        for(int i = 0; i < integral1.second.size(); i++)
            calib1_inf += integral1.second.at(i);
        calib1_inf /= integral1.second.size();
        outFile.setFileName("calib1_inf.txt");
        outFile.open(QIODevice::WriteOnly | QIODevice::Text);
        out.setDevice(&outFile);
        out << QString::number(calib1_inf);
        outFile.close();
    }

    if(info == "1_6"){
        calib1_6 = integral1.second.first();
        outFile.setFileName("calib1_6.txt");
        outFile.open(QIODevice::WriteOnly | QIODevice::Text);
        out.setDevice(&outFile);
        out << QString::number(calib1_6);
        outFile.close();
    }

    if(info == "1_3"){
        calib1_3 = integral1.second.first();
        outFile.setFileName("calib1_3.txt");
        outFile.open(QIODevice::WriteOnly | QIODevice::Text);
        out.setDevice(&outFile);
        out << QString::number(calib1_3);
        outFile.close();
    }

    if(info == "2_inf"){
        calib2_inf = 0;
        for(int i = 0; i < integral2.second.size(); i++)
            calib2_inf += integral2.second.at(i);
        calib2_inf /= integral2.second.size();
        outFile.setFileName("calib2_inf.txt");
        outFile.open(QIODevice::WriteOnly | QIODevice::Text);
        out.setDevice(&outFile);
        out << QString::number(calib2_inf);
        outFile.close();
    }

    if(info == "2_6"){
        calib2_6 = integral2.second.first();
        outFile.setFileName("calib2_6.txt");
        outFile.open(QIODevice::WriteOnly | QIODevice::Text);
        out.setDevice(&outFile);
        out << QString::number(calib2_6);
        outFile.close();
    }

    if(info == "2_3"){
        calib2_3 = integral2.second.first();
        outFile.setFileName("calib2_3.txt");
        outFile.open(QIODevice::WriteOnly | QIODevice::Text);
        out.setDevice(&outFile);
        out << QString::number(calib2_3);
        outFile.close();
    }

}

void MeasurementForm::slotApplyCalibCH1()
{
    follow1 = true;
    autofollow1 = true;
    c1 = average1;
}

void MeasurementForm::slotApplyCalibCH2()
{
    follow2 = true;
    autofollow2 = true;
    c2 = average2;
}

void MeasurementForm::on_measurementPushButton_clicked()
{
    chooseAudioDevice();
    ui->stopPushButton->setEnabled(true);
    ui->savePushButton->setDisabled(true);
    ui->measurementPushButton->setDisabled(true);

    /*serial test*/
    QString serialCommand = "ASP000 CTC:NTC.req cce8b98f";
    arduino.writeSerial(serial, serialCommand);
    QObject::connect(&serial, &QSerialPort::readyRead, this, &MeasurementForm::readSerial);
    //serial.waitForBytesWritten();


}

void MeasurementForm::slotLastWave(QList<qint16> waveForm1, QList<qint16> waveForm2)
{
    wave1List.append(waveForm1);
    wave2List.append(waveForm2);

    int wave1ListSize = wave1List.size();
    int wave2ListSize = wave2List.size();

    while (wave1ListSize > (integralSamples + samplingNum)){
        wave1List.removeFirst();
        wave1ListSize--;
    }

    while (wave2ListSize > (integralSamples + samplingNum)){
        wave2List.removeFirst();
        wave2ListSize--;
    }

    if (wave1ListSize >= (integralSamples + samplingNum) && wave2ListSize >= (integralSamples + samplingNum)){

        markValue1.clear(); markValue2.clear();
        markTime1.clear(); markTime2.clear();
        integral1.first.clear(); integral1.second.clear();
        integral2.first.clear(); integral2.second.clear();
        amplitudes1.clear(); amplitudes2.clear();

        integral1 = getNumericIntegral(wave1List, wave1ListSize);
        integral2 = getNumericIntegral(wave2List, wave2ListSize);

        markValue1 = integral1.first; //1. elem: markValue, 2. elem: Integrals
        markValue2 = integral2.first;

        int markValue1Size = markValue1.size();
        int markValue2Size = markValue2.size();

        for (int j=0; j<markValue1Size; j++) markTime1.append(seconds+(markValue1.at(j)/samplingFreq));
        for (int j=0; j<markValue2Size; j++) markTime2.append(seconds+(markValue2.at(j)/samplingFreq));

        amplitudes1 = getAmplitude(1, integral1.second);
        amplitudes2 = getAmplitude(2, integral2.second);

        //follow1

        int amp1Size = amplitudes1.size();
        double maxSize = followParameter;
        double threshold1 = triggerLevel;

        QList<double> average1List;

        average1 = 0;
        double integral1Size = integral1.second.size();
        for(int i = 0; i < integral1Size; i++)
            average1List.append(integral1.second.at(i));
        while(average1List.size() > maxSize)
            average1List.removeFirst();
        double average1ListSize = average1List.size();
        for(int i = 0; i < average1ListSize; i++)
            average1 += average1List.at(i);
        average1 /= average1ListSize;


        QVector<double> average1Vector;
        average1Vector.append(average1);
        QVector<double> averageAmplitude1 = getAmplitude(1, average1Vector);
        QDateTime autoFollowDateTime1;

        for(int i = 0; i < amp1Size; i++) {
            if(qAbs<double>(averageAmplitude1.at(0) - amplitudes1.at(i)) > threshold1) {
                autofollow1 = false;
                autoFollowDateTime1 = QDateTime::currentDateTime();
                break;
            }
        }

        //calib1

        if(autofollow1 == false && autoFollowDateTime1.addMSecs(static_cast<int>(triggerTime)) < QDateTime::currentDateTime()) {
            c1 -= c1 + correction1 - integral1.second.at(0);
            average1List.clear();
            autofollow1 = true;
        }

        //follow2

        int amp2Size = amplitudes2.size();
        double threshold2 = triggerLevel;

        QList<double> average2List;

        average2 = 0;
        double integral2Size = integral2.second.size();
        for(int i = 0; i < integral2Size; i++)
            average2List.append(integral2.second.at(i));
        while(average2List.size() > maxSize)
            average2List.removeFirst();
        double average2ListSize = average2List.size();
        for(int i = 0; i < average2ListSize; i++)
            average2 += average2List.at(i);
        average2 /= average2ListSize;


        QVector<double> average2Vector;
        average2Vector.append(average2);
        QVector<double> averageAmplitude2 = getAmplitude(2, average2Vector);
        QDateTime autoFollowDateTime2;

        for(int i = 0; i < amp2Size; i++) {
            if(qAbs<double>(averageAmplitude2.at(0) - amplitudes2.at(i)) > threshold2) {
                autofollow2 = false;
                autoFollowDateTime2 = QDateTime::currentDateTime();
                break;
            }
        }

        //calib2

        if(autofollow2 == false && autoFollowDateTime2.addMSecs(static_cast<int>(triggerTime)) < QDateTime::currentDateTime()) {
            c2 -= c2 + correction2 - integral2.second.at(0);
            average2List.clear();
            autofollow2 = true;
        }

        int Amplitudes1Size = amplitudes1.size();
        int Amplitudes2Size = amplitudes2.size();
        int markTime1Size = markTime1.size();
        int markTime2Size = markTime2.size();
        int mark1ValueSize = markValue1.size();
        int mark2ValueSize = markValue2.size();

        for (int i=0; i<Amplitudes1Size; i++) longAmp1Data.append(amplitudes1.at(i));
        for (int i=0; i<Amplitudes2Size; i++) longAmp2Data.append(amplitudes2.at(i));

        for (int i=0; i<markTime1Size; i++) longMarkTime1Data.append(markTime1.at(i));
        for (int i=0; i<markTime2Size; i++) longMarkTime2Data.append(markTime2.at(i));

        for (int i=0; i<mark1ValueSize; i++) longMarkValue1Data.append(markValue1.at(i));
        for (int i=0; i<mark2ValueSize; i++) longMarkValue2Data.append(markValue2.at(i));

        int longAmpDataSize = longAmp1Data.size();

        while (longAmpDataSize > joinNum) {
            longAmp1Data.removeFirst();
            longAmp2Data.removeFirst();
            longMarkTime1Data.removeFirst();
            longMarkTime2Data.removeFirst();
            longMarkValue1Data.removeFirst();
            longMarkValue2Data.removeFirst();
            longAmpDataSize--;
        }


        if(qqq > plottingNum) {
            qqq = 0;
            double xmin1 = findMin(longMarkTime1Data);
            double xmax1 = findMax(longMarkTime1Data);
            double ymin1 = findMin(longAmp1Data) / 2.0;
            double ymax1 = findMax(longAmp1Data) + findMin(longAmp1Data) / 2.0;
            double xmin2 = findMin(longMarkTime2Data);
            double xmax2 = findMax(longMarkTime2Data);
            double ymin2 = findMin(longAmp2Data) / 2.0;
            double ymax2 = findMax(longAmp2Data) + findMin(longAmp2Data) / 2.0;
            ui->channel1Widget->setPlottingData("Channel1", longMarkTime1Data, longAmp1Data, QwtText("time (s)"), QwtText("distance (mm)"), QColor(Qt::black), xmin1, xmax1, ymin1, ymax1);
            ui->channel2Widget->setPlottingData("Channel2", longMarkTime2Data, longAmp2Data, QwtText("time (s)"), QwtText("distance (mm)"), QColor(Qt::black), xmin2, xmax2, ymin2, ymax2);

        }
        qqq++;

        seconds += ((double)samplingNum)/((double)samplingFreq);
    }
}

void MeasurementForm::chooseAudioDevice()
{
    QList<QAudioDeviceInfo> devices = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);
    QStringList deviceNames;
    for(int i = 0; i < devices.size(); i++)
        deviceNames.append(devices.at(i).deviceName());
    QString deviceName = QInputDialog::getItem(nullptr, "Device", "Device", deviceNames, 0, false);

    if (deviceNames.indexOf(deviceName) < 0)
        return;

    setWindowTitle(deviceName);

    sc = new SoundCardReader(devices.at(deviceNames.indexOf(deviceName)), samplingFreq, samplingNum, this);

    connect(sc, &SoundCardReader::signalLastWave, this, &MeasurementForm::slotLastWave);
    sc->blockSignals(false);

}

QPair<QVector<double>, QVector<double> > MeasurementForm::getNumericIntegral(QList<qint16> &dataList, int waveListSize)
{
    double integral = 0;
    QVector<double> integralList;
    QVector<double> markValue;
    QPair<QVector<double>, QVector<double> > integralData;
    double xValue=0;

    for (double k=0; k < (double)(waveListSize-integralSamples); k += integralStep){
        integral = 0;
        xValue = k;
        for (double i=0; i<integralSamples; i++){
            integral += (1.0/2.0)*(qAbs((double)dataList.at(i+k))+qAbs((double)dataList.at(i+k+1))); //koztuk 1 a tavolsag
            xValue++;
        }
        integralList.append(integral);
        markValue.append(xValue);
    }

    integralData.first = markValue;
    integralData.second = integralList;
    return integralData;
}

QVector<double> MeasurementForm::getAmplitude(int head, QVector<double> &value)
{
    QVector<double> result;
    double ampValue = 0;
    double x_3 = 0, x_6 = 0, x_Inf = 0;
    double a = 0, b = 0, c = 0;

    int valueSize = value.size();

    for(int i=0; i<valueSize; i++){
        if(head==1){
            x_3 = calib1_3;
            x_6 = calib1_6;
            x_Inf = calib1_inf;
            if(follow1 == true) {
                if(autofollow1 == true)
                    correction1 = average1 - c1;
                x_Inf += correction1;
                x_3 += correction1;
                x_6 += correction1;
            }
        }

        if(head==2){
            x_3 = calib2_3;
            x_6 = calib2_6;
            x_Inf = calib2_inf;
            if(follow2 == true) {
                if(autofollow2 == true)
                    correction2 = average2 - c2;
                x_Inf += correction2;
                x_3 += correction2;
                x_6 += correction2;
            }
        }

        if ((x_3>0) && (x_6>0) && (x_Inf>0)){
            c = x_Inf;
            a = (1.0 / (x_3 - c) - 1.0 / (x_6 - c)) / (9.0 - 36.0); //1/(x*x)
            b = 1.0 / (x_3 - c) - a * 9.0;
        }
        else{
            a = 0;
            b = 0;
            c = 0;
        }

        double critValue = (1 / a) * ( (1 / (value.at(i) - c ) ) - b);
        if(critValue >= 0) ampValue = qSqrt((1 / a) * ( (1 / (value.at(i) - c ) ) - b));
        if ((ampValue < 0.5)||(ampValue >=10)) ampValue = 10;
        result.append(ampValue);
    }

    if ((a==0) && (b==0) && (c==0)) return value;
    else return result;

}

double MeasurementForm::findMin(QVector<double> data)
{
    qSort(data);
    double min = data.first();
    return min;
}

double MeasurementForm::findMax(QVector<double> data)
{
    qSort(data);
    double max = data.last();
    return max;
}

void MeasurementForm::on_clearPushButton_clicked()
{
    if(!wave1List.isEmpty()) wave1List.clear();
    if(!wave2List.isEmpty()) wave2List.clear();
    ui->channel1Widget->clearPlot();
    ui->channel2Widget->clearPlot();
    longAmp1Data.clear();
    longAmp2Data.clear();
    longMarkTime1Data.clear();
    longMarkTime2Data.clear();
    longMarkValue1Data.clear();
    longMarkValue2Data.clear();

}

void MeasurementForm::on_stopPushButton_clicked()
{
    if (isMeasureStopped == false){

        isMeasureStopped = true;
        ui->savePushButton->setEnabled(true);
        sc->blockSignals(true);
        ui->stopPushButton->setText("Resume");
    }
    else {
        isMeasureStopped = false;
        ui->savePushButton->setDisabled(true);
        sc->blockSignals(false);
        ui->stopPushButton->setText("Stop");
    }

}

void MeasurementForm::on_savePushButton_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this);
    QFile file;

    if(!filename.endsWith(".csv", Qt::CaseInsensitive))
        filename.append(".csv");

    file.setFileName(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);

    out << "markValue1\t"; for (int i=0; i<longMarkValue1Data.size(); i++) out << QString::number(longMarkValue1Data.at(i)) + "\t"; out << "\n";
    out << "markTime1\t"; for (int i=0; i<longMarkTime1Data.size(); i++) out << QString::number(longMarkTime1Data.at(i)) + "\t"; out << "\n";
    out << "Ch1\t"; for (int i=0; i<longAmp1Data.size(); i++) out << QString::number(longAmp1Data.at(i)) + "\t"; out << "\n";
    out << "markValue2\t"; for (int i=0; i<longMarkValue2Data.size(); i++) out << QString::number(longMarkValue2Data.at(i)) + "\t"; out << "\n";
    out << "markTime2\t"; for (int i=0; i<longMarkTime2Data.size(); i++) out << QString::number(longMarkTime2Data.at(i)) + "\t"; out << "\n";
    out << "Ch2\t"; for (int i=0; i<longAmp2Data.size(); i++) out << QString::number(longAmp2Data.at(i)) + "\t"; out << "\n";

    file.close();
}

void MeasurementForm::readSerial()
{
    qDebug() << arduino.readSerial(serial);
}
