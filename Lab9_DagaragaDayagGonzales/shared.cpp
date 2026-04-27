/*
Pete Justin Dagaraga (231874)
Shaan Graal Dayag (231928)
Robynne Alexa Gonzales (232944)

April 27, 2026
*/

/*
We have not discussed the C++ language code and Bash scripting in our program with anyone other than our instructor or the teaching assistants assigned to this course.
We have not used C++ language code and Bash scripting obtained from another student, or any other unauthorized source, either modified or unmodified.
If any C++ language code and Bash scripting or documentation of either were used in our program was obtained from another source, such as a textbook or course notes, that has been clearly noted with a proper citation in the comments of our program.
*/

#include "shared.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
 
// The 4th argument needed for semctl()
// Reference: https://man7.org/linux/man-pages/man2/semctl.2.html

union semun {
    int              val;    // Value for SETVAL
    struct semid_ds* buf;    // Buffer for IPC_STAT, IPC_SET
    unsigned short* array;   // Array for GETALL, SETALL
    struct seminfo* __buf;   // Buffer for IPC_INFO (Linux-specific)
};
 
// IPC Setup ------------------------------------------------------------

// Initializes the Semaphore Set to be used by the programs
int getSemaphores() {
    /*
    Get a System V semaphore set identifier
    Reference: https://man7.org/linux/man-pages/man2/semget.2.html
    Command also exists in the slides
    */
    int semId = semget(SEM_KEY, NUM_SEMS, IPC_CREAT | IPC_EXCL | 0666);
    
    if (semId != -1) {
        // The first process to arrive will initialize the values.
        union semun arg;
        arg.val = 0;
        
        /*
        semctl(): Semaphore control operations (SETVAL)
        Reference: https://man7.org/linux/man-pages/man2/semctl.2.html
        */
        if (semctl(semId, SEM_MUTEX,     SETVAL, arg) == -1 ||
            semctl(semId, SEM_NEW_FRAME, SETVAL, arg) == -1) {
            perror("semctl SETVAL");

            // Cleans up the semaphore set created if initialization fails
            // Reference: https://man7.org/linux/man-pages/man2/semctl.2.html
            semctl(semId, 0, IPC_RMID);
            return -1;
        }
    } else if (errno == EEXIST) {
        // If the semaphore set is already created, then just join the existing set.
        semId = semget(SEM_KEY, NUM_SEMS, 0666);
        if (semId == -1) {
            perror("semget existing");
            return -1;
        }
    } else {
        perror("semget");
        return -1;
    }
    return semId;
}

// Initializes the Shared Memory Segment to be used by the programs
int getSharedMemory() {
    int shmId = shmget(SHM_KEY, sizeof(SharedData), IPC_CREAT | 0666);
    if (shmId == -1) {
        perror("shmget");
    }
    return shmId;
}

/*
This attaches the SharedData memory segment to process address space
to be shared between the producer and consumer, of which will be
accessed later.
*/
SharedData* attachSharedMemory(int shmId) {
    /*
     shmat(): Shared memory operations (attach)
     Reference: https://man7.org/linux/man-pages/man2/shmat.2.html
     */
    void* ptr = shmat(shmId, NULL, 0);
    if (ptr == (void*)-1) {
        perror("shmat");
        return NULL;
    }
    return (SharedData*)ptr;
}

int detachSharedMemory(SharedData* ptr) {
    if (ptr == NULL) return 0;
    /*
    shmdt(): Shared memory operations (detach)
    Reference: https://man7.org/linux/man-pages/man2/shmat.2.html
    */
    return shmdt(ptr);
}

int removeSharedMemory(int shmId) {
    // Prevent invalid shmId
    if (shmId == -1) return -1;
    /*
    shmctl(): Shared memory control (IPC_RMID to remove)
    Reference: https://man7.org/linux/man-pages/man2/shmctl.2.html
    */
    return shmctl(shmId, IPC_RMID, NULL);
}

int removeSemaphores(int semId) {
    // Prevent invalid semId
    if (semId == -1) return -1;
    /*
    semctl(): Semaphore control (IPC_RMID to remove)
    Reference: https://man7.org/linux/man-pages/man2/semctl.2.html
    */
    return semctl(semId, 0, IPC_RMID);
}
 
// Mutex & Signaling Logic ----------------------------------------------

void semLock(int semId) {
    struct sembuf ops[2];
    ops[0].sem_num = SEM_MUTEX;  // Use first semaphore
    ops[0].sem_op  = 0;          // Forces operation to wait if semaphore is at 0
    ops[0].sem_flg = 0;
    ops[1].sem_num = SEM_MUTEX;  // Use first semaphore
    ops[1].sem_op  = 1;          // Increments 1 to the sempahore
    ops[1].sem_flg = SEM_UNDO;   // Important: This uatomatically releases lock if process dies
 
    while (semop(semId, ops, 2) == -1) {
        if (errno == EINTR) continue; // Retry if interrupted by system signal
        if (errno == EIDRM || errno == EINVAL) return; 
        perror("semop lock");
        return;
    }
}

void semUnlock(int semId) {
    struct sembuf op;
    op.sem_num = SEM_MUTEX;     // Use the first semaphore
    op.sem_op  = -1;            // Decrement to 0
    op.sem_flg = SEM_UNDO;      // If something goes wrong, allows an undo
 
    if (semop(semId, &op, 1) == -1) {
        if (errno == EIDRM || errno == EINVAL) return;
        perror("semop unlock");
    }
}

void semSignalNewFrame(int semId) {
    union semun arg;
    arg.val = 1;
    // We use semctl SETVAL to ensure signal doesn't "stack" up (remains binary 0 or 1)
    if (semctl(semId, SEM_NEW_FRAME, SETVAL, arg) == -1) {
        if (errno == EIDRM || errno == EINVAL) return;
        perror("semctl signal new_frame");
    }
}

int semWaitNewFrame(int semId) {
    struct sembuf op;
    op.sem_num = SEM_NEW_FRAME; // Use the second semaphore
    op.sem_op  = -1; // Decrement to 0. Will block the program that called it as a result until semaphore value is brought back up again 
    op.sem_flg = 0;

    // Loop on semop in case of EINTR (interrupted by signal)
    // Exit on EIDRM (semaphore set removed) or EINVAL (invalid semaphore set)
    // Ref: https://man7.org/linux/man-pages/man2/semop.2.html
    while (semop(semId, &op, 1) == -1) {
        if (errno == EINTR) continue; // Retry on signal interruption
        return -1; // EIDRM, EINVAL, or other fatal IPC error → caller exits
    }
    return semop(semId, &op, 1);
}
