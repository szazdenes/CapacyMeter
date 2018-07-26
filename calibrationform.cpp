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

    isClicked_1_inf = false;
    isClicked_1_6 = false;
    isClicked_1_3 = false;
    isClicked_2_inf = false;
    isClicked_2_6 = false;
    isClicked_2_3 = false;
}

CalibrationForm::~CalibrationForm()
{
    delete ui;
}

void CalibrationForm::slotReceivedIntegral1Value(double value)
{
    QFile outFile;
    QTextStream out;

    if(isClicked_1_inf == true){
        outFile.setFileName("calib1_inf.txt");
        outFile.open(QIODevice::WriteOnly | QIODevice::Text);
        out.setDevice(&outFile);
        out << QString::number(value);
        outFile.close();
        ui->calibPushButton_1_inf->setStyleSheet("background-color: green");
    }

    if(isClicked_1_6 == true){
        outFile.setFileName("calib1_6.txt");
        outFile.open(QIODevice::WriteOnly | QIODevice::Text);
        out.setDevice(&outFile);
        out << QString::number(value);
        outFile.close();
        ui->calibPushButton_1_6->setStyleSheet("background-color: green");
    }

    if(isClicked_1_3 == true){
        outFile.setFileName("calib1_3.txt");
        outFile.open(QIODevice::WriteOnly | QIODevice::Text);
        out.setDevice(&outFile);
        out << QString::number(value);
        outFile.close();
        ui->calibPushButton_1_3->setStyleSheet("background-color: green");
    }

    isClicked_1_inf = false;
    isClicked_1_6 = false;
    isClicked_1_3 = false;
}

void CalibrationForm::slotReceivedIntegral2Value(double value)
{
    QFile outFile;
    QTextStream out;

    if(isClicked_2_inf == true){
        outFile.setFileName("calib2_inf.txt");
        outFile.open(QIODevice::WriteOnly | QIODevice::Text);
        out.setDevice(&outFile);
        out << QString::number(value);
        outFile.close();
        ui->calibPushButton_2_inf->setStyleSheet("background-color: green");
    }

    if(isClicked_2_6 == true){
        outFile.setFileName("calib2_6.txt");
        outFile.open(QIODevice::WriteOnly | QIODevice::Text);
        out.setDevice(&outFile);
        out << QString::number(value);
        outFile.close();
        ui->calibPushButton_2_6->setStyleSheet("background-color: green");
    }

    if(isClicked_2_3 == true){
        outFile.setFileName("calib2_3.txt");
        outFile.open(QIODevice::WriteOnly | QIODevice::Text);
        out.setDevice(&outFile);
        out << QString::number(value);
        outFile.close();
        ui->calibPushButton_2_3->setStyleSheet("background-color: green");
    }

    isClicked_2_inf = false;
    isClicked_2_6 = false;
    isClicked_2_3 = false;
}

void CalibrationForm::on_calibPushButton_1_inf_clicked()
{
    isClicked_1_inf = true;
    emit signalGetIntegral1Value();
}

void CalibrationForm::on_calibPushButton_1_6_clicked()
{
    isClicked_1_6 = true;
    emit signalGetIntegral1Value();
}

void CalibrationForm::on_calibPushButton_1_3_clicked()
{
    isClicked_1_3 = true;
    emit signalGetIntegral1Value();
}

void CalibrationForm::on_calibPushButton_2_inf_clicked()
{
    isClicked_2_inf = true;
    emit signalGetIntegral2Value();
}

void CalibrationForm::on_calibPushButton_2_6_clicked()
{
    isClicked_2_6 = true;
    emit signalGetIntegral2Value();
}

void CalibrationForm::on_calibPushButton_2_3_clicked()
{
    isClicked_2_3 = true;
    emit signalGetIntegral2Value();
}

void CalibrationForm::on_applyCH1PushButton_clicked()
{
    ui->calibPushButton_1_inf->setStyleSheet(styleSheet());
    ui->calibPushButton_1_6->setStyleSheet(styleSheet());
    ui->calibPushButton_1_3->setStyleSheet(styleSheet());
}

void CalibrationForm::on_applyCH2PushButton_clicked()
{
    ui->calibPushButton_2_inf->setStyleSheet(styleSheet());
    ui->calibPushButton_2_6->setStyleSheet(styleSheet());
    ui->calibPushButton_2_3->setStyleSheet(styleSheet());
}

void CalibrationForm::on_follow1CheckBox_toggled(bool checked)
{

}

void CalibrationForm::on_follow2CheckBox_toggled(bool checked)
{

}
