#include "dialog.h"
#include "mainwindow.h"
#include <QApplication>
#include <qmessagebox.h>
#include <qdebug.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Dialog w;
    w.show();

    return a.exec();
}
