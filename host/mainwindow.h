#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "thread.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    QDialog * m_dialog;
    ~MainWindow();
    void show_status();

private slots:
    void on_seerecord_Button_clicked();

    void on_adduser_Button_clicked();

    void on_deleteuser_Button_clicked();

    void on_changevalue_Button_clicked();


    void on_add_pushButton_clicked();

    void on_delete_pushButton_clicked();

    void on_logout_Button_clicked();

    void on_changemodel_Button_clicked();

    void on_pushButton_clicked();

public slots:
    void fresh_table();

    void control();

private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
