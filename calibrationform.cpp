#include "calibrationform.h"
#include "ui_calibrationform.h"

CalibrationForm::CalibrationForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CalibrationForm)
{
    ui->setupUi(this);
}

CalibrationForm::~CalibrationForm()
{
    delete ui;
}

void CalibrationForm::on_calibPushButton_1_inf_clicked()
{
    emit signalSendCalibInfo("1_inf");
    ui->calibPushButton_1_inf->setStyleSheet("background-color: green");
}

void CalibrationForm::on_calibPushButton_1_6_clicked()
{
    emit signalSendCalibInfo("1_6");
    ui->calibPushButton_1_6->setStyleSheet("background-color: green");
}

void CalibrationForm::on_calibPushButton_1_3_clicked()
{
    emit signalSendCalibInfo("1_3");
    ui->calibPushButton_1_3->setStyleSheet("background-color: green");
}

void CalibrationForm::on_calibPushButton_2_inf_clicked()
{
    emit signalSendCalibInfo("2_inf");
    ui->calibPushButton_2_inf->setStyleSheet("background-color: green");
}

void CalibrationForm::on_calibPushButton_2_6_clicked()
{
    emit signalSendCalibInfo("2_6");
    ui->calibPushButton_2_6->setStyleSheet("background-color: green");
}

void CalibrationForm::on_calibPushButton_2_3_clicked()
{
    emit signalSendCalibInfo("2_3");
    ui->calibPushButton_2_3->setStyleSheet("background-color: green");
}

void CalibrationForm::on_applyCH1PushButton_clicked()
{
    emit signalApplyCalibCH1();
    ui->calibPushButton_1_inf->setStyleSheet(styleSheet());
    ui->calibPushButton_1_6->setStyleSheet(styleSheet());
    ui->calibPushButton_1_3->setStyleSheet(styleSheet());
}

void CalibrationForm::on_applyCH2PushButton_clicked()
{
    emit signalApplyCalibCH2();
    ui->calibPushButton_2_inf->setStyleSheet(styleSheet());
    ui->calibPushButton_2_6->setStyleSheet(styleSheet());
    ui->calibPushButton_2_3->setStyleSheet(styleSheet());
}
