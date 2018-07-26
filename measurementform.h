#ifndef MEASUREMENTFORM_H
#define MEASUREMENTFORM_H

#include <QWidget>
#include <QInputDialog>

#include "soundcardreader.h"

namespace Ui {
class MeasurementForm;
}

class MeasurementForm : public QWidget
{
    Q_OBJECT

public:
    explicit MeasurementForm(QWidget *parent = 0);
    ~MeasurementForm();

private slots:
    void on_measurementPushButton_clicked();
    void slotLastWave(QList<qint16> waveForm1, QList<qint16> waveForm2);

    void on_clearPushButton_clicked();

    void on_stopPushButton_clicked();

private:
    Ui::MeasurementForm *ui;
    void chooseAudioDevice();
    QPair<QVector<double>, QVector<double>> getNumericIntegral(QList<qint16> &dataList, int waveListSize);
    QVector<double> getAmplitude(int head, QVector<double> &value);

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

};

#endif // MEASUREMENTFORM_H
