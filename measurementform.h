#ifndef MEASUREMENTFORM_H
#define MEASUREMENTFORM_H

#include <QWidget>
#include <QInputDialog>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QFile>
#include <QFileDialog>
#include <QDebug>

#include "soundcardreader.h"
#include "arduino.h"

namespace Ui {
class MeasurementForm;
}

class MeasurementForm : public QWidget
{
    Q_OBJECT

public:
    explicit MeasurementForm(QWidget *parent = 0);
    ~MeasurementForm();

public slots:
    void slotGetCalibInfo(QString info);
    void slotApplyCalibCH1();
    void slotApplyCalibCH2();

private slots:
    void on_measurementPushButton_clicked();
    void slotLastWave(QList<qint16> waveForm1, QList<qint16> waveForm2);

    void on_clearPushButton_clicked();

    void on_stopPushButton_clicked();

    void on_savePushButton_clicked();
    void readSerial();

private:
    Ui::MeasurementForm *ui;
    void chooseAudioDevice();
    QPair<QVector<double>, QVector<double>> getNumericIntegral(QList<qint16> &dataList, int waveListSize);
    QVector<double> getAmplitude(int head, QVector<double> &value);
    double findMin(QVector<double> data);
    double findMax(QVector<double> data);

    double integralSamples;
    double integralStep;
    double followParameter;
    double noiseFilter;
    double samplingFreq;
    double samplingNum;
    double amplLimit;
    QList<qint16> wave1List, wave2List;
    bool isMeasureStopped;
    SoundCardReader *sc;
    QVector<double> markValue1, markValue2;
    QVector<double> markTime1, markTime2;
    QVector<double> amplitudes1, amplitudes2;
    QPair<QVector<double>, QVector<double>> integral1, integral2;
    double seconds;
    double calib1_inf, calib1_6, calib1_3;
    double calib2_inf, calib2_6, calib2_3;
    double average1, average2;
    double correction1, correction2, c1, c2;
    bool follow1, autofollow1, follow2, autofollow2;
    double triggerLevel, triggerTime;
    QVector<double> longAmp1Data, longAmp2Data;
    QVector<double> longMarkTime1Data, longMarkTime2Data;
    QVector<double> longMarkValue1Data, longMarkValue2Data;
    int qqq;
    double joinNum;
    double plottingNum;

    Arduino arduino;
    QSerialPort serial;


};

#endif // MEASUREMENTFORM_H
