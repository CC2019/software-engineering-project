#ifndef TOOLS_H
#define TOOLS_H
#include <QSqlQuery>
#include <qmessagebox.h>
#include <qdebug.h>
#include <qwidget.h>
#include <qregexp.h>
#include <qfile.h>
#include <qdatetime.h>
#include <dialog.h>
#include "dbimple.h"
#include <qtimer.h>
#include <qthread.h>

extern int model_flag;//模式，0制冷，1制热
extern Thread thread1;

QString add_user(QString str);//将新入住用户信息加入log表，成功返回true，否则返回false
QString delete_user(QString id);//将用户信息从log表删除，成功返回true，否则返回false
QString get_status();
QString get_log();
QString delete_status();
QString clear_status();
QString clear_request();
QString search_user(QString room);
bool print_log(QString str);//打印日志表信息

/*#define adduser(str)
        \dbImple::operateLog(str,"setStatus");
#define deleteuser(str)
        \dbImple::operateLog(str,"deleteStatus");
#define getlog()
        \dbImple::operateLog("","getLog");
#define getstatus()
        \dbImple::operateLog("","getStatus");
#define deletestatus()
        \dbImple::operateLog("","deleteStatus");*/

#endif // TOOLS_H
