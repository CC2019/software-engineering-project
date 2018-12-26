#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tools.h"
#include "synchapi.h"
#include "control.h"
#include <QObject>
#include <qfile.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("主机控制");
    ui->log_table->horizontalHeader()->resizeSection(0,120);
    ui->log_table->horizontalHeader()->resizeSection(1,240);
    ui->log_table->horizontalHeader()->resizeSection(2,100);
    ui->log_table->horizontalHeader()->resizeSection(3,100);
    ui->log_table->horizontalHeader()->resizeSection(4,63);
    ui->log_table->horizontalHeader()->resizeSection(5,63);
    ui->log_table->horizontalHeader()->resizeSection(6,63);
    QRegExp regExp_id("[0-9]{1,10}");
    QRegExp regExp_card_id("[0-9]{1,18}");
    ui->room_id_lineEdit->setValidator(new QRegExpValidator(regExp_id,this));
    ui->room_id_lineEdit2->setValidator(new QRegExpValidator(regExp_id,this));
    ui->card_id_lineEdit->setValidator(new QRegExpValidator(regExp_card_id,this));
    ui->room_id_lineEdit->setPlaceholderText("纯数字小于10位");
    ui->room_id_lineEdit2->setPlaceholderText("纯数字小于10位");
    ui->card_id_lineEdit->setPlaceholderText("纯数字小于18位");

    //QObject::connect(&thread1, SIGNAL(begin_refresh()), this, SLOT(fresh_table()));

    QTimer *timer = new QTimer;
    connect(timer,SIGNAL(timeout()),this,SLOT(fresh_table()));
    connect(timer,SIGNAL(timeout()),this,SLOT(control()));
    timer->start(1000);

    //QObject::connect(&thread1, SIGNAL(begin_control()), this, SLOT(control()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_seerecord_Button_clicked()
{
    this->show_status();
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_adduser_Button_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_deleteuser_Button_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_changevalue_Button_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
    if( model_flag )
    {
        ui->changemodel_Button->setText("制热");
        ui->model_label->setText("制冷");
    }
    else
    {
        ui->changemodel_Button->setText("制冷");
        ui->model_label->setText("制热");
    }
}

void MainWindow::show_status()
{
    QString log = get_status();

    QString str;
    QStringList row,clo;
    row = log.split("$");

    for(int i = 0; i < row.size()-1; i++ )
    {
        str = row[i];
        clo = str.split("&");

        ui->log_table->setRowCount(i);
        ui->log_table->insertRow(i);

        QTableWidgetItem *item0 = new QTableWidgetItem();
        QTableWidgetItem *item1 = new QTableWidgetItem();
        QTableWidgetItem *item2 = new QTableWidgetItem();
        QTableWidgetItem *item3 = new QTableWidgetItem();
        QTableWidgetItem *item4 = new QTableWidgetItem();
        QTableWidgetItem *item5 = new QTableWidgetItem();
        QTableWidgetItem *item6= new QTableWidgetItem();

        item0->setText(clo[1]);
        item1->setText(clo[2]);
        item2->setText(clo[3]);
        item3->setText(clo[4]);
        item4->setText(clo[5]);
        item5->setText(clo[6]);
        item6->setText(clo[7]);

        ui->log_table->setItem(i,0,item0);
        ui->log_table->setItem(i,1,item1);
        ui->log_table->setItem(i,2,item2);
        ui->log_table->setItem(i,3,item3);
        ui->log_table->setItem(i,4,item4);
        ui->log_table->setItem(i,5,item5);
        ui->log_table->setItem(i,6,item6);

        clo.clear();
        str.clear();
    }
}

void MainWindow::on_add_pushButton_clicked()
{
    int cur_temp,tar_temp;
    if(!model_flag)
    {
            cur_temp = 20;
            tar_temp = 26;
    }
    else
    {
        cur_temp = 30;
        tar_temp = 25;
    }
    QString info = ui->room_id_lineEdit->text() +"&"
                 + ui->card_id_lineEdit->text()+"&"
                 + QString::number(tar_temp) + "&"
                 + QString::number(cur_temp) + "&"
                 + "0" + "&"
                 + "0" + "&" +"0";
    QString success = add_user(info);
    if( success == "success" )
        QMessageBox::warning(this,tr("成功"),tr("添加用户成功！"),QMessageBox::Yes);
    else
        QMessageBox::warning(this,tr("失败"),tr("添加用户失败！"),QMessageBox::Yes);

    ui->card_id_lineEdit->clear();
    ui->room_id_lineEdit->clear();
}

void MainWindow::on_delete_pushButton_clicked()
{
    QString success = search_user(ui->room_id_lineEdit2->text());
    if( !success.isEmpty() )
    {
        delete_user(ui->room_id_lineEdit2->text());
        QMessageBox::warning(this,tr("成功"),tr("删除用户成功！"),QMessageBox::Yes);
    }
    else
        QMessageBox::warning(this,tr("失败"),tr("删除用户失败！"),QMessageBox::Yes);

    ui->room_id_lineEdit2->clear();
}

void MainWindow::on_logout_Button_clicked()
{
    //clear_request();
    this->close();
    m_dialog->show();
    this->deleteLater();
}

void MainWindow::on_changemodel_Button_clicked()
{
    if( !model_flag )
    {
        model_flag = 1;
        ui->changemodel_Button->setText("制热");
        ui->model_label->setText("制冷");
    }
    else
    {
        model_flag = 0;
        ui->changemodel_Button->setText("制冷");
        ui->model_label->setText("制热");
    }
    QMessageBox::warning(this,tr("成功"),tr("修改模式成功！"),QMessageBox::Yes);
}

void MainWindow::on_pushButton_clicked()
{
    QString info = get_log();
    bool success = print_log(info);
    if( success )
        QMessageBox::warning(this,tr("成功"),tr("打印日志表成功！"),QMessageBox::Yes);
    else
        QMessageBox::warning(this,tr("失败"),tr("打印日志表失败！"),QMessageBox::Yes);
}

void MainWindow::fresh_table()
{
    int n = ui->stackedWidget->currentIndex();
    if(n == 0)
    {
        this->show_status();
    }
}

void MainWindow::control()
{
    CheckUpdate();
}
