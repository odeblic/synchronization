#include <unistd.h>  // sleep()
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
 
int kbhit(void)
{
  struct termios oldt, newt;
  int ch;
  int oldf;
 
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
 
  ch = getchar();
 
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);
 
  if(ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }
 
  return 0;
}


int getkey(void)
{
  while(1)
  {
    if(kbhit())
    {
      int key = getchar();
      //printf("got key '%d'\n", key);
      return key;
    }
  }
}

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

char stuff[6] = "*****";

int check_stuff()
{
  for(int i=0;i<5;++i)
  {
    if(stuff[i] != '*')
    {
      return 0;
    }
  }
  
  return 1;
}

void set_stuff()
{
  for(int i=0;i<5;++i)
  {
    stuff[i] = '#';
  }
}

void reset_stuff()
{
  for(int i=0;i<5;++i)
  {
    stuff[i] = '*';
  }
}

struct data
{
  pthread_t th;
  int id;
};

void * thread(void * arg)
{
  struct data * parameters = (struct data *)arg;

  char character = 'A' + parameters->id;
  int shift = 1 + parameters->id;

  while(1)
  {
    /*
    pthread_mutex_lock(&mutex);
    printf("[%c]", character);
    for(int i=0;i<shift;++i){putchar(' ');putchar(' ');}
    puts("|");
    pthread_mutex_unlock(&mutex);
    sleep(1);
    */
    
    pthread_mutex_lock(&mutex);
    
    pthread_cond_wait(&cond, &mutex);
    if(check_stuff())
    printf("[%c] got the condition signal (success) !\n", character);
    else
    printf("[%c] got the condition signal (failure) !\n", character),
    puts("##########################################");
    
    pthread_mutex_unlock(&mutex);
  }

  return 0;
}

int main()
{
  struct data tab[5];

  for(int i=0; i<5; ++i)
  {
    tab[i].id = i;
    pthread_create(&tab[i].th, 0, thread, &tab[i]);
  }

  while(1)
  {
    pthread_mutex_lock(&mutex);
  
    switch(getkey())
    {
      case 'S':
      case 's':
      set_stuff();
      pthread_cond_signal(&cond);
      break;

      case 'B':
      case 'b':
      set_stuff();
      pthread_cond_broadcast(&cond);
      break;
    }
    
    pthread_mutex_unlock(&mutex);
    
    reset_stuff();
  }

  sleep(10);

  pthread_exit(NULL);

  exit(EXIT_SUCCESS);
}

