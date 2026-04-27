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

// For Lab 9: Producer-Consumer ASCII video streaming using System V IPC.

/** 
 * Consumer.cpp holds all the code necessary to create the consumer program
 * when compiled alongside shared.cpp
 */

#include "shared.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <cstdlib>
#include <string>
#include <stdexcept> // Ref: https://en.cppreference.com/w/cpp/error/invalid_argument
                     // Ref: https://en.cppreference.com/w/cpp/error/out_of_range

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

/**
 * Consumer.out's main role is to connect to the generated interprocess communication resources,
 * then wait for producer to first send information towards the shared memory. Once that occurs,
 * it then displays the information (the scene of the ASCII video) on the terminal that is 
 * running the consumer program. It will continue looping through the video until an interruption 
 * happens via pressing Enter or some other error. Frames per second will depend on what is inputted, 
 * and can be greater/lesser than the frames per second assigned to the producer program.
 */
int main(int argc, char* argv[]) {
    // Step 1. Argument validation

    // Requires FPS
    if (argc != 2) {
        std::cerr << "Usage: ./" << argv[0] << " <FPS (0 for sync)>\n";
        return 1;
    }

    int fps;

    // Wrapped stoi in try-catch to handle non-numeric or out-of-range FPS input
    // Ref: https://en.cppreference.com/w/cpp/string/basic_string/stoi
    // Ref: https://en.cppreference.com/w/cpp/error/invalid_argument
    // Ref: https://en.cppreference.com/w/cpp/error/out_of_range
    try {
        fps = std::stoi(argv[2]);
    } catch (const std::invalid_argument&) {
        std::cerr << "Error: FPS must be a valid integer. Got: " << argv[2] << "\n";
        return 1;
    } catch (const std::out_of_range&) {
        std::cerr << "Error: FPS value is out of range: " << argv[2] << "\n";
        return 1;
    }

    // Ref (division by zero / UB): https://en.cppreference.com/w/cpp/language/operator_arithmetic
    // Ref (signed overflow / UB): https://en.cppreference.com/w/cpp/language/ub
    if (fps < 0) {
        std::cerr << "Error: FPS must be 0 (sync with producer) or a positive integer. Got: " << fps << "\n";
        return 1;
    }

    // Step 2. Link to existing IPC Resources (ought to be initialized by Producer first)
    // Refer to shared.cpp/shared.h for logic

    // Semaphore set and Shared Memory segment linked here.
    int semId = getSemaphores();
    int shmId = getSharedMemory();
    if (semId == -1 || shmId == -1) {
        std::cerr << "Error: Shared memory/Semaphores not found. Is Producer running?\n";
        return 1;
    }

    // Shared Memory segment connected to local address space.
    // SharedData will be sent here for consumer to output.
    shmPtr = attachSharedMemory(shmId);
    if (shmPtr == nullptr) return 1;

    // Step 3. Start monitoring for exit
    
    std::thread inputThread(waitForExit);

    int lastSeqNum = 0;
    int lastFrameIdx = 0;
    int skippedFrames = 0;
    int sleep_ms = fps > 0 ? (1000 / fps) : 0;

    // Step 4. Start the consumption loop

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
        // Producer.out can't read/edit by this point
        semLock(semId);
        
        int seq = shmPtr->sequenceNumber;
        int total = shmPtr->totalFrames;
        int current = shmPtr->currentFrame;
        std::string frameStr(shmPtr->frame, shmPtr->frameLength);
        
        // UNLOCK: Release memory for the next Producer write
        // Producer.out can read/edit by this point
        semUnlock(semId);

        if (total == 0 || current == 0) {
            continue;
        }

        // Skip Calculation (Bonus Feature)
        if (lastSeqNum != 0) {
            // Reset skip count if the video loops
            if (current < lastFrameIdx) {
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
        
        int producerFPS = shmPtr->producerFPS;

        // Displays fps info for both producer and consumer
        if (fps == 0) {
            std::cout << "[Sync mode | Producer FPS: " << producerFPS << "]\n";
        } else {
            std::cout << "[Consumer FPS: " << fps << " | Producer FPS: " << producerFPS << "]\n";
        }

        std::cout.flush(); 

        lastSeqNum = seq;
        lastFrameIdx = current;
    }

    // Step 5. Thread linking

    inputThread.join();
    return 0;
}