#include "control.h"
#include "mainwindow.h"
#include <QApplication>
#include<iostream>
#include<QDebug>
#include<QString>
#include<QStringList>
#include<time.h>
#include <windows.h>
#include <QDateTime>
#include<stdio.h>
#include "dbimple.h"
#include "tools.h"

int num=0;
bool ok=true;
dbImple imple;
struct cur{
    int id[100];
    float temp[100];//当前温度
    int lenth;
};

//关机前置所有风速为0
void SetSpeedZero()
{
    QString s=NULL;
    QString status = imple.operateLog(s,"getStatus");
    QStringList list=status.split("$");
    for(int i=0;i<list.length()-1;i++)
    {
        QString add=NULL;
        QString col;
        col=list.at(i);
        QStringList n= col.split("&");
        n.replace(5,"0");
        for(int j=1;j<n.length();j++)
        {
            add=add+n.at(j)+"&";
        }
        imple.operateLog(add,"updateStatus");
    }
}

int getMode()
{
    return model_flag;
}


//检查和更新从机状态表，处理请求，响应请求后生成日志
void CheckUpdate()
{

    struct cur c;
    QString st=imple.operateLog("","getStatus");
    QStringList list=st.split("$");
    int ID;
    c.lenth = list.length()-1;
    for(int count=0;count<list.length()-1;count++)//将current temp存入结构
    {
        QString col=list.at(count);
        QStringList n=col.split("&");
        ID=n.at(1).toInt();
        c.id[count]=ID;
        int current=n.at(4).toInt();
        c.temp[count]=(float)current;
    }
    int MODE=getMode();
    st=imple.operateLog("","getStatus");
    list=st.split("$");

    for(int i=0;i<list.length()-1;i++)
    {
        QString add=NULL;
        float amount;
        float energy;
        int speed;
        int ID;
        QString col=list.at(i);
        QStringList n=col.split("&");
        amount=n.at(7).toFloat();
        energy=n.at(6).toFloat();
        speed=n.at(5).toInt();
        ID=n.at(1).toInt();
        energy=energy+speed*0.5;
        amount=amount+speed*2.5;
        QString data=QString("%1").arg(energy);
        n.replace(6,data);
        data=QString("%1").arg(amount);
        n.replace(7,data);
        int targettemp,currenttemp;
        targettemp=n.at(3).toInt();
        currenttemp=n.at(4).toInt();
        int j,k;
        int flag = 0;
        for(j = 0; j < c.lenth && flag == 0;j++)
        {
            if(c.id[j] == ID)
                flag = 1;
        }
        k=j-1;
        if(flag == 0)
        {
            c.id[c.lenth] = ID;
            c.temp[c.lenth] = currenttemp;
            c.lenth++;
        }
        int num1;
        QString log1 = NULL;
        /*if(MODE==0)//制冷模式
        {
            if(targettemp==currenttemp)
            {
                if(speed!=0)
                {
                    speed=0;
                    n.replace(5,QString::number(speed, 10));
                    log1=QString::number(num1, 10)+"&"+n.at(2)+"&"+n.at(1)+"&"+n.at(5)+"&"+n.at(3)+"&"+n.at(4)+"&";//添加日志
                    imple.operateLog(log1,"setLog");
                }

                c.temp[k]=c.temp[k]+0.6;
                currenttemp=(int)c.temp[k];
                data=QString("%1").arg(currenttemp);
                n.replace(4,data);
            }

            if(targettemp>=currenttemp&&currenttemp <= 35)//目标温度高于当前温度,风速为0，当前温度自然升高
            {
                c.temp[k]=c.temp[k]+1;
                currenttemp=(int)c.temp[k];
                data=QString("%1").arg(currenttemp);
                n.replace(4,data);

            }
            if(targettemp<currenttemp)//目标温度低于当前温度，且风速大于0
            {
                switch(speed)
                {
                case 1:
                {

                    c.temp[k]=c.temp[k]-0.1;
                    currenttemp=(int)c.temp[k];
                    data=QString("%1").arg(currenttemp);
                    n.replace(4,data);
                    break;
                }
                case 2:
                {

                    c.temp[k]=c.temp[k]-0.3;
                    currenttemp=(int)c.temp[k];
                    data=QString("%1").arg(currenttemp);
                    n.replace(4,data);
                    break;
                }
                case 3:
                {

                    c.temp[k]=c.temp[k]-0.5;
                    currenttemp=(int)c.temp[k];
                    data=QString("%1").arg(currenttemp);
                    n.replace(4,data);
                    break;
                }
                case 0:
                {
                    //speed=1;
                    n.replace(5,QString::number(speed, 10));
                    log1=QString::number(num1, 10)+"&"+n.at(2)+"&"+n.at(1)+"&"+n.at(5)+"&"+n.at(3)+"&"+n.at(4)+"&";//添加日志
                  //  imple.operateLog(log1,"setLog");
                    /*c.temp[k]=c.temp[k]-0.1;
                    currenttemp=(int)c.temp[k];
                    data=QString("%1").arg(currenttemp);
                    n.replace(4,data);*
                    break;
                }
                break;

                }
            }

        }
        if(MODE==1)//制热模式
        {
            if(targettemp==currenttemp)
            {

                if(speed!=0)
                {
                    speed=0;
                    n.replace(5,QString::number(speed, 10));
                    log1=QString::number(num1, 10)+"&"+n.at(2)+"&"+n.at(1)+"&"+n.at(5)+"&"+n.at(3)+"&"+n.at(4)+"&";//添加日志
                    imple.operateLog(log1,"setLog");
                }
                c.temp[k]=c.temp[k]-0.1;
                currenttemp=(int)c.temp[k];
                data=QString("%1").arg(currenttemp);
                n.replace(4,data);
            }

            if(targettemp<=currenttemp&&currenttemp >= 17)//目标温度高于当前温度,风速为0，当前温度自然decrease
            {

                c.temp[k]=c.temp[k]-0.1;
                currenttemp=(int)c.temp[k];
                data=QString("%1").arg(currenttemp);
                n.replace(4,data);
            }
            if(targettemp>currenttemp)//目标温度低于当前温度，且风速大于0
            {
                switch(speed)
                {
                case 1:
                {

                    c.temp[k]=c.temp[k]+0.1;
                    currenttemp=(int)c.temp[k];
                    data=QString("%1").arg(currenttemp);
                    n.replace(4,data);
                    break;
                }
                case 2:
                {

                    c.temp[k]=c.temp[k]+0.3;
                    currenttemp=(int)c.temp[k];
                    data=QString("%1").arg(currenttemp);
                    n.replace(4,data);
                    break;
                }
                case 3:
                {

                    c.temp[k]=c.temp[k]+0.5;
                    currenttemp=(int)c.temp[k];
                    data=QString("%1").arg(currenttemp);
                    n.replace(4,data);
                    break;
                }
                case 0:
                {
                    //speed=1;
                    n.replace(5,QString::number(speed, 10));
                    log1=QString::number(num1, 10)+"&"+n.at(2)+"&"+n.at(1)+"&"+n.at(5)+"&"+n.at(3)+"&"+n.at(4)+"&";//添加日志
                   // imple.operateLog(log1,"setLog");
                    /*c.temp[k]=c.temp[k]+0.1;
                    currenttemp=(int)c.temp[k];
                    data=QString("%1").arg(currenttemp);
                    n.replace(4,data);*
                    break;
                }
                break;

                }
            }

        }*/

        for(int m=1;m<n.length();m++)
        {
            add=add+n.at(m)+"&";
        }
        imple.operateLog(add,"updateStatus");
    }
    QString req=imple.operateLog("","getRequest");
    QStringList reqlist=req.split("$");
    int len=reqlist.length()-1;
    int reallen;
    if(len<=3)
    {
        reallen=len;
    }
    else
    {
        reallen=3;
    }
        int num;
        QString slaveid;
        int tem,spe;
        for(int a=0;a<reallen;a++)
        {
            QString add1=NULL;
            QString col=reqlist.at(a);
            QStringList n=col.split("&");
            slaveid=n.at(2);
            spe=n.at(3).toInt();
            tem=n.at(4).toInt();
            QString sta=imple.operateLog(slaveid,"searchStatus");
            QStringList l=sta.split("&");
            if(MODE==0)
            {
                if(spe>=0&&spe<4&&tem<26&&tem>17)//执行请求
                {
                    if(tem>l.at(4).toInt())
                        spe=0;

                    l.replace(3,QString::number(tem, 10));
                    l.replace(5,QString::number(spe, 10));
                    for(int b=1;b<l.length();b++)
                    {
                        add1=add1+l.at(b)+"&";
                    }
                    imple.operateLog(add1,"updateStatus");
                    QString lognew=NULL;
                    lognew=QString::number(num, 10)+"&"+l.at(2)+"&"+l.at(1)+"&"+l.at(5)+"&"+l.at(3)+"&"+l.at(4)+"&"+n.at(5);//添加日志
                    num++;
                    imple.operateLog(lognew,"setLog");
                    imple.operateLog(n[1],"deleteRequest");//执行完毕删除请求

                }
                else
                    imple.operateLog(n[1],"deleteRequest");//删除请求
            }
            if(MODE==1)
            {
                if(spe>=0&&spe<4&&tem<31&&tem>25)//执行请求
                {
                    if(tem<l.at(4).toInt())
                        spe=0;

                    l.replace(3,QString::number(tem, 10));
                    l.replace(5,QString::number(spe, 10));
                    for(int b=1;b<l.length();b++)
                    {
                        add1=add1+l.at(b)+"&";
                    }
                    imple.operateLog(add1,"updateStatus");
                    QString lognew=NULL;
                    lognew=QString::number(num, 10)+"&"+l.at(2)+"&"+l.at(1)+"&"+l.at(5)+"&"+l.at(3)+"&"+l.at(4)+"&"+n.at(5);//添加日志
                    num++;
                    imple.operateLog(lognew,"setLog");
                    imple.operateLog(n[1],"deleteRequest");//执行完毕删除请求

                }
                else
                    imple.operateLog(n[1],"deleteRequest");//删除请求
            }

        }
}
