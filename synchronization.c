#include <pthread.h>

typedef struct
{

} context_t;

void * producer(void * ctx)
{
  context_t context = (context_t *) ctx;
}

void * consumer(void * context)
{
  context_t context = (context_t *) ctx;
}

void use_mutex(void)
{
  pthread_mutex_t mutex;

  pthread_mutex_init(&mutex, 0);
  pthread_mutex_lock(&mutex);
  pthread_mutex_unlock(&mutex);
  pthread_mutex_trylock(&mutex);
  pthread_mutex_destroy(&mutex);
}

void use_spinlock(void)
{
  pthread_spinlock_t spinlock;

  pthread_spin_init(&spinlock);
  pthread_spin_lock(&spinlock);
  pthread_spin_unlock(&spinlock);
  pthread_spin_trylock(&spinlock);
  pthread_spin_destroy(&spinlock);
}

void use_barrier(void)
{
  pthread_barrier_t barrier;

  int count = 3;
  pthread_barrier_init(&barrier, 0, count);
  pthread_barrier_wait(&barrier);
  pthread_barrier_destroy(&barrier);
}

void use_monitor(void)
{
}

void use_semaphore(void)
{
}

int main(int argc, char ** argv)
{

}

// https://docs.oracle.com/cd/E26502_01/html/E35303/sync-110.html#scrolltoc
// https://computing.llnl.gov/tutorials/pthreads/
