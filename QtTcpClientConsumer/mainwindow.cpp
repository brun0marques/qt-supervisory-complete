#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QTimer>
#include <iostream>
#include <QList>
#include <vector>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    t = new QTimer(this);

    ui->widget->setDados(dados);

    connect(ui->pushButtonConnection,
            SIGNAL(clicked(bool)),
            this,
            SLOT(tcpConnect()));

    connect(ui->horizontalSliderTiming,
            SIGNAL(valueChanged(int)),
            this,
            SLOT(changeTiming(int)));

    connect(t,
            SIGNAL(timeout()),
            this,
            SLOT(getData()));

    connect(ui->pushButtonUpdate,
            SIGNAL(clicked(bool)),
            this,
            SLOT(getIPs()));

    connect(ui->pushButtonGet,
            SIGNAL(clicked(bool)),
            this,
            SLOT(setRunON()));

    connect(ui->pushButtonStop,
            SIGNAL(clicked(bool)),
            this,
            SLOT(setRunOFF()));

    connect(ui->listWidget,
            SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
            this,
            SLOT(IP_ON()));

    ui->pushButtonUpdate->setEnabled(false);
    ui->horizontalSliderTiming->setEnabled(false);
    ui->pushButtonGet->setEnabled(false);
    ui->pushButtonStop->setEnabled(false);
}

//PRONTO
void MainWindow::tcpConnect()
{
    socket->connectToHost(ui->lineEditIP->displayText(), 1234);

    if(socket->waitForConnected(4000))
    {
        qDebug() << "Connected";

        ui->statusBar->showMessage("Connected");

        disconnect(ui->pushButtonConnection, SIGNAL(clicked(bool)), this, SLOT(tcpConnect()));
        ui->pushButtonConnection->setText("Disconnect");
        connect(ui->pushButtonConnection, SIGNAL(clicked(bool)), this, SLOT(tcpDisconnect()));

        ui->pushButtonUpdate->setEnabled(true);
        ui->horizontalSliderTiming->setEnabled(true);
        ui->pushButtonGet->setEnabled(true);
        ui->pushButtonStop->setEnabled(true);
    }
    else
    {
        qDebug() << "Disconnected";
    }
}

//PRONTO
void MainWindow::tcpDisconnect()
{
    socket->disconnectFromHost();

    qDebug() << "Disconnected";

    ui->statusBar->showMessage("Disconnected");

    disconnect(ui->pushButtonConnection, SIGNAL(clicked(bool)), this, SLOT(tcpDisconnect()));
    ui->pushButtonConnection->setText("Connect");
    connect(ui->pushButtonConnection, SIGNAL(clicked(bool)), this, SLOT(tcpConnect()));

    ui->pushButtonUpdate->setEnabled(false);
    ui->horizontalSliderTiming->setEnabled(false);
    ui->pushButtonGet->setEnabled(false);
    ui->pushButtonStop->setEnabled(false);
}


void MainWindow::IP_ON(){
    ipSelecionado = true;
}

void MainWindow::setRunON()
{
    t->start(timing);
}

void MainWindow::setRunOFF()
{
    t->stop();
}

void MainWindow::changeTiming(int _timing)
{
    if(_timing < 1) _timing = 1;
    ui->labelTiming->setNum(_timing);
    timing = _timing*1000;
    if(t->isActive()){
        t->start(timing);
    }
}

void MainWindow::getData(){
    QString str, comandoGet;
    QByteArray array;
    QStringList list;
    qint64 thetime;
    QStringList linha;

    qDebug() << "to get data...";
    if(socket->state() == QAbstractSocket::ConnectedState){
        if(socket->isOpen()){
            if(ipSelecionado == false){
                qDebug() << "Nenhum ip selecionado!";
            }
            else{
                qDebug() << "reading...";
                qDebug() << "TESTANDO: " << ui->listWidget->currentItem()->text().replace("\"", "") << endl;
                comandoGet = "get "+ ui->listWidget->currentItem()->text() + " 30\r\n";


                socket->write(comandoGet.toStdString().c_str());
                socket->waitForBytesWritten();
                socket->waitForReadyRead();
                qDebug() << socket->bytesAvailable();
                int i=0;
                dados.clear();
                while(socket->bytesAvailable()){
                    str = socket->readLine().replace("\n","").replace("\r","");
                    linha = str.split(" ");
                    dados.push_back(Data());
                    dados[i].valor = linha[1].toInt();
                    dados[i].tempo = linha[0].toLongLong();

                    list = str.split(" ");
                    if(list.size() == 2){
                        bool ok;
                        str = list.at(0);
                        thetime = str.toLongLong(&ok);
                        str = list.at(1);
                        qDebug() << thetime << ": " << str;
                    }
                    i++;
                }
                ui->widget->setDados(dados);
            }
        }
    }
}

void MainWindow::getIPs(){
    QStringList IPs;
    if(socket->state() == QAbstractSocket::ConnectedState){
        if(socket->isOpen()){
            socket->write("list");
            socket->waitForBytesWritten();
            socket->waitForReadyRead();

            while(socket->bytesAvailable()){
                IPs.append(socket->readLine().replace("\n","").replace("\r",""));
            }
            ui->listWidget->clear();
            ui->listWidget->addItems(IPs);
        }
    }
}

void MainWindow::moldaVector(std::vector<Data> dados){
    int k = dados.size()-1;
    for(int i = 29; i>=0; i--){
        dados[i] = dados[k];
        k--;
    }
    dados.resize(30);
}

MainWindow::~MainWindow()
{
    delete socket;
    delete ui;
}
