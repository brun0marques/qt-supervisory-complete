#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QDebug>
#include <QString>
#include <QTime>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();
  
public slots:
  void tcpConnect();
  void putData();
  void tcpDisconnect();
  void startTransmission();
  void stopTransmission();

private:
  Ui::MainWindow *ui;
  QTcpSocket *socket;
  QString ip = "127.0.0.1";
  int timer;
  bool ativar = false;
  void timerEvent(QTimerEvent *e);
};

#endif // MAINWINDOW_H
