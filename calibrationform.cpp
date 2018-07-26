#include "calibrationform.h"
#include "ui_calibrationform.h"

CalibrationForm::CalibrationForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CalibrationForm)
{
    ui->setupUi(this);

    follow1 = false;
    autofollow1 = false;
    follow2 = false;
    autofollow2 = false;
}

CalibrationForm::~CalibrationForm()
{
    delete ui;
}

void CalibrationForm::on_calibPushButton_1_inf_clicked()
{

}

void CalibrationForm::on_calibPushButton_1_6_clicked()
{

}

void CalibrationForm::on_calibPushButton_1_3_clicked()
{

}

void CalibrationForm::on_calibPushButton_2_inf_clicked()
{

}

void CalibrationForm::on_calibPushButton_2_6_clicked()
{

}

void CalibrationForm::on_calibPushButton_2_3_clicked()
{

}

void CalibrationForm::on_applyCH1PushButton_clicked()
{

}

void CalibrationForm::on_applyCH2PushButton_clicked()
{

}

void CalibrationForm::on_follow1CheckBox_toggled(bool checked)
{

}

void CalibrationForm::on_follow2CheckBox_toggled(bool checked)
{

}
