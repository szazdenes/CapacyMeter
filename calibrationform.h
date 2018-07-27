#ifndef CALIBRATIONFORM_H
#define CALIBRATIONFORM_H

#include <QWidget>

namespace Ui {
class CalibrationForm;
}

class CalibrationForm : public QWidget
{
    Q_OBJECT

public:
    explicit CalibrationForm(QWidget *parent = 0);
    ~CalibrationForm();

signals:
    void signalSendCalibInfo(QString info);
    void signalApplyCalibCH1();
    void signalApplyCalibCH2();

public slots:


private slots:
    void on_calibPushButton_1_inf_clicked();

    void on_calibPushButton_1_6_clicked();

    void on_calibPushButton_1_3_clicked();

    void on_calibPushButton_2_inf_clicked();

    void on_calibPushButton_2_6_clicked();

    void on_calibPushButton_2_3_clicked();

    void on_applyCH1PushButton_clicked();

    void on_applyCH2PushButton_clicked();

private:
    Ui::CalibrationForm *ui;
};

#endif // CALIBRATIONFORM_H
