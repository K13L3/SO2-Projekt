#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <iomanip>
#include <atomic>

#ifdef _WIN32
    #define CLEAR_SCREEN "cls"
#else
    #define CLEAR_SCREEN "clear"
#endif

// Proper semaphore implementation using mutex and condition variable
class Semaphore {
private:
    std::mutex mutex;
    std::condition_variable cv;
    int count;

public:
    Semaphore(int init_count) : count(init_count) {}

    void wait() {
        std::unique_lock<std::mutex> lock(mutex);
        cv.wait(lock, [this]() { return count > 0; });
        count--;
    }

    void signal() {
        std::unique_lock<std::mutex> lock(mutex);
        count++;
        cv.notify_one();
    }
};

const int NUM_PHILOSOPHERS = 7;
const int SIMULATION_TIME_SEC = 60; // Run for 60 seconds

std::vector<std::mutex> fork_mutexes(NUM_PHILOSOPHERS);
std::mutex state_mutex;
Semaphore table(NUM_PHILOSOPHERS - 1); // Allow N-1 philosophers at table
std::atomic<int> philosopher_states[NUM_PHILOSOPHERS];
std::atomic<bool> running{true};

// States: 0 = Waiting, 1 = Thinking, 2 = Hungry, 3 = Eating
const char* states[] = {"Waiting", "Thinking", "Hungry", "Eating"};

void display_table() {
    auto start_time = std::chrono::steady_clock::now();
    int elapsed_seconds = 0;
    
    while (running) {
        system(CLEAR_SCREEN);
        auto current_time = std::chrono::steady_clock::now();
        elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time).count();
            
        std::cout << "Dining Philosophers Simulation - Running for " << elapsed_seconds << "s (Total: " << SIMULATION_TIME_SEC << "s)" << std::endl;
        std::cout << "------------------------------------------------------" << std::endl;
        std::cout << "Philosopher ID | State        | Fork Status" << std::endl;
        std::cout << "------------------------------------------------------" << std::endl;
        
        for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
            std::cout << std::setw(14) << i << " | " << std::setw(12) << states[philosopher_states[i].load()] << " | ";
                      
            if (philosopher_states[i].load() == 3) { // If eating
                std::cout << "Has forks " << i << " and " << (i + 1) % NUM_PHILOSOPHERS;
            } else {
                std::cout << "No forks";
            }
            std::cout << std::endl;
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        
        if (elapsed_seconds >= SIMULATION_TIME_SEC) {
            running = false;
        }
    }
}

void philosopher(int id) {
    int left_fork = id;
    int right_fork = (id + 1) % NUM_PHILOSOPHERS;
    
    // Ensure lower-numbered fork is picked up first to prevent deadlock
    if (left_fork > right_fork) {
        std::swap(left_fork, right_fork);
    }
    
    while (running) {
        // Thinking
        philosopher_states[id].store(1); // Thinking
        std::this_thread::sleep_for(std::chrono::milliseconds(500 + rand() % 2000));
        
        if (!running) break;
        
        // Hungry
        philosopher_states[id].store(2); // Hungry
        
        // Enter table (limited resource)
        table.wait();
        
        if (!running) {
            table.signal(); // Make sure to release if simulation ended
            break;
        }
        
        // Try to get forks - acquire in order to prevent deadlock
        fork_mutexes[left_fork].lock();
        fork_mutexes[right_fork].lock();
        
        // Eating
        philosopher_states[id].store(3); // Eating
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 + rand() % 2000));
        
        // Release forks
        fork_mutexes[right_fork].unlock();
        fork_mutexes[left_fork].unlock();
        
        // Leave table
        table.signal();
        
        if (!running) break;
    }
    
    // Final state - waiting
    philosopher_states[id].store(0);
}

int main() {
    // Seed random number generator
    srand(static_cast<unsigned int>(time(nullptr)));
    
    // Initialize philosopher states
    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        philosopher_states[i].store(0); // Waiting
    }
    
    std::vector<std::thread> philosophers;
    std::thread table_display(display_table);
    
    // Create philosopher threads
    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        philosophers.emplace_back(philosopher, i);
    }
    
    // Wait for simulation to complete
    for (auto& ph : philosophers) {
        if (ph.joinable()) {
            ph.join();
        }
    }
    
    if (table_display.joinable()) {
        table_display.join();
    }
    
    std::cout << "\nSimulation completed.\n";
    return 0;
}