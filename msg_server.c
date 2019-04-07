#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>

#define MSG_FILE "."

//消息结构
struct msg_form {
    long mtype;
    char mtext[256];
};

int main()
{
    long msqid;
    key_t key;
    struct msg_form msg;

    //获取key值
    if((key = ftok(MSG_FILE,1)) < 0)
    {
        perror("ftok error");
        exit(1);
    }

    //打印key值
    printf("Message Queue - Server key is: %d.\n",key);

    //创建消息对列
    if((msqid = msgget(key,IPC_CREAT|0777)) == -1)
    {
        perror("msgget error");
        exit(1);
    }

    //打印消息队列ID及进程ID
    printf("My msqid is: %ld.\n",msqid);
    printf("My pid is: %d.\n",getpid());
    int i = 0;
    //循环读取消息
    for(;;)
    {
        printf("begin receiver msg.mtype is 888\n");
        msgrcv(msqid,&msg,256,888,0); //返回类型为888的第一个消息
        printf("end receiver msg.mtype is 888\n");

        printf("Server: receive msg.mtext is: %s.\n",msg.mtext);
        printf("Server: receive msg.mtype is: %ld.\n",msg.mtype);

        msg.mtype = 999; //客户端接收的消息类型nnn
        sprintf(msg.mtext,"hello,I'm server %d",getpid());

        printf("begin receiver msg.mtype is 999\n");
        msgsnd(msqid,&msg,sizeof(msg.mtext),0);
        printf("end receiver msg.mtype is 999\n");
    }
    return 0;
}
