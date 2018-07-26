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
    //void signalGetIntegralValue();

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

    void on_follow1CheckBox_toggled(bool checked);

    void on_follow2CheckBox_toggled(bool checked);

private:
    Ui::CalibrationForm *ui;

    bool follow1, autofollow1, follow2, autofollow2;
    bool isClicked_1_inf, isClicked_1_6, isClicked_1_3;
    bool isClicked_2_inf, isClicked_2_6, isClicked_2_3;
};

#endif // CALIBRATIONFORM_H
