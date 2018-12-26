#include "dialog.h"
#include "ui_dialog.h"
#include "mainwindow.h"
#include <qmessagebox.h>
#include "tools.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    ui->host_password_lineEdit->setEchoMode(QLineEdit::Password);
    this->setWindowTitle("主机控制系统");
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_login_Button_clicked()
{
    if((ui->host_id_lineEdit->text().trimmed() == tr("admin")) && (ui->host_password_lineEdit->text() == tr("111111")))
    {
        MainWindow *m_mainwindow = new MainWindow;
        dbImple db;
        db.initdb();
        //clear_request();
        m_mainwindow->show_status();
        //thread1.start();
        m_mainwindow->m_dialog = this;
        this->hide();
        m_mainwindow->show();
        ui->host_id_lineEdit->clear();
        ui->host_password_lineEdit->clear();
        ui->host_id_lineEdit->setFocus();
    }
    else
    {
        QMessageBox::warning(this,tr("警告"),tr("用户名或密码错误!"),QMessageBox::Yes);
        ui->host_id_lineEdit->clear();
        ui->host_password_lineEdit->clear();
        ui->host_id_lineEdit->setFocus();
    }
}
