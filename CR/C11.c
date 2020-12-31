// end by Ctrl+C
// no more processes
// max count of clients is undefined (but fits into unsigned short)
// queue to each seller
// no queue to choosing the seller
// clients may be executed in independent processes

// seller don't handle the clients queue

// program can't increase zombies continuosly

// more parallelism


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <unistd.h>
#include <signal.h>

#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/wait.h>

void sigint_hndlr(int s);

int initializer(void);
int finalizer(void);
int sellers(void);
int seller(int seller_no);
int client(void);

int default_runner(void);


enum { SELLERS_COUNT = 5 };
enum { ROWS_COUNT = 10 };
enum { ROW_LENGTH = 40 };



typedef struct
{
    long type;
    pid_t pid;
} Message;

// SIGINT HANDLER
void sigint_hndlr(int s) {
    finalizer();
    signal(SIGINT, SIG_DFL);
    raise(SIGINT);
}


int main(int argc, char **argv)
{

    signal(SIGINT, sigint_hndlr);

    if (strcmp(argv[0], "sellers") == 0) {
        return sellers();
    } else if (strcmp(argv[0], "client") == 0) {
        return client();
    } else if (strcmp(argv[0], "initializer") == 0) {
        return initializer();
    } else if (strcmp(argv[0], "finalizer") == 0) {
        return finalizer();
    } else {
        return default_runner();
    }
}

int default_runner(void)
{
    srand(time(0));

    int r;
    if ((r = initializer()) != 0) {
        return r;
    }

    if (fork() == 0) {
        return sellers();
    }

    while (1) {
        sleep(rand() * (1.0 / RAND_MAX) + 3);
        if (fork() == 0) {
            return client();
        }
        wait(NULL);
    }
    return 0;
}

int initializer(void)
{
    // create IPC resources
    key_t key = ftok("./C11.c", 1);

    // shared memory for cinema hall
    size_t size = ROWS_COUNT * ROW_LENGTH * sizeof(char);
    int shmid = shmget(key, size, 0666 | IPC_CREAT | IPC_EXCL);
    char *hall = shmat(shmid, NULL, 0);
    for (int i = 0; i < ROWS_COUNT * ROW_LENGTH; ++i) {
        hall[i] = 0;
    }
    shmdt(hall);

    /* semaphores (5 arrays with 5 semaphores in each):

        first array - sync of interaction between the first customer in the
        queue and the seller

        second array - sync of client interaction within one queue

        third, forth and fifth - messages sync

    */
    int semid = semget(key, 5 * SELLERS_COUNT, 0666 | IPC_CREAT | IPC_EXCL);
    unsigned short semaph[5 * SELLERS_COUNT] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                                0, 0, 0, 0, 0};
    semctl(semid, 0, SETALL, semaph);

    return 0;
}

int finalizer(void)
{
    // remove IPC resources
    key_t key = ftok("./C11.c", 1);
    int shmid = shmget(key, ROWS_COUNT * ROW_LENGTH * sizeof(char), 0666);
    int semid = semget(key, 5 * SELLERS_COUNT, 0666);

    shmctl(shmid, IPC_RMID, NULL);
    semctl(semid, 0, IPC_RMID, NULL);

    return 0;
}

int sellers(void)
{
    for (int i = 0; i < SELLERS_COUNT; ++i) {
        if (fork() == 0) {
            return seller(i);
        }
    }
    while (wait(0) != -1);
    return 0;
}

int seller(int seller_no)
{
    #define MESSAGE(str) fprintf(stderr, "Seller %d: " str "\n", seller_no)

    key_t key = ftok("./C11.c", 1);
    int semid = semget(key, 5 * SELLERS_COUNT, 0666);

    while (1) {
        MESSAGE("1. Please come in");
        //1. wait for a new client
        struct sembuf sop;
        sop.sem_num = seller_no;
        sop.sem_op = -1;
        sop.sem_flg = 0;
        semop(semid, &sop, 1);

        //2. client chooses his seat himself
        MESSAGE("2. Hello a new client; choose your place");

        sop.sem_num = seller_no + 3 * SELLERS_COUNT;
        sop.sem_op = 1;
        semop(semid, &sop, 1);
        // sleep until client chooses his seat
        sop.sem_num = seller_no + 2 * SELLERS_COUNT;
        sop.sem_op = -1;
        semop(semid, &sop, 1);

        // 3. get the ticket
        MESSAGE("3. Here is your ticket. Good bye");
        sop.sem_num = seller_no + 4 * SELLERS_COUNT;
        sop.sem_op = 1;
        semop(semid, &sop, 1);
    }
    return 0;

    #undef MESSAGE
}

int client(void)
{
    // access to ipc recources
    key_t key = ftok("./C11.c", 1);
    int shmid = shmget(key, ROWS_COUNT * ROW_LENGTH * sizeof(char), 0666);
    int semid = semget(key, 5 * SELLERS_COUNT, 0666);

    pid_t pid = getpid();

    #define MESSAGE(str,...) fprintf(stderr, "Client %d: " str "\n", pid, ##__VA_ARGS__)

    srand(time(0));

    MESSAGE("1. Hi!");

    // 1. choose the seller (may be random)
    int seller_no = rand() * ((double) SELLERS_COUNT / RAND_MAX);
    MESSAGE("2. I choose the seller number %d", seller_no);

    // 2. put self into the queue

    // semaphores for queues
    struct sembuf sop;
    sop.sem_num = seller_no + SELLERS_COUNT;
    sop.sem_op = 1;
    sop.sem_flg = 0;
    semop(semid, &sop, 1);
    int num_in_queue = semctl(semid, seller_no + SELLERS_COUNT, GETVAL);

    MESSAGE("2b. My uniq number in queue = %d", num_in_queue);

    MESSAGE("3. I sleep until my turn"); // I will be waken up by previous client in my queue
    sop.sem_op = -num_in_queue;
    semop(semid, &sop, 1);


    MESSAGE("4. Hello the seller!");
    // 3. wake the seller
    sop.sem_num = seller_no;
    sop.sem_op = 1;
    semop(semid, &sop, 1);

    sop.sem_num = seller_no + 3 * SELLERS_COUNT;
    sop.sem_op = -1;
    semop(semid, &sop, 1);

    MESSAGE("5a. I'm choosing the seat");
    // 4. choose the place

    int my_row, my_pos;
    char *hall = shmat(shmid, NULL, 0);
    int i;
    for (i = 0; i < ROWS_COUNT * ROW_LENGTH; ++i) {
        if (hall[i] == 0) {
            hall[i] = 1;
            break;
        }
    }
    shmdt(hall);
    my_row = i / ROWS_COUNT;
    my_pos = i % ROWS_COUNT;

    MESSAGE("5b. I've chosen the seat %d %d", my_row, my_pos);
    sop.sem_num = seller_no + 2 * SELLERS_COUNT;
    sop.sem_op = 1;
    semop(semid, &sop, 1);

    sop.sem_num = seller_no + 4 * SELLERS_COUNT;
    sop.sem_op = -1;
    semop(semid, &sop, 1);


    MESSAGE("6. Thank you! Bye!");

    // 5. wake up the following client from the queue
    sop.sem_num = seller_no + SELLERS_COUNT;
    sop.sem_op = num_in_queue;
    semop(semid, &sop, 1);

    return 0;

    #undef MESSAGE
}