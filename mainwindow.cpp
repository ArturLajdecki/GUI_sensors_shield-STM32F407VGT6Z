#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QList>
#include <QSerialPortInfo>
#include <QDateTime>
#include <QSerialPort>
#include <QSlider>
#include <QSpinBox>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->device = new QSerialPort(this);


    connect(ui->horizontalSliderThermometer1,SIGNAL(valueChanged(int)),ui->spinBoxThermometer1,SLOT(setValue(int)));
    connect(ui->spinBoxThermometer1,SIGNAL(valueChanged(int)),ui->horizontalSliderThermometer1,SLOT(setValue(int)));
    connect(ui->spinBoxThermometer1,SIGNAL(editingFinished()),this,SLOT(on_horizontalSliderThermometer1_sliderReleased()));

    connect(ui->horizontalSliderThermometer2,SIGNAL(valueChanged(int)),ui->spinBoxThermometer2,SLOT(setValue(int)));
    connect(ui->spinBoxThermometer2,SIGNAL(valueChanged(int)),ui->horizontalSliderThermometer2,SLOT(setValue(int)));
    connect(ui->spinBoxThermometer2,SIGNAL(editingFinished()),this,SLOT(on_horizontalSliderThermometer2_sliderReleased()));

    connect(ui->horizontalSliderThermometer3,SIGNAL(valueChanged(int)),ui->spinBoxThermometer3,SLOT(setValue(int)));
    connect(ui->spinBoxThermometer3,SIGNAL(valueChanged(int)),ui->horizontalSliderThermometer3,SLOT(setValue(int)));
    connect(ui->spinBoxThermometer3,SIGNAL(editingFinished()),this,SLOT(on_horizontalSliderThermometer3_sliderReleased()));

    connect(ui->horizontalSliderThermometer4,SIGNAL(valueChanged(int)),ui->spinBoxThermometer4,SLOT(setValue(int)));
    connect(ui->spinBoxThermometer4,SIGNAL(valueChanged(int)),ui->horizontalSliderThermometer4,SLOT(setValue(int)));
    connect(ui->spinBoxThermometer4,SIGNAL(editingFinished()),this,SLOT(on_horizontalSliderThermometer4_sliderReleased()));

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButtonSearch_clicked()
{
    ui->comboBoxDevices->clear();

    this->addToLogs("Searching for active devices.");
    QList<QSerialPortInfo> devices;
    devices = QSerialPortInfo::availablePorts();
    for(int i = 0; i < devices.count(); i++)
    {
      QString printable = QStringLiteral("%1").arg(i);
      this->addToLogs("Device " + printable + "." + devices.at(i).portName() + " " + devices.at(i).description());
      ui->comboBoxDevices->addItem(devices.at(i).portName() + "\t" + devices.at(i).description());
    }
}


void MainWindow::addToLogs(QString message)
{
    QString currentDateTime = QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss");
    ui->textEditLogs->append(currentDateTime + "\t" + message);
}

void MainWindow::addToThermometer(QString message)
{
    qDebug() << message;

    //if(message[0]=='0' && message[1]=='1' && message[2]=='0' && message[3]=='1' && message[4]=='0' && message[5]=='1' && message[6]=='0')
    if(message.startsWith("0101010"))
    {
        if(message[7]=='1')
        {
            ui->textEditThermometer1->clear();
            ui->textEditThermometer1->append(message.remove(0,8));
        }
        else if(message[7]=='2')
        {
            ui->textEditThermometer2->clear();
            ui->textEditThermometer2->append(message.remove(0,8));
        }
        else if(message[7]=='3')
        {
            ui->textEditThermometer3->clear();
            ui->textEditThermometer3->append(message.remove(0,8));
        }
        else if(message[7]=='4')
        {
            ui->textEditThermometer4->clear();
            ui->textEditThermometer4->append(message.remove(0,8));
        }
    }
    else
    {
        addToLogs(message);
    }

}

void MainWindow::on_pushButtonConnect_clicked()
{
    if(ui->comboBoxDevices->count() == 0) {
      this->addToLogs("No devices have been found!");
      return;
    }

    QString comboBoxQString = ui->comboBoxDevices->currentText();
    QStringList portList = comboBoxQString.split("\t");
    QString portName = portList.first();

    this->device->setPortName(portName);

    // OTWÓRZ I SKONFIGURUJ PORT:
    if(!device->isOpen())
    {
        if(device->open(QSerialPort::ReadWrite)) {
          this->device->setBaudRate(QSerialPort::Baud115200);
          this->device->setDataBits(QSerialPort::Data8);
          this->device->setParity(QSerialPort::NoParity);
          this->device->setStopBits(QSerialPort::OneStop);
          this->device->setFlowControl(QSerialPort::NoFlowControl);


          connect(this->device, SIGNAL(readyRead()), this, SLOT(readFromPort()));

          this->addToLogs("COM port has been opened.");
        } else {
          this->addToLogs("Error during opening COM port!");
        }
    }
    else
    {
      this->addToLogs("Port is now opened!");
      return;
    }

}

void MainWindow::on_pushButtonDisconnect_clicked()
{
    if(this->device->isOpen())
    {
      this->device->close();
      this->addToLogs("Connection has been closed.");
    }
    else
    {
      this->addToLogs("COM port is not opened!");
      return;
    }
}

void MainWindow::readFromPort()
{
    while(this->device->canReadLine())
    {
        QString line = this->device->readLine();
        //QString line = this->device->readAll();
        //qDebug() << line.split("\r\n");

        QString terminator = "\r\n";
        int pos = line.lastIndexOf(terminator);
        //qDebug() << line.left(pos);

        this->addToThermometer(line.left(pos));
    }
}

void MainWindow::sendMessageToDevice(QString message)
{
    if(this->device->isOpen() && this->device->isWritable())
    {
        this->addToLogs("Sending information to the device " + message);
        this->device->write(message.toStdString().c_str());
    }
    else
    {
        this->addToLogs("Error whilst sending a message. Port is not opened!");
    }
}

void MainWindow::on_pushButtonThermometer1_clicked()
{
    this->sendMessageToDevice("1010101500000");
}

void MainWindow::on_pushButtonThermometer2_clicked()
{
    this->sendMessageToDevice("1010101600000");
}

void MainWindow::on_pushButtonThermometer3_clicked()
{
    this->sendMessageToDevice("1010101700000");
}

void MainWindow::on_pushButtonThermometer4_clicked()
{
    this->sendMessageToDevice("1010101800000");
}


void MainWindow::on_horizontalSliderThermometer1_sliderReleased()
{
    int value = ui->horizontalSliderThermometer1->value();
    QString printable = QStringLiteral("%1").arg(value);
    if( value>=0 && value<10 )
    {
        this->sendMessageToDevice("1010101100"+printable+"00");
    }
    else if(value>=10 && value<100)
    {
        this->sendMessageToDevice("101010110"+printable+"00");
    }
    else if(value>=100 && value<=999)
    {
        this->sendMessageToDevice("10101011"+printable+"00");
    }
}


void MainWindow::on_horizontalSliderThermometer2_sliderReleased()
{
    int value = ui->horizontalSliderThermometer2->value();
    QString printable = QStringLiteral("%1").arg(value);
    if( value>=0 && value<10 )
    {
        this->sendMessageToDevice("1010101200"+printable+"00");
    }
    else if(value>=10 && value<100)
    {
        this->sendMessageToDevice("101010120"+printable+"00");
    }
    else if(value>=100 && value<=999)
    {
        this->sendMessageToDevice("10101012"+printable+"00");
    }
}

void MainWindow::on_horizontalSliderThermometer3_sliderReleased()
{
    int value = ui->horizontalSliderThermometer3->value();
    QString printable = QStringLiteral("%1").arg(value);
    if( value>=0 && value<10 )
    {
        this->sendMessageToDevice("1010101300"+printable+"00");
    }
    else if(value>=10 && value<100)
    {
        this->sendMessageToDevice("101010130"+printable+"00");
    }
    else if(value>=100 && value<=999)
    {
        this->sendMessageToDevice("10101013"+printable+"00");
    }
}

void MainWindow::on_horizontalSliderThermometer4_sliderReleased()
{
    int value = ui->horizontalSliderThermometer4->value();
    QString printable = QStringLiteral("%1").arg(value);
    if( value>=0 && value<10 )
    {
        this->sendMessageToDevice("1010101400"+printable+"00");
    }
    else if(value>=10 && value<100)
    {
        this->sendMessageToDevice("101010140"+printable+"00");
    }
    else if(value>=100 && value<=999)
    {
        this->sendMessageToDevice("10101014"+printable+"00");
    }
}
