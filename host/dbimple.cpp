#include "dbimple.h"
#include <QApplication>
#include<QtSql/QtSql>
#include<QSqlQuery>
#include<QSqlDatabase>
#include<QStringList>

dbImple::dbImple()
{

}

bool dbImple::initdb()
{
    //connect database
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("api.mywsq.cn");
    db.setPort(3306);
    db.setDatabaseName("yyn");
    db.setUserName("bupt");
    db.setPassword("bupt");
    bool ok = db.open();
    if (ok)
    {
        printf("hello");
        qDebug() << QString("open successfully!");
    }
    else
    {
        qDebug() << "open failed!" << db.lastError();
    }
}

QString dbImple::operateLog(QString str,QString op)
{
    qDebug() << str;

    //initdb();

    QSqlQuery query;

    //according op to deal with my QString
    //begin with & each row value is splited with & ;
    //each line is splited by $

    if(op == "getStatus" )
    {
        QString statusInfo;
        query.exec("select * from status");
        while (query.next()) {
            for(int i = 0;i<=6;i++)
            {
                statusInfo = statusInfo + "&" + query.value(i).toString();
            }
            statusInfo = statusInfo + "$";
        }
        qDebug() <<statusInfo;
        return statusInfo;
    }
    else if(op == "setStatus")   //st should be splited by &, insert int status
    {
        QStringList list;
        list = str.split("&");
        qDebug() << list;
        query.prepare("INSERT INTO status (id, card_id, target_temp, cur_temp, speed, energy, amount)"
                      "VALUES (?,?,?,?,?,?,?)");

        int id = list.at(0).toInt();
        qDebug()<<id;
        int target_temp = list.at(2).toInt();
        int cur_temp = list.at(3).toInt();
        int speed = list.at(4).toInt();
        float energy = list.at(5).toFloat();
        float amount = list.at(6).toFloat();
        query.addBindValue(id);
        query.addBindValue(list.at(1));
        query.addBindValue(target_temp);
        query.addBindValue(cur_temp);
        query.addBindValue(speed);
        query.addBindValue(energy);
        query.addBindValue(amount);

        if(query.exec())
            return "success";
        else
            qDebug()<<query.lastError();
            return "false";
     }
    else if(op == "setLog")   //st should be splited by &, insert int log
    {
        QStringList list;
        list = str.split("&");
        qDebug() << list;
        query.prepare("INSERT INTO log (card_id, slave_id, speed, target_temp, cur_temp)"
                      "VALUES (?,?,?,?,?)");

        //int id = list.at(0).toInt();
        int slave_id = list.at(2).toInt();
        int speed = list.at(3).toInt();
        int target_temp = list.at(4).toInt();
        int cur_temp = list.at(5).toInt();
        //query.addBindValue(id);
        query.addBindValue(list.at(1));
        query.addBindValue(slave_id);
        query.addBindValue(speed);
        query.addBindValue(target_temp);
        query.addBindValue(cur_temp);
        //query.addBindValue(list.at(6));
        //query.addBindValue(list.at(7));

        if(query.exec())
            return "success";
        else
            return "false";
     }
    else if(op == "setRequest")   //st should be splited by &, insert int request
    {
        QStringList list;
        list = str.split("&");
        qDebug() << list;
        query.prepare("INSERT INTO log (slave_id, speed, temp, time)"
                      "VALUES (?,?,?,?)");
        //int id = list.at(0).toInt();
        int slave_id = list.at(1).toInt();
        int speed = list.at(2).toInt();
        int temp = list.at(3).toInt();
        //query.addBindValue(id);
        query.addBindValue(slave_id);
        query.addBindValue(speed);
        query.addBindValue(temp);
        query.addBindValue(list.at(4));

        if(query.exec())
            return "success";
        else
            return "false";
     }
    else if(op == "getRequest" )
    {
        QString requestInfo;
        query.exec("select * from request");
        while (query.next()) {
            for(int i = 0;i<=4;i++)
            {
                requestInfo = requestInfo + "&" + query.value(i).toString();
            }
            requestInfo = requestInfo + "$";
        }
        qDebug() <<requestInfo;
        return requestInfo;
    }
    else if(op == "getLog" )
    {
        QString requestInfo;
        query.exec("select * from log");
        while (query.next()) {
            for(int i = 0;i<=7;i++)
            {
                requestInfo = requestInfo + "&" + query.value(i).toString();
            }
            requestInfo = requestInfo + "$";
        }
        qDebug() <<requestInfo;
        return requestInfo;
    }
    else if(op == "getStatus" )
    {
        QString requestInfo;
        query.exec("select * from status");
        while (query.next()) {
            for(int i = 0;i<=6;i++)
            {
                requestInfo = requestInfo + "&" + query.value(i).toString();
            }
            requestInfo = requestInfo + "$";
        }
        qDebug() <<requestInfo;
        return requestInfo;
    }
    else if(op == "updateStatus")  //update where id = id, you need to send me a complete line!!!
    {
        QStringList list;
        list = str.split("&");
        qDebug()<<list;
        int id = list.at(0).toInt();
        QString speed = list.at(4);

        //update speed
        query.prepare("update status set speed = :speed where id = :id");
        query.bindValue(":speed",speed);
        query.bindValue(":id", id);
        query.exec();

        QString card_id = list.at(1);
        query.prepare("update status set card_id = :card_id where id = :id");
        query.bindValue(":card_id",card_id);
        query.bindValue(":id", id);
        query.exec();

        int target_temp = list.at(2).toInt();
        query.prepare("update status set target_temp = :target_temp where id = :id");
        query.bindValue(":target_temp",target_temp);
        query.bindValue(":id", id);
        query.exec();

        int cur_temp = list.at(3).toInt();
        query.prepare("update status set cur_temp = :cur_temp where id = :id");
        query.bindValue(":cur_temp",cur_temp);
        query.bindValue(":id", id);
        query.exec();

        float energy = list.at(5).toFloat();
        query.prepare("update status set energy = :energy where id = :id");
        query.bindValue(":energy",energy);
        query.bindValue(":id", id);
        query.exec();

        float amount = list.at(6).toFloat();
        query.prepare("update status set amount = :amount where id = :id");
        query.bindValue(":amount",amount);
        query.bindValue(":id", id);

        if(query.exec())
            return "success";
        else
            return "false";
    }
    else if (op == "deleteStatus")
    {
        int id =  str.toInt();
        query.prepare("delete from status where id = :id");
        query.bindValue(":id",id);

        if(query.exec())
            return "success";
        else
            return "false";

    }
    else if(op == "clearStatus")
    {
        if(query.exec("delete from status"))
            return "success";
        else
            return "false";
    }
    else if(op == "clearRequest")
    {
        if(query.exec("delete from request"))
            return "success";
        else
            return "false";
    }
    else if(op == "searchStatus")
    {
        int id = str.toInt();
        qDebug()<<id;

        QString statusInfo;
        query.prepare("select * from status where id = :id");
        query.bindValue(":id",id);
        query.exec();
        while (query.next()) {
            for(int i = 0;i<=6;i++)
            {
                statusInfo = statusInfo + "&" + query.value(i).toString();
            }
        }
        qDebug() <<statusInfo;

        return statusInfo;
    }
    else if(op == "deleteRequest")  //I delete basing on primary key
    {
        QStringList list = str.split("&");
        int id = list.at(0).toInt();
        query.prepare("delete from request where id = :id");
        query.bindValue(":id",id);

        if(query.exec())
            return "success";
        else
            return "false";

    }

    else{
         return "false";
    }
}



