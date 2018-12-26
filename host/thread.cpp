#include "thread.h"
#include <synchapi.h>
#include "control.h"
#include "tools.h"

Thread::Thread()
{

}

void Thread::run()
{
    while(1)
    {
        Sleep(1000);
        begin_refresh();
        //CheckUpdate();
    }
}
