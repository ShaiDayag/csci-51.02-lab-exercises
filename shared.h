// shared.h — Common definitions for producer and consumer.
// Lab 9: Producer-Consumer ASCII video streaming using System V IPC.
 
#ifndef SHARED_H
#define SHARED_H
 
/**
 * System V IPC Headers
 * Reference for types: https://man7.org/linux/man-pages/man7/inode.7.html
 * Reference for IPC: https://man7.org/linux/man-pages/man7/sysvipc.7.html
 */
#include <sys/types.h> // Definitions for key_t and size_t
#include <sys/ipc.h>   // Common IPC flags (IPC_CREAT, IPC_RMID)
#include <sys/sem.h>   // Semaphore set operations (semget, semctl, semop)
#include <sys/shm.h>   // Shared memory operations (shmget, shmat, shmdt)
 
// "Magic" IPC keys
// Ref: https://man7.org/linux/man-pages/man2/ftok.3.html
#define SHM_KEY 0x1A2B3C4D
#define SEM_KEY 0x5E6F7A8B
 
// Maximum size of one frame in bytes (16 KB).
#define MAX_FRAME_SIZE 16384
 
// Indices inside the semaphore set.
// Ref: https://man7.org/linux/man-pages/man2/semop.2.html (sem_num)
#define SEM_MUTEX     0   // Binary mutex for locking/unlocking shared memory access.
#define SEM_NEW_FRAME 1   // Signaling semaphore: wakes up consumer when data is ready.
#define NUM_SEMS      2
 
/**
 * SharedData: The memory layout used by both processes.
 * When shmat() is called, the returned pointer is cast to this struct.
 * Ref: https://man7.org/linux/man-pages/man2/shmat.2.html
 */
struct SharedData {
    int  totalFrames;       // Total frame count in the source file.
    int  currentFrame;      // The current frame sequence (1 to totalFrames).
    int  producerFPS;       // The FPS value provided to the producer.
    int  sequenceNumber;    // Monotonically increasing counter for skip detection.
    int  frameLength;       // Actual byte length of the 'frame' array content.
    char frame[MAX_FRAME_SIZE]; // Buffer for raw ASCII frame data.
};
 
// IPC Setup Functions
 
/**
 * getSemaphores(): Creates or accesses the semaphore set.
 * Ref: https://man7.org/linux/man-pages/man2/semget.2.html
 */
int getSemaphores();
 
/**
 * getSharedMemory(): Creates or accesses the shared memory segment.
 * Ref: https://man7.org/linux/man-pages/man2/shmget.2.html
 */
int getSharedMemory();
 
/**
 * attachSharedMemory(): Maps the shared segment into the process address space.
 * Ref: https://man7.org/linux/man-pages/man2/shmat.2.html
 */
SharedData* attachSharedMemory(int shmId);
 
/**
 * detachSharedMemory(): Unmaps the segment from the process address space.
 * Ref: https://man7.org/linux/man-pages/man2/shmat.2.html (shmdt section)
 */
int detachSharedMemory(SharedData* ptr);
 
// --- IPC Teardown (Cleanup) ---
 
/**
 * removeSharedMemory(): Marks the segment to be destroyed.
 * Ref: https://man7.org/linux/man-pages/man2/shmctl.2.html (IPC_RMID)
 */
int removeSharedMemory(int shmId);
 
/**
 * removeSemaphores(): Removes the semaphore set from the system.
 * Ref: https://man7.org/linux/man-pages/man2/semctl.2.html (IPC_RMID)
 */
int removeSemaphores(int semId);
 
// --- Synchronization Helpers ---
 
/**
 * semLock/semUnlock: Implementation of Mutex logic using semop().
 * Ref: https://man7.org/linux/man-pages/man2/semop.2.html
 */
void semLock(int semId);
void semUnlock(int semId);
 
// Signaling Logic
 
/**
 * semSignalNewFrame: Uses semctl SETVAL to notify the consumer.
 * Ref: https://man7.org/linux/man-pages/man2/semctl.2.html
 */
void semSignalNewFrame(int semId);
 
/**
 * semWaitNewFrame: Blocks the calling process until a signal is received.
 * Ref: https://man7.org/linux/man-pages/man2/semop.2.html
 */
int semWaitNewFrame(int semId);
 
#endif // SHARED_H