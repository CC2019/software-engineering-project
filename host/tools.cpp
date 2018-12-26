#include "tools.h"


QSqlQuery m_query;
int model_flag = 1;
int run_flag = 0;
dbImple db;
Thread thread1;


bool print_log(QString str)//打印日志表信息
{
    QDateTime time = QDateTime::currentDateTime();
    QString filename = time.toString("yyyy_mm_dd_hh_mm_ss")+".txt";

    QFile *file = new QFile;
    file->setFileName(filename);

    if(!file->open(QFile::WriteOnly | QFile::Text))
    {
         qDebug() << "Could not open file for writing";
         return false;
    }

    QStringList row,clo;
    QString t;
    row = str.split("$");

    int width = 40;
    QTextStream out(file);
    out << qSetFieldWidth(width) << "id";
    out << qSetFieldWidth(width) <<"card_id";
    out << qSetFieldWidth(width) <<"slave_id";
    out << qSetFieldWidth(width) << "speed";
    out << qSetFieldWidth(width) << "target_temp";
    out << qSetFieldWidth(width) << "cur_temp";
    out << qSetFieldWidth(width) << "req_time";
    out << qSetFieldWidth(width) << "res_time";
    for(int i = 0; i < row.size(); i++ )
    {
        t = row[i];
        clo = t.split("&");
        for(int j = 1; j < clo.size(); j++ )
            out << qSetFieldWidth(width) << clo[j];
        out<<endl;
    }
    file->flush();
    file->close();
    return true;
}

QString add_user(QString str)//将新入住用户信息加入status表，成功返回true，否则返回false
{
    return db.operateLog(str,"setStatus");
}

QString delete_user(QString id)//将用户信息从status表删除，成功返回true，否则返回false
{
    return db.operateLog(id,"deleteStatus");
}

QString get_log()
{
    return db.operateLog("","getLog");
}

QString get_status()
{
    return db.operateLog("","getStatus");
}

QString delete_status()
{
    return db.operateLog("","deleteStatus");
}

QString clear_status()
{
    return db.operateLog("","clearStatus");
}

QString search_user(QString room)
{
    return db.operateLog(room,"searchStatus");
}

QString clear_request()
{
    return db.operateLog("","clearRequest");
}
