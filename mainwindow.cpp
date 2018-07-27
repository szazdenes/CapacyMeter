#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->calibrationWidget, &CalibrationForm::signalSendCalibInfo, ui->measurementWidget, &MeasurementForm::slotGetCalibInfo);
    connect(ui->calibrationWidget, &CalibrationForm::signalApplyCalibCH1, ui->measurementWidget, &MeasurementForm::slotApplyCalibCH1);
    connect(ui->calibrationWidget, &CalibrationForm::signalApplyCalibCH2, ui->measurementWidget, &MeasurementForm::slotApplyCalibCH2);
}

MainWindow::~MainWindow()
{
    delete ui;
}
