#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>



typedef struct
{
    long type;
    pid_t pid;
} Message;



int main(int argc, char **argv) {

    // getting pid
    pid_t pid;
    printf("pid of first program: %d\n", (pid = getpid()));

    // creating message queue and message
    key_t key = ftok("./1.c", 1);
    int msqid = msgget(key, 0666 | IPC_CREAT | IPC_EXCL);

    Message msg;
    msg.type = 1;
    msg.pid = pid;

    // send pid
    msgsnd(msqid, &msg, sizeof msg.pid, 0);

    // receive msg
    msgrcv(msqid, &msg, sizeof msg.pid, 2, 0);

    // delete message queue
    msgctl(msgid, IPC_RMID, NULL);
    return 0;
}
