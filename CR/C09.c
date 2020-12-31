#include <stdio.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <time.h>
#include <errno.h>

// end by Ctrl+C
// no more processes
// max count of persons is undefined
// exactly CAR_CAPACITY passengers in the car

int car(void);
int person(int person_no);

enum { CAR_CAPACITY = 5 };

int fd_get_in[2], fd_get_out[2], fd_count[2], fd_message_sync[2], fd_last_in[2], fd_last_out[2];

void remove_zombie(int signo)
{
    wait(0);
}

int main(void)
{
    srand(time(0));
    signal(SIGCHLD, remove_zombie);


    // pipe for possibility of getting in the car flag
    int err = pipe(fd_get_in);
    if (err != 0) {
        perror("Error creating pipe for possibility of getting in the car");
        return err;
    }

    // pipe for possibility of getting out of the car flag
    err = pipe(fd_get_out);
    if (err != 0) {
        perror("Error creating pipe for possibility of getting out the car");
        return err;
    }

    // pipe for count of persons
    err = pipe(fd_count);
    if (err != 0) {
        perror("Error creating pipe for count of persons");
        return err;
    }

    int count = 0;
    write(fd_count[1], &count, sizeof count);

    // pipe for lets go (to print the "i am in the trip!" message while
    // persons are actually in the trip)
    err = pipe(fd_message_sync);
    if (err != 0) {
        perror("Error creating pipe for the message sync");
        return err;
    }

    // pipe for last_passenger_in flag
    err = pipe(fd_last_in);
    if (err != 0) {
        perror("Error creating pipe for last_passenger_in");
        return err;
    }

    // pipe for last_passenger_out flag
    err = pipe(fd_last_out);
    if (err != 0) {
        perror("Error creating pipe for last_passenger_out");
        return err;
    }


    if (fork() == 0) {
        return car();
    }

    int person_no = 0;
    while (1) {
        sleep(rand() * (5.0 / RAND_MAX));
        if (person_no == INT_MAX) {
            break;
        }
        ++ person_no;
        if (fork() == 0) {
            return person(person_no);
        }
    }
    return 0;
}

int car(void)
{
    #define MESSAGE(str) fprintf(stderr, "Car: " str "\n")

    MESSAGE("1. Hi!");

    while (1) {
        MESSAGE("2. Please load the car");
        //1. allow passengers to seat and wait while the last passenger will wake up me

        char possibility_in = '1';
        for (int i = 0; i < CAR_CAPACITY; ++i) {
            write(fd_get_in[1], &possibility_in, sizeof possibility_in);
        }

        char last_passenger_in;
        read(fd_last_in[0], &last_passenger_in, sizeof last_passenger_in);


        MESSAGE("3. Go!");
        char lets_go = '1';
        for (int i = 0; i < CAR_CAPACITY; ++i) {
            write(fd_message_sync[1], &lets_go, sizeof lets_go);
        }
        sleep(5);

        MESSAGE("4. Trip ends");
        //2. allow passengers to go out and wait while the last passenger will wake up me

        char possibility_out = '1';
        for (int i = 0; i < CAR_CAPACITY; ++i) {
            write(fd_get_out[1], &possibility_out, sizeof possibility_out);
        }

        char last_passenger_out;
        read(fd_last_out[0], &last_passenger_out, sizeof last_passenger_out);
    }
    return 0;

    #undef MESSAGE
}

int person(int person_no)
{
    #define MESSAGE(str) fprintf(stderr, "Person %02d: " str "\n", person_no)

    MESSAGE("1. Hi!");
    //1. wait while there is possibility to seat into the car
    char possibility_in;
    read(fd_get_in[0], &possibility_in, sizeof possibility_in);

    //2. occupy the seat in the car (+ wake up the car if I occupy the last seat)
    MESSAGE("2. I'm sitting into the car");
    sleep(1);

    int count;
    read(fd_count[0], &count, sizeof count);
    count += 1;
    write(fd_count[1], &count, sizeof count);

    char last_passenger_in = '1';
    if (count == CAR_CAPACITY) {
        write(fd_last_in[1], &last_passenger_in, sizeof last_passenger_in);
    }

    // for message sync - waiting while the car is not full
    char lets_go;
    read(fd_message_sync[0], &lets_go, sizeof lets_go);

    MESSAGE("3. I'm in trip!");
    //3. wait while there is possibility to leave the car (+ wake up the car if I leave the last seat)
    char possibility_out;
    read(fd_get_out[0], &possibility_out, sizeof possibility_out);

    read(fd_count[0], &count, sizeof count);
    count -= 1;
    write(fd_count[1], &count, sizeof count);

    char last_passenger_out = '1';
    if (count == 0) {
        write(fd_last_out[1], &last_passenger_out, sizeof last_passenger_out);
    }

    MESSAGE("4. Thanks for the trip. Bye!");

    return 0;

    #undef MESSAGE
}