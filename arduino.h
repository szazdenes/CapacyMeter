#ifndef ARDUINO_H
#define ARDUINO_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QDebug>
#include <QCoreApplication>
#include <QTime>
#include <QTextCodec>

class Arduino : public QObject
{
    Q_OBJECT
public:
    explicit Arduino(QObject *parent = nullptr);

    void setSerial(QSerialPort &serial);
    QString readSerial(QSerialPort &serial);
    void writeSerial(QSerialPort &serial, QString &writeData);
    void closeSerial(QSerialPort &serial);

signals:

public slots:
};

#endif // ARDUINO_H
