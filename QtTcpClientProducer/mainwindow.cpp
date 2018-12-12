#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QString>
#include <cstdlib>
#include <ctime>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent), ui(new Ui::MainWindow){
  ui->setupUi(this);
  socket = new QTcpSocket(this);
  tcpConnect();

          connect(ui->pushButtonConnect,
                    SIGNAL(clicked(bool)),
                    this,
                    SLOT(tcpConnect()));

          connect(ui->pushButtonStart,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(startTransmission()));

          connect(ui->pushButtonStop,
                    SIGNAL(clicked(bool)),
                    this,
                    SLOT(stopTransmission()));

          connect(ui->pushButtonDisconnect,
                   SIGNAL(clicked(bool)),
                   this,
                   SLOT(tcpDisconnect()));

  ui->pushButtonConnect->setEnabled(true);
  ui->pushButtonDisconnect->setEnabled(false);
  ui->pushButtonStart->setEnabled(false);
  ui->pushButtonStop->setEnabled(false);
  ui->horizontalSliderMin->setEnabled(false);
  ui->horizontalSliderMax->setEnabled(false);
  ui->horizontalSliderTiming->setEnabled(false);
}

void MainWindow::tcpConnect(){
  //QString str;
  ip = ui->lineEditIP->displayText();
  socket->connectToHost(ip,1234);
    if(socket->waitForConnected(3000)){
      ui->pushButtonConnect->setEnabled(false);
      ui->pushButtonDisconnect->setEnabled(true);
      ui->pushButtonStart->setEnabled(true);
      ui->horizontalSliderMin->setEnabled(true);
      ui->horizontalSliderMax->setEnabled(true);
      ui->horizontalSliderTiming->setEnabled(true);
      qDebug() << "Connected";
      ui->textEditDados->append("Connected");
    }
    else{
      qDebug() << "Disconnected";
    }
 }

void MainWindow::tcpDisconnect(){
    if(socket->state()== QAbstractSocket::ConnectedState){
        socket->disconnectFromHost();
        ui->pushButtonConnect->setEnabled(true);
        ui->pushButtonDisconnect->setEnabled(false);
        ui->pushButtonStart->setEnabled(false);
        ui->pushButtonStop->setEnabled(false);
        ui->horizontalSliderMin->setEnabled(false);
        ui->horizontalSliderMax->setEnabled(false);
        ui->horizontalSliderTiming->setEnabled(false);
        qDebug() << "Disconnected";
    }
}

void MainWindow::startTransmission(){
    if(ui->horizontalSliderMin->value() <= ui->horizontalSliderMax->value()){
         ui->pushButtonStart->setEnabled(false);
         ui->pushButtonStop->setEnabled(true);
         ui->horizontalSliderMin->setEnabled(false);
         ui->horizontalSliderMax->setEnabled(false);
         ui->horizontalSliderTiming->setEnabled(false);
         timer = startTimer(ui->horizontalSliderTiming->value()*1000);
         qDebug() << "Transmission Started";

     }else{
         qDebug()<<"Min tem que ser <= Max";
         ui->pushButtonStart->setEnabled(true);
         ui->pushButtonStop->setEnabled(false);
         ui->horizontalSliderMin->setEnabled(true);
         ui->horizontalSliderMax->setEnabled(true);
         ui->horizontalSliderTiming->setEnabled(true);
     }
}

void MainWindow::stopTransmission()
{

  killTimer(timer);
  timer=0;
  qDebug() << "Transmission stoped";

  ui->pushButtonStart->setEnabled(true);
  ui->pushButtonStop->setEnabled(false);
  ui->horizontalSliderMin->setEnabled(true);
  ui->horizontalSliderMax->setEnabled(true);
  ui->horizontalSliderTiming->setEnabled(true);
}

void MainWindow::timerEvent(QTimerEvent *e)
{
    putData();
}


void MainWindow::putData(){

QString str;
qint64 msecdate;

int min = ui->horizontalSliderMin->value();
int max = ui->horizontalSliderMin->value();



  if(socket->state()== QAbstractSocket::ConnectedState){

    msecdate = QDateTime::currentDateTime().toMSecsSinceEpoch();

    str = "set "+ QString::number(msecdate) + " " + QString::number(qrand()%max+(min+1))+"\r\n";

      qDebug() << str;
      qDebug() << socket->write(str.toStdString().c_str()) << " bytes written";

      ui->textEditDados->append(str);

      if(socket->waitForBytesWritten(3000)){
        qDebug() << "wrote";
      }
  }
}

MainWindow::~MainWindow(){
  delete socket;
  delete ui;
  killTimer(timer);
}
