#ifndef THREAD_H
#define THREAD_H

#include<QThread>
#include<QString>

class Thread : public QThread
{
    Q_OBJECT
public:
    Thread();
    virtual void run();
    void stop();
signals:

    void begin_refresh();
    void begin_control();
    void do_nothing();
private:

};

#endif // THREAD_H
