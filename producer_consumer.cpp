#include <ctime>
#include <thread>
#include <mutex>
#include <semaphore>
#include <array>

// Fixed-size fuffer
constexpr int SIZE = 50;
std::array<int, SIZE> shared_buffer;

// Index of the last produced value
int current_index = 0;

// Mutex to protect current_index
std::mutex buffer_mutex;


// Initial value of production semphore is equal to the buffer size (empty at start)
std::counting_semaphore<SIZE> sem_production{SIZE};

// Initial value of consuming is 0 (nothing in the buffer)
std::counting_semaphore<SIZE> sem_consumption{0};


void producer(void* args) {
  while(true) {

    // Sleeps if it cannot produce
    sem_production.acquire();
    {
      // Lock and updates the buffer
      std::lock_guard<std::mutex> lock(buffer_mutex);
      shared_buffer[current_index++] = std::rand() % 100;
    }

    // Wakes the consumer up
    sem_consumption.release();
  }
}

void consumer(void* args) {
  while(true) {

    // Sleeps if it cannot consume
    sem_consumption.acquire();
    {
      // Locks and updates the buffer
      std::lock_guard<std::mutex> lock(buffer_mutex);
      int value = shared_buffer[--current_index];

      // Do something with value... 
      (void) value;
    }

    // Wakes the producer up
    sem_production.release();
  }
}

int main(int argc, char** argv) {
  std::srand(std::time({}));
  
  std::thread producer_thread(producer, nullptr);
  std::thread consumer_thread(consumer, nullptr);
  
  consumer_thread.join();
  producer_thread.join(); 
}
