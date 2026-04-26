#include "shared.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <thread> // Ref: https://en.cppreference.com/w/cpp/thread/thread
#include <chrono>
#include <cstring>
#include <atomic> // Ref: https://en.cppreference.com/w/cpp/atomic/atomic

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

int main(int argc, char* argv[]) {
    // 1. Argument validation
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <video_file.txt> <FPS>\n";
        return 1;
    }

    std::string filename = argv[1];
    int fps = std::stoi(argv[2]);

    // 2. Open ASCII video file
    // Ref: https://en.cppreference.com/w/cpp/io/basic_ifstream
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << "\n";
        return 1;
    }
    
    // Read whole file into a string buffer
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    // 3. Parse frames based on the ESC 'c' sequence (\033c)
    std::vector<std::string> frames;
    std::string delim = "\033c"; 
    size_t start = content.find(delim);
    
    while (start != std::string::npos) {
        start += delim.length();
        size_t end = content.find(delim, start);
        frames.push_back(content.substr(start, end - start));
        start = end;
    }

    // 4. Connect to IPC Resources
    // Logic defined in shared.cpp using semget() and shmget()
    int semId = getSemaphores();
    int shmId = getSharedMemory();
    if (semId == -1 || shmId == -1) return 1;

    // Attach memory segment to process address space
    // Ref: https://man7.org/linux/man-pages/man2/shmat.2.html
    SharedData* shmPtr = attachSharedMemory(shmId);
    if (shmPtr == nullptr) return 1;

    // 5. Launch exit listener thread
    std::thread inputThread(waitForExit);

    int currentFrameIndex = 0;
    int sleep_ms = 1000 / fps; 

    // 6. Production Loop
    while (running) {
        const std::string& frame = frames[currentFrameIndex];

        // LOCK: Protect shared memory from simultaneous access
        // Ref: https://man7.org/linux/man-pages/man2/semop.2.html
        semLock(semId);
        
        shmPtr->totalFrames = (int)frames.size();
        shmPtr->currentFrame = currentFrameIndex + 1;
        shmPtr->producerFPS = fps;
        shmPtr->sequenceNumber++; 
        
        // Copy the raw ASCII data into the shared buffer
        // Ref: https://man7.org/linux/man-pages/man3/memcpy.3.html
        int copyLen = frame.length() < MAX_FRAME_SIZE - 1 ? frame.length() : MAX_FRAME_SIZE - 1;
        shmPtr->frameLength = copyLen;
        std::memcpy(shmPtr->frame, frame.c_str(), copyLen);
        shmPtr->frame[copyLen] = '\0'; 
        
        // UNLOCK: Allow the consumer to access the data
        semUnlock(semId);

        // SIGNAL: Wake up any consumers waiting for a new frame
        // Ref: https://man7.org/linux/man-pages/man2/semctl.2.html (SETVAL)
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

    // 7. Teardown
    inputThread.join();
    detachSharedMemory(shmPtr);
    removeSharedMemory(shmId); 
    removeSemaphores(semId);   

    return 0;
}