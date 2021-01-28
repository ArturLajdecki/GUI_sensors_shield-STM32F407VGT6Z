#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QList>
#include <QSerialPort>
#include <QSlider>
#include <QSpinBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_pushButtonSearch_clicked();

    void on_pushButtonConnect_clicked();

    void on_pushButtonDisconnect_clicked();

    void readFromPort();

    void sendMessageToDevice(QString message);

    void on_pushButtonThermometer1_clicked();

    void on_pushButtonThermometer2_clicked();

    void on_pushButtonThermometer3_clicked();

    void on_pushButtonThermometer4_clicked();

    void on_horizontalSliderThermometer1_sliderReleased();

    void on_horizontalSliderThermometer2_sliderReleased();

    void on_horizontalSliderThermometer3_sliderReleased();

    void on_horizontalSliderThermometer4_sliderReleased();

private:
    Ui::MainWindow *ui;
    void addToLogs(QString message);
    void addToThermometer(QString message);
    QSerialPort *device;

};
#endif // MAINWINDOW_H
