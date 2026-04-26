// shared.cpp — Implementation of the helpers declared in shared.h.
 
#include "shared.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
 
/**
 * union semun:
 * Reference: https://man7.org/linux/man-pages/man2/semctl.2.html
 */
#if defined(__linux__) && !defined(_SEM_SEMUN_UNDEFINED)
union semun {
    int              val;    // Value for SETVAL
    struct semid_ds* buf;    // Buffer for IPC_STAT, IPC_SET
    unsigned short* array;  // Array for GETALL, SETALL
    struct seminfo* __buf;  // Buffer for IPC_INFO (Linux-specific)
};
#endif
 
// IPC Setup
 
int getSemaphores() {
    /**
     * semget(): Get a System V semaphore set identifier
     * Reference: https://man7.org/linux/man-pages/man2/semget.2.html
     */
    int semId = semget(SEM_KEY, NUM_SEMS, IPC_CREAT | IPC_EXCL | 0666);
    
    if (semId != -1) {
        // First process to arrive initializes values.
        union semun arg;
        arg.val = 0;
        
        /**
         * semctl(): Semaphore control operations (SETVAL)
         * Reference: https://man7.org/linux/man-pages/man2/semctl.2.html
         */
        if (semctl(semId, SEM_MUTEX,     SETVAL, arg) == -1 ||
            semctl(semId, SEM_NEW_FRAME, SETVAL, arg) == -1) {
            perror("semctl SETVAL");
            return -1;
        }
    } else if (errno == EEXIST) {
        // Already created; then just join the existing set.
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
 
int getSharedMemory() {
    /**
     * shmget(): Allocate a System V shared memory segment
     * Reference: https://man7.org/linux/man-pages/man2/shmget.2.html
     */
    int shmId = shmget(SHM_KEY, sizeof(SharedData), IPC_CREAT | 0666);
    if (shmId == -1) {
        perror("shmget");
    }
    return shmId;
}
 
SharedData* attachSharedMemory(int shmId) {
    /**
     * shmat(): Shared memory operations (attach)
     * Reference: https://man7.org/linux/man-pages/man2/shmat.2.html
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
    /**
     * shmdt(): Shared memory operations (detach)
     * Reference: https://man7.org/linux/man-pages/man2/shmat.2.html
     */
    return shmdt(ptr);
}
 
int removeSharedMemory(int shmId) {
    /**
     * shmctl(): Shared memory control (IPC_RMID to remove)
     * Reference: https://man7.org/linux/man-pages/man2/shmctl.2.html
     */
    return shmctl(shmId, IPC_RMID, NULL);
}
 
int removeSemaphores(int semId) {
    /**
     * semctl(): Semaphore control (IPC_RMID to remove)
     * Reference: https://man7.org/linux/man-pages/man2/semctl.2.html
     */
    return semctl(semId, 0, IPC_RMID);
}
 
// Mutex & Signaling Logic
 
void semLock(int semId) {
    /**
     * semop(): System V semaphore operations
     * Reference: https://man7.org/linux/man-pages/man2/semop.2.html
     * We use a 2-operation atomic array: 
     * 1. Wait for value to be 0 (is free?) 
     * 2. Increment to 1 (lock it)
     */
    struct sembuf ops[2];
    ops[0].sem_num = SEM_MUTEX;
    ops[0].sem_op  = 0;
    ops[0].sem_flg = 0;
    ops[1].sem_num = SEM_MUTEX;
    ops[1].sem_op  = 1;
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
    op.sem_num = SEM_MUTEX;
    op.sem_op  = -1; // Decrement to 0
    op.sem_flg = SEM_UNDO;
 
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
    op.sem_num = SEM_NEW_FRAME;
    op.sem_op  = -1; // Block until value is > 0, then decrement to 0
    op.sem_flg = 0;
    return semop(semId, &op, 1);
}