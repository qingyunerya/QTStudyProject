#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QPushButton>
#include <QFileDialog>
#include <QTextStream>
#include <QDialog>
#include <QLineEdit>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    isUntitled=true;
    curFile=tr("未命名.txt");
    setWindowTitle(curFile);
    finDlg =new QDialog(this);
    finDlg->setWindowTitle(tr("查找"));
    findLineEdit =new QLineEdit(finDlg);
    QPushButton *btn=new QPushButton(tr("查找下一个"),finDlg);
    QVBoxLayout *layout=new QVBoxLayout(finDlg);
    layout->addWidget(findLineEdit);
    layout->addWidget(btn);
    connect(btn,&QPushButton::clicked,this,&MainWindow::showFindText);
    //ui->statusBar->showMessage("夕餐菊英",2000);
    statusLabel=new QLabel;
    statusLabel->setMinimumSize(150,50);
    statusLabel->setFrameShape(QFrame::WinPanel);
    statusLabel->setFrameShadow(QFrame::Sunken);
    ui->statusBar->addWidget(statusLabel);
    statusLabel->setText("夕餐菊英!");
    QLabel *permanent=new QLabel;
    permanent->setFrameStyle(QFrame::Box|QFrame::Sunken);
    permanent->setText(tr("青云尔雅"));
    permanent->setTextFormat(Qt::RichText);
    permanent->setOpenExternalLinks(true);
    ui->statusBar->addPermanentWidget(permanent);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow ::newFile()
{
    if(maybeSave())
    {
        isUntitled=true;
        curFile=tr("未命名.txt");
        setWindowTitle(curFile);
        ui->textEdit->clear();
        ui->textEdit->setVisible(true);
    }

}
bool MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QFile::ReadOnly|QFile::Text))
    {
        QMessageBox::warning(this,tr("多文档编辑器"),tr("无法读取文件%1:\n%2").arg(fileName).arg(file.errorString()));
        return  false;
    }
    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    ui->textEdit->setPlainText(in.readAll());
    QApplication::restoreOverrideCursor();
    curFile=QFileInfo(fileName).canonicalFilePath();
    setWindowTitle(curFile);
    return  true;
}
bool MainWindow ::maybeSave()
{
    if(ui->textEdit->document()->isModified())
    {
        QMessageBox box;
        box.setWindowTitle(tr("警告"));
        box.setIcon(QMessageBox::Warning);
        box.setText(curFile+tr("尚未保存,是否保存?"));
        QPushButton *yesBtn=box.addButton(tr("是(&Y)"),QMessageBox::YesRole);
        box.addButton(tr("否&N"),QMessageBox::NoRole);
        QPushButton *cancleBtn=box.addButton(tr("取消"),QMessageBox::RejectRole);
        box.exec();
        if(box.clickedButton()==yesBtn)
        {
            return save();
        }
        else if(box.clickedButton()==cancleBtn)
        {
            return false;
        }
    }
    return true;
}
bool MainWindow::save()
{
    if(isUntitled)
    {
        return saveAs();
    }
    else
    {
        return saveFile(curFile);
    }
}
bool MainWindow::saveAs()
{
    QString fileName=QFileDialog::getSaveFileName(this,tr("另存为"),curFile);
    if(fileName.isEmpty())
        return  false;
    return  saveFile(fileName);
}
bool MainWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QFile::WriteOnly|QFile::Text))
    {
        QMessageBox::warning(this,tr("多文档编辑器"),tr("无法写入文件%1 /n %2").arg(fileName).arg(file.errorString()));
        return  false;
    }
    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out<<ui->textEdit->toPlainText();
    QApplication::restoreOverrideCursor();
    isUntitled=false;
    curFile=QFileInfo(fileName).canonicalFilePath();
    setWindowTitle(curFile);
    return  true;
}

void MainWindow::on_action_N_triggered()
{
    newFile();
}

void MainWindow::on_action_S_triggered()
{
    save();
}

void MainWindow::on_action_A_triggered()
{
    saveAs();
}

void MainWindow::on_action_O_triggered()
{
    if(maybeSave())
    {
        QString fileName=QFileDialog::getOpenFileName(this);
        if(!fileName.isEmpty())
        {
            loadFile(fileName);
            ui->textEdit->setVisible(true);
        }
    }
}

void MainWindow::on_action_C_triggered()
{
    if(maybeSave())
    {
        ui->textEdit->setVisible(false);
    }
}

void MainWindow::on_action_X_triggered()
{
    on_action_C_triggered();
    qApp->quit();
}

void MainWindow::on_action_Z_triggered()
{
    ui->textEdit->undo();
}

void MainWindow::on_action_X_2_triggered()
{
    ui->textEdit->cut();
}

void MainWindow::on_action_C_2_triggered()
{
    ui->textEdit->copy();
}

void MainWindow::on_action_V_triggered()
{
    ui->textEdit->paste();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(maybeSave())
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void MainWindow::showFindText()
{
    QString str=findLineEdit->text();
    if(!ui->textEdit->find(str,QTextDocument::FindBackward))
    {
        QMessageBox::warning(this,tr("查找"),tr("找不到%1").arg(str));

    }
}

void MainWindow::on_action_F_triggered()
{
    finDlg->show();
}
