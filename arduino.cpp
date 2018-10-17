#include "arduino.h"

Arduino::Arduino(QObject *parent) : QObject(parent)
{

}


void Arduino::setSerial(QSerialPort &serial)
{
    QString serialPortName = "ttyACM0";
    serial.setPortName(serialPortName);
    serial.setBaudRate(9600);

    if (!serial.open(QIODevice::ReadWrite)) {
            qDebug() << QObject::tr("Failed to open port %1, error: %2").arg(serialPortName).arg(serial.error());
            return;
    }
}

QString Arduino::readSerial(QSerialPort &serial)
{
    int numOfReadBytes = 0;
    char * dataBuffer;
    QString appendedDataBuffer = "";
    int size = 1023; //serial.bytesAvailable();
    dataBuffer = new char[size+1];            // + 1 byte for '\0'

    while(!appendedDataBuffer.contains("\n")){
        serial.waitForReadyRead();
        numOfReadBytes = serial.readLine(dataBuffer, size);
        dataBuffer[numOfReadBytes] = '\0';                       // terminate the string
        QString dataString(QString::fromUtf8(dataBuffer));
        appendedDataBuffer += dataString;
    }
    delete dataBuffer;

//    QString dataString = QString::fromStdString(serial.readAll().toStdString());

    if (serial.error() == QSerialPort::ReadError) {
        qDebug() << QObject::tr("Failed to read from port %1, error: %2").arg(serial.portName()).arg(serial.errorString());
        return nullptr;
    } else if (serial.error() == QSerialPort::TimeoutError && appendedDataBuffer.isEmpty()) {
        qDebug() << QObject::tr("No data was currently available for reading from port %1").arg(serial.portName());
        return nullptr;
    }

    return appendedDataBuffer;
}

void Arduino::writeSerial(QSerialPort &serial, QString &writeData)
{
    int i = 0;
    int size = writeData.size();

    QString line = "";
    int c = 0;
    while(i < size){
        line.append(writeData[i]);
        if(c == 24){
            int sended = serial.write(line.toUtf8(), 24);
            serial.flush();
            line.clear();
            c = 0;
        }
        i++;
        c++;
    }
    if(c > 0){
        serial.write(line.toUtf8(), c);
    }
    serial.flush();

    if (serial.error() == QSerialPort::WriteError) {
        qDebug() << QObject::tr("Failed to write to port %1, error: %2").arg(serial.portName()).arg(serial.errorString());
        return;
    } else if (serial.error() == QSerialPort::TimeoutError && writeData.isEmpty()) {
        qDebug() << QObject::tr("No data was currently available for reading from port %1").arg(serial.portName());
        return;
    }
}

void Arduino::closeSerial(QSerialPort &serial)
{
    serial.close();
}
