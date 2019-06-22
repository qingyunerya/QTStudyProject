#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QTime>
#include <QTextCursor>
#include <QMetaMethod>
#include <QTimer>
#include  <QGraphicsOpacityEffect>
#include <QNetworkInterface>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->tcp_server = new QTcpServer(this);
    connect(tcp_server,SIGNAL(newConnection()),this,SLOT(newConnection()));
    data_timer=new QTimer(this);
    data_timer->setInterval(100);
    connect(data_timer,SIGNAL(timeout()),this,SLOT(data_timer_tick()));
    this->tcp_client=new QTcpSocket(this);
    connect(this->tcp_client,SIGNAL(connected()),this,SLOT(connected()));
    connect(this->tcp_client,SIGNAL(disconnected()),this,SLOT(Cdisconnected()));
    connect(this->tcp_client,SIGNAL(readyRead()),this,SLOT(readyread()));
    //获取所有网络接口的列表
    foreach(QNetworkInterface interface,QNetworkInterface::allInterfaces())
    {
        QList<QNetworkAddressEntry> entryList = interface.addressEntries();
        foreach(QNetworkAddressEntry entry,entryList)
        {
            if(entry.ip().toString().contains("192.168."))
            {
                ui->lineEdit_ca->setText(entry.ip().toString());
                ui->lineEdit_cp->setText("8080");
                qDebug()<<"ipdrr"<<entry.ip().toString();
            }
        }
    }
    commandList.append("control_key1_click");
    commandList.append("control_key2_click");
    commandList.append("control_key3_click");
    commandList.append("control_led1_click");
    commandList.append("control_led2_click");
    commandList.append("control_led3_click");
    commandList.append("control_readKeys_click");
    commandList.append("control_directControl_click");
    commandList.append("control_recLedOp_click");
    commandList.append("control_recOp_click");
    commandList.append("control_viewLedOp_click");
    commandList.append("control_viewOp_click");
    reCommandList.append("remote_status_isReadkeys:");
    reCommandList.append("remote_status_isdirectControl:");
    reCommandList.append("remote_status_l1:");
    reCommandList.append("remote_status_l2:");
    reCommandList.append("remote_status_l3:");
    //ui->groupBox->setEnabled(false);
    QPixmap tb=QPixmap(":/Background/tb");
    tb.scaled(ui->centralWidget->size(),Qt::KeepAspectRatio);
    ui->centralWidget->setAutoFillBackground(true);
    palette.setBrush(QPalette::Window,QBrush(tb));
    ui->centralWidget->setPalette(palette);
    debug2("远程控制不可用");
}

MainWindow::~MainWindow()
{
    delete ui;
}
//调试输出
void MainWindow::debug(QString str)
{
    QString time;
    QTime current_time =QTime::currentTime();
    time=current_time.toString("hh:mm:ss:");
    consoleStr=consoleStr+"\n"+time+str;
    ui->textEdit_console->setText(consoleStr);
    ui->textEdit_console->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
}

void MainWindow::debug2(QString str)
{
    ui->textEdit_console_2->setText(str);
    ui->textEdit_console_2->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
}
//监听端口
void MainWindow::on_connectToClient_clicked()
{
    QString sport=ui->lineEdit_sp->text();
    if(QString(sport).isEmpty()){ui->lineEdit_sp->setText("8080");sport="8080";}
    quint16 port=quint16(sport.toUInt());
    if(!tcp_server->isListening())
    {
        tcp_server->listen(QHostAddress::Any,port);
        if(tcp_server->isListening())ui->connectToClient->setText("停止监听");
        data_timer->start(0);
        debug("监听端口"+sport+"中");
    }
    else
    {
        this->tcp_server->close();
        if(!tcp_server->isListening())
        ui->connectToClient->setText("监听端口");
        QTcpSocket *client_socket;
        foreach (client_socket, this->ClientSocketList)
        {
            debug(client_socket->peerAddress().toString());
            int i=ui->comboBox->findText(client_socket->peerAddress().toString());
            ui->comboBox->removeItem(i);
            debug("清理"+QString::number(i,10));
            client_socket->close();
            if(client_socket!=nullptr)
            {
               client_socket->destroyed();
               debug("清理socket"+QString::number(i,10));
            }
        }
        data_timer->stop();
        this->ClientSocketList.clear();
        debug("已停止监听");
    }
}
//连接到客户端
void MainWindow::newConnection()
{
    client=tcp_server->nextPendingConnection();
    ui->comboBox->addItem(client->peerAddress().toString());
    ui->lineEdit_sp->setText(QString::number(client->peerPort(),10));
    connect(client,SIGNAL(readyRead()),this,SLOT(ReceiveData()));
    connect(client,SIGNAL(Sdisconnected()),this,SLOT(Sdisconnected()));
    this->ClientSocketList.append(client); 
    debug("连接到一个客户端");
    QTcpSocket *client_socket;
    foreach (client_socket, this->ClientSocketList)
    {
        if(client_socket->peerAddress().toString()==ui->comboBox->currentText()){
        this->client=client_socket;
        }
    }
    debug2("远程控制已启用");
    isRemoteControl=true;
}

//服务端发送数据
void MainWindow::on_s_send_clicked()
{
    QString time;
    QTime current_time =QTime::currentTime();
    time=current_time.toString("hh:mm:ss.zzz");
    QString str=time+ui->textEdit_ss->toPlainText();
    this->client->write(str.toUtf8());
}

//接受到客户端数据
void MainWindow::ReceiveData()
{
    debug("接受到来自"+client->peerAddress().toString()+"\n"+"客户端的数据");
    QString sarr=QString::fromUtf8(client->readAll());
    ui->textEdit_sa->setText(sarr);
    ui->textEdit_sa->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
}
//断开服务端连接
void MainWindow::Sdisconnected()
{
    QTcpSocket *client_socket;
    foreach (client_socket, this->ClientSocketList)
    {
        if(client_socket->peerAddress().toString()==ui->comboBox->currentText()){
        int i=ui->comboBox->findText(client->peerAddress().toString());
        ui->comboBox->removeItem(i);
        client->destroyed();
        }
    }
    if(ui->comboBox->currentText().trimmed().isEmpty())
    {
        //ui->groupBox->setEnabled(false);
        debug2("远程控制已关闭");
        debug(client->peerAddress().toString()+"已断开服务端连接");
        isRemoteControl=false;
        ui->textEdit_sa->clear();
        ui->textEdit_ss->clear();
    }

}
//发送命令
void MainWindow::sendCommand(QString str)
{
    QString time;
    QTime current_time =QTime::currentTime();
    time=current_time.toString("hh:mm:ss.zzz");
    str=time+":"+str;
    QByteArray ba=str.toUtf8();
    this->client->write(ba);
}
//接收命令
QString MainWindow::acceptCommand()
{
    QString carr=ui->textEdit_sa->toPlainText();
    return carr;
}
void MainWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{
    QTcpSocket *client_socket;
    foreach (client_socket, this->ClientSocketList)
    {
        if(client_socket->peerAddress().toString()==arg1){
        this->client=client_socket;
        ui->lineEdit_sp->setText(QString::number(client->peerPort(),10));
        }
    }
    debug("与"+arg1+"通信中");
}
void MainWindow::data_timer_tick()
{

}
//发送数据
void MainWindow::on_c_send_clicked()
{
     QString str=ui->textEdit_cs->toPlainText();
     QByteArray ba=str.toLocal8Bit();
     this->tcp_client->write(ba);
}
//连接服务端
void MainWindow::on_connectToSever_clicked()
{
    QString address=ui->lineEdit_ca->text();
    QString sport=ui->lineEdit_cp->text();
    quint16 port=quint16(sport.toUInt());
    if(!this->tcp_client->isOpen())
    {
        this->tcp_client->connectToHost(address,port,QTcpSocket::ReadWrite);
        ui->connectToSever->setText(QStringLiteral("断开连接"));
        debug(QStringLiteral("连接服务器"));
    }
    else
    {
        tcp_client->close();
        debug(QString::number(this->tcp_client->isOpen())+QStringLiteral("测试"));
        ui->connectToSever->setText(QStringLiteral("连接服务器"));
        debug(QStringLiteral("断开连接"));
    }
}

//连接到服务器
void MainWindow::connected()
{
    debug(QStringLiteral("连接服务器成功"));
}
//开始读取数据
void MainWindow::readyread()
{
     debug(QStringLiteral("接受到服务端数据"));
     QString carr=QString::fromLocal8Bit(this->tcp_client->readAll());
     ui->textEdit_ca->setText(carr);
}
//断开服务端连接
void MainWindow::Cdisconnected()
{
    debug(QStringLiteral("已断开服务端连接"));
    ui->connectToSever->setText(QStringLiteral("连接服务端"));
}
