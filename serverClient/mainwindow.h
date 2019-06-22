#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class QTcpSocket;
class QTcpServer;
class QTimer;
class QGroupBox;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void debug(QString str);
    void debug2(QString str);
    QTimer *data_timer;
    QList<QString> commandList;
    QList<QString> reCommandList;
    QList<QTcpSocket *> ClientSocketList;
    QTcpSocket *tcp_client;
    QString consoleStr;
    QTimer *ani_timer;
    QGroupBox *gb;
    QTcpServer *tcp_server;
    QTcpSocket *client;
    QString consoleStr2;
    QString curCommand;
    QString befCommand="null";
    bool isStartReadKeys=false;
    void sendCommand(QString str);
    QString acceptCommand();
    bool isReadKeys=false;
    bool isControlLed=false;
    bool isRemoteControl=false;
    QPalette palette;
    bool l1=false,l2=false,l3=false;

private slots:

    void readyread();
    void connected();
    void Sdisconnected();
    void on_connectToSever_clicked();
    void on_c_send_clicked();
    void newConnection();
    void ReceiveData();
    void Cdisconnected();
    void on_connectToClient_clicked();
    void on_s_send_clicked();
    void data_timer_tick();
    void on_comboBox_currentIndexChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
