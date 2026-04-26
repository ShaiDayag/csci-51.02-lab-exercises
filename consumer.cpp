#include "shared.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <cstdlib>
#include <string>

std::atomic<bool> running(true);
SharedData* shmPtr = nullptr;

/**
 * Cleanup thread logic.
 * Forcefully exits the process to unblock any kernel-level semop() calls.
 */
void waitForExit() {
    std::cin.get();
    running = false;
    if (shmPtr) detachSharedMemory(shmPtr);
    std::exit(0); // Ref: https://en.cppreference.com/w/cpp/utility/program/exit
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <FPS (0 for sync)>\n";
        return 1;
    }

    int fps = std::stoi(argv[1]);
    if (fps < 0) fps = 0; 

    // link to existing IPC (initialized by Producer)
    // Ref: shared.cpp/shared.h
    int semId = getSemaphores();
    int shmId = getSharedMemory();
    if (semId == -1 || shmId == -1) {
        std::cerr << "Error: Shared memory/Semaphores not found. Is Producer running?\n";
        return 1;
    }

    shmPtr = attachSharedMemory(shmId);
    if (shmPtr == nullptr) return 1;

    // Start exit monitor
    std::thread inputThread(waitForExit);

    int lastSeqNum = 0;
    int lastFrameIdx = 0;
    int skippedFrames = 0;
    int sleep_ms = fps > 0 ? (1000 / fps) : 0;

    // Consumption Loop
    while (running) {
        if (fps == 0) {
            // Wait for Producer to signal a new frame via semop()
            // Ref: https://man7.org/linux/man-pages/man2/semop.2.html
            if (semWaitNewFrame(semId) == -1) break; 
        } else {
            // Manual timing based on user FPS
            std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ms));
        }

        if (!running) break;

        // LOCK: Secure memory while we read the frame and metadata
        semLock(semId);
        
        int seq = shmPtr->sequenceNumber;
        int total = shmPtr->totalFrames;
        int current = shmPtr->currentFrame;
        std::string frameStr(shmPtr->frame, shmPtr->frameLength);
        
        // UNLOCK: Release memory for the next Producer write
        semUnlock(semId);

        // Skip Calculation (Bonus Feature)
        if (lastSeqNum != 0) {
            // Reset skip count if the video loops
            if (current < lastFrameIdx || current == 1) {
                skippedFrames = 0; 
            }
            // If sequence jumped, calculate how many frames we missed
            if (seq > lastSeqNum + 1) {
                skippedFrames += (seq - lastSeqNum - 1);
            }
        }

        // Outputting \033c (ESC + c) resets/clears the terminal screen
        // Ref: https://man7.org/linux/man-pages/man4/console_codes.4.html
        std::cout << "\033c" << frameStr; 
        
        std::cout << "\nCurrent frame: " << current << " / " << total 
                  << " (" << skippedFrames << " frames skipped)\n";
        std::cout.flush(); 

        lastSeqNum = seq;
        lastFrameIdx = current;
    }

    inputThread.join();
    return 0;
}