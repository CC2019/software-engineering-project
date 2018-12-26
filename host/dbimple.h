#ifndef DBIMPLE_H
#define DBIMPLE_H
#include<QString>


class dbImple
{
public:
    dbImple();
    bool initdb();
    QString operateLog(QString str,QString op);
    //QString makeString(QString st,QString op);
    //void unpacketString(QString st,QString op);
};

#endif // DBIMPLE_H
