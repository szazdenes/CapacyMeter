#include "measurementform.h"
#include "ui_measurementform.h"

MeasurementForm::MeasurementForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MeasurementForm)
{
    ui->setupUi(this);
   // ui->channel1Widget->setPlottingData("alma", QVector<double>{1, 2, 3}, QVector<double>{1, 3, 5}, QwtText("time"), QwtText("distance"), QColor(Qt::red), 0.0, 10.0, 0.0, 10.0);

    wave1List.clear();
    wave2List.clear();

    amplLimit = 0;
    samplingFreq = 192000;
    samplingNum = 1000;
    integralSamples = 10000;
    integralStep = 200;
    followParameter = 3900;//10000;

    seconds = 0;

    ui->stopPushButton->setDisabled(true);
    ui->savePushButton->setDisabled(true);

    isMeasureStopped = false;
}

MeasurementForm::~MeasurementForm()
{
    delete sc;
    delete ui;
}

void MeasurementForm::on_measurementPushButton_clicked()
{
    chooseAudioDevice();

    ui->stopPushButton->setEnabled(true);
    ui->savePushButton->setDisabled(true);
    ui->measurementPushButton->setDisabled(true);

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

        integral1 = getNumericIntegral(wave1List, wave1ListSize);
        integral2 = getNumericIntegral(wave2List, wave2ListSize);

        markValue1 = integral1.first; //1. elem: markValue, 2. elem: Integrals
        markValue2 = integral2.second;

        int markValue1Size = markValue1.size();
        int markValue2Size = markValue2.size();

        for (int j=0; j<markValue1Size; j++) markTime1.append(seconds+(markValue1.at(j)/samplingFreq));
        for (int j=0; j<markValue2Size; j++) markTime2.append(seconds+(markValue2.at(j)/samplingFreq));

        seconds += ((double)samplingNum)/((double)samplingFreq);

    }


}

void MeasurementForm::chooseAudioDevice()
{
    QList<QAudioDeviceInfo> devices = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);
    QStringList deviceNames;
    for(int i = 0; i < devices.size(); i++)
        deviceNames.append(devices.at(i).deviceName());
    QString deviceName = QInputDialog::getItem(0, "Device", "Device", deviceNames, 0, false);

    if (deviceNames.indexOf(deviceName) < 0)
        return;

    setWindowTitle(deviceName);

    sc = new SoundCardReader(devices.at(deviceNames.indexOf(deviceName)), samplingFreq, samplingNum, this);

    connect(sc, &SoundCardReader::signalLastWave, this, &MeasurementForm::slotLastWave);
    sc->blockSignals(true);

}

QPair<QVector<double>, QVector<double> > MeasurementForm::getNumericIntegral(QList<qint16> dataList, int waveListSize)
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

void MeasurementForm::on_clearPushButton_clicked()
{
    if(!wave1List.isEmpty()) wave1List.clear();
    if(!wave2List.isEmpty()) wave2List.clear();
    ui->channel1Widget->clearPlot();
    ui->channel2Widget->clearPlot();

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
