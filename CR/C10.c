// end by Ctrl+C
// no more processes
// max count of patients is undefined
// don't change the protocol of getting to the queue

// doctor can talk with only one patient
// doctor mustn't handle the requests "who is the latest"
// doctor mustn't handle the patients queue

// almost all time patients sleep in queue and eventually:
//      - check if the previous patient is existed
//      - answer to requests "who is the latest"

// please don't use arrays of pipes
// don't use pipes for long-time storing the information
// (but you can use pipe for transferring
//  the complex information between processes)

// don't use active waiting

// don't increase zombies
// freezing by Ctrl+Z and the following "fg" command mustn't cause the deadlock

#include <stdio.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <time.h>
#include <errno.h>

int doctor(void);
int patient(int patient_no);

int main(void)
{
    srand(time(0));

    if (fork() == 0) {
        return doctor();
    }

    int patient_no = 0;
    while (1) {
        sleep(rand() * (10.0 / RAND_MAX) + 3);
        if (patient_no == INT_MAX) {
            break;
        }
        ++ patient_no;
        if (fork() == 0) {
            return patient(patient_no);
        }
    }
    return 0;
}

#define MESSAGE(str) fprintf(stderr, "Doctor: " str "\n")
int doctor(void)
{

    while (1) {
        MESSAGE("1. Please come in");
        // 1. sleep until a patient will come

        // 2. talk with the patient
        MESSAGE("2. Hello a new patient, my good patient");
        sleep(rand() * (7.0 / RAND_MAX) + 3);

        MESSAGE("3. Good bye");
    }
    return 0;
}
#undef MESSAGE

#define MESSAGE(str) fprintf(stderr, "Patient %02d [%06d]: " str "\n", patient_no, pid)

int patient(int patient_n)
{
    patient_no = patient_n;

    MESSAGE("1. Hi!");

    // Only one patient may do steps 1 and 2 simultaneously

    // 1. Broadcast the request, who is the latest patient?

    MESSAGE("2. Who is the latest?");

    // 2. wait for the answer for a 5 second (check the answer each second)

    //!!! THE LATEST PROCESS MUST INVOKE MESSAGE("2b. I'm the latest");

    // Now you have PID of the latest patient process ("previous patient")
    // or info that you are the first patient


    MESSAGE("3. I'm waiting for my turn");
    // 3. wait until we become the first patient to the doctor
    //  (i.e. check each second if the previous person process is existed)


    MESSAGE("4. I'm the next to the doctor");
    // 4. wait until doctor will become free


    MESSAGE("5. Hi doctor!");
    // 5. wake up the doctor and exit my process

    return 0;
}
#undef MESSAGE