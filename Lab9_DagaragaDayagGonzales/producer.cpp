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
 * Producer.cpp holds all the code necessary to create the producer program
 * when compiled alongside shared.cpp
 */

#include "shared.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <thread> // Ref: https://en.cppreference.com/w/cpp/thread/thread
#include <chrono>
#include <cstring>
#include <atomic> // Ref: https://en.cppreference.com/w/cpp/atomic/atomic
#include <stdexcept> // Ref: https://en.cppreference.com/w/cpp/error/invalid_argument
                     // Ref: https://en.cppreference.com/w/cpp/error/out_of_range
// Atomic flag for thread-safe signaling between the input thread and main loop
std::atomic<bool> running(true);

/**
 * Background thread function to monitor for the Enter key.
 * This prevents the program from being stuck in a sleep loop.
 */
void waitForExit() {
    std::cin.get(); 
    running = false; 
}

/**
 * Producer's main role is to set up interprocess communication resources, read 
 * the whole ASCII video file, store its contents and split them by frames, then feed
 * the right order of frames to the consumer program via writing into shared memory. 
 * Frames fed to the consumer program will depend on the ASCII video file the program
 * has read, as well as the assigned frames per second.
 */
int main(int argc, char* argv[]) {
    // Step 1. Argument validation

    // Requires file name and FPS
    if (argc != 3) {
        std::cerr << "Usage: ./" << argv[0] << " <video_file.txt> <FPS>\n";
        return 1;
    }

    std::string filename = argv[1];
    // int fps = std::stoi(argv[2]);
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
    if (fps <= 0) {
        std::cerr << "Error: FPS must be a positive integer (greater than 0). Got: " << fps << "\n";
        return 1;
    }

    // Step 2. Open ASCII video file, if available

    // Ref: https://en.cppreference.com/w/cpp/io/basic_ifstream
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << "\n";
        return 1;
    }
    
    // This reads the whole file into a string buffer
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close(); // File is done being read, "video" information has been gathered.

    // Step 3. Parse frames based on the ESC 'c' sequence (\033c)

    std::vector<std::string> frames;
    std::string delim = "\033c"; 
    size_t start = content.find(delim);
    
    while (start != std::string::npos) {
        start += delim.length();
        size_t end = content.find(delim, start);
        frames.push_back(content.substr(start, end - start));
        start = end;
    }

    // Prevent empty frames vector or if file exists but contains no valid ESC-C delimited frames
    // Ref: https://en.cppreference.com/w/cpp/container/vector/operator_at
    if (frames.empty()) {
        std::cerr << "Error: No frames found in \"" << filename << "\". "
                  << "Ensure the file uses the \\033c frame delimiter format.\n";
        return 1;
    }

    std::cout << "Loaded " << frames.size() << " frames from \"" << filename << "\" at " << fps << " FPS.\n";
    std::cout << "Press Enter to stop.\n";

    // Step 4. Create and connect to IPC Resources

    // Logic defined in shared.cpp using semget() and shmget()
    int semId = getSemaphores();
    int shmId = getSharedMemory();
    if (semId == -1 || shmId == -1) return 1;

    // Attach memory segment to process address space
    // Ref: https://man7.org/linux/man-pages/man2/shmat.2.html
    SharedData* shmPtr = attachSharedMemory(shmId);
    if (shmPtr == nullptr) return 1;

    // Step 5. Launch exit listener thread

    std::thread inputThread(waitForExit);

    int currentFrameIndex = 0;
    int sleep_ms = 1000 / fps; 

    // Step 6. Start the production loop

    while (running) {
        const std::string& frame = frames[currentFrameIndex];

        // LOCK: Protect shared memory from simultaneous access
        // Ref: https://man7.org/linux/man-pages/man2/semop.2.html
        // Consumer.out can't read/edit by this point
        semLock(semId);
        
        shmPtr->totalFrames = (int)frames.size();
        shmPtr->currentFrame = currentFrameIndex + 1;
        shmPtr->producerFPS = fps;
        shmPtr->sequenceNumber++;
        
        // Warning for when frame data exceeds MAX_FRAME_SIZE
        if ((int)frame.length() >= MAX_FRAME_SIZE) {
            std::cerr << "Warning: Frame " << (currentFrameIndex + 1) << " exceeds MAX_FRAME_SIZE (" << MAX_FRAME_SIZE << " bytes) " << "and will be truncated. Consider increasing MAX_FRAME_SIZE in shared.h.\n";
        }
        
        // Copy the raw ASCII data into the shared buffer
        // Ref: https://man7.org/linux/man-pages/man3/memcpy.3.html
        int copyLen = frame.length() < MAX_FRAME_SIZE - 1 ? frame.length() : MAX_FRAME_SIZE - 1;
        shmPtr->frameLength = copyLen;
        std::memcpy(shmPtr->frame, frame.c_str(), copyLen);
        shmPtr->frame[copyLen] = '\0'; 
        
        // UNLOCK: Allow the consumer to access the data
        // Consumer.out can read/edit by this point
        semUnlock(semId);

        // SIGNAL: Wake up any consumers waiting for a new frame
        // Ref: https://man7.org/linux/man-pages/man2/semctl.2.html (SETVAL)
        // Consumer.out is now awake
        semSignalNewFrame(semId);

        // Precision sleeping that respects the 'running' flag
        auto start_time = std::chrono::steady_clock::now();
        while (running) {
            auto now = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time).count();
            if (elapsed >= sleep_ms) break;
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
        }

        currentFrameIndex = (currentFrameIndex + 1) % frames.size();
    }

    // Step 7. Teardown/Memory clean up
    
    inputThread.join();
    detachSharedMemory(shmPtr);
    removeSharedMemory(shmId); 
    removeSemaphores(semId);   

    return 0;
}