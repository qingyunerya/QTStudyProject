#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
class QLineEdit;
class QDialog;
class QLabel;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void newFile();
    bool loadFile(const QString &fileName);
    bool maybeSave();
    bool save();
    bool saveAs();
    bool saveFile(const QString &fileName);
protected:
    void closeEvent(QCloseEvent *event);
private slots:
    void showFindText();
    void on_action_N_triggered();

    void on_action_S_triggered();

    void on_action_A_triggered();

    void on_action_O_triggered();

    void on_action_C_triggered();

    void on_action_X_triggered();

    void on_action_Z_triggered();

    void on_action_X_2_triggered();

    void on_action_C_2_triggered();

    void on_action_V_triggered();

    void on_action_F_triggered();

private:
    bool isUntitled;
    QString curFile;
    QLineEdit *findLineEdit;
    QDialog *finDlg;
    QLabel *statusLabel;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
