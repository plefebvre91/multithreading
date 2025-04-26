#include <thread>
#include <mutex>
#include <semaphore>

// This represents the ressource shared between process, irl it would be a file or a device
int shared_ressource = 0;

// Number of readers reading
unsigned int n_readers = 0;

// Mutex to protect the number of readers variable
std::mutex readers_mutex;

// Ressource is free at start
std::binary_semaphore sem_write{1};


void reader(void* args) {
  while(true) {
    {
      // The first reader takes the semaphore
      std::lock_guard<std::mutex> lock(readers_mutex);
      if (++n_readers == 1) sem_write.acquire();
    }

    // Reads...
    int value = shared_ressource;
    (void) value;

    {
      // The last one releases the semaphore 
      std::lock_guard<std::mutex> lock(readers_mutex);
      if (--n_readers == 0) sem_write.release();
    }
  }
}

void writer(void* args) {
  while(true) {
    sem_write.acquire();
    shared_ressource++;
    sem_write.release();
  }
}

int main(int argc, char** argv) {
  std::thread writer_thread(writer, nullptr);
  std::thread reader_thread_1(reader, nullptr);
  std::thread reader_thread_2(reader, nullptr);
  std::thread reader_thread_3(reader, nullptr);
  
  writer_thread.join();
  reader_thread_1.join();
  reader_thread_2.join();
  reader_thread_3.join(); 
}
