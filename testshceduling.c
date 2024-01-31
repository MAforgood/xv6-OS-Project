#include "param.h"
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
#include "syscall.h"
#include "traps.h"
#include "memlayout.h"
#define SLEEP_TIME 200
void ThreadFunc1(void *args1,void *args2)
{
    sleep(SLEEP_TIME);
    exit();
}
int main()
{
    int f1 = fork();
    if (f1 == 0)
    {
        for(int i=0;i<4;i++){
            //printf(1,"in process 1\n");
            sleep(SLEEP_TIME);
            //printf(1,"%d complete\n",i);
        }
        exit();
    }
    int f2=fork();
    if(f2==0){
         int arg1 = 2, arg2 = 1;

        thread_create(&ThreadFunc1, (void *)&arg1, (void *)&arg2);
        int arg3 = 2;
        thread_create(&ThreadFunc1, (void *)&arg1, (void *)&arg3);
        int arg4 = 3;
        thread_create(&ThreadFunc1, (void *)&arg1, (void *)&arg4);
        int arg5 = 4;
        thread_create(&ThreadFunc1, (void *)&arg1, (void *)&arg5);
        thread_join();
        thread_join();
        thread_join();
        thread_join();
    }
    wait();
    wait();
    // printf("%d\n", x);
    return 0;
}
