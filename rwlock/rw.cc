#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

pthread_rwlock_t rwlock;
int book = 0;

void* Reader(void* arg)
{
  while(1)
  {
    pthread_rwlock_rdlock(&rwlock);
    cout<<"read book..."<<book<<endl;
    pthread_rwlock_unlock(&rwlock);
    usleep(300000);
  }
}
void* Writer(void* arg)
{
  while(1)
  {
    pthread_rwlock_wrlock(&rwlock);
    book++;
    pthread_rwlock_unlock(&rwlock);
    cout<<"write book..."<<book<<endl;
    sleep(1);
  }
}
int main()
{
  pthread_rwlock_init(&rwlock,NULL);
  pthread_t r,w;

  pthread_create(&r,NULL,Reader,NULL);
  pthread_create(&w,NULL,Writer,NULL);

  pthread_join(r,NULL);
  pthread_join(w,NULL);

  pthread_rwlock_destroy(&rwlock);
  return 0;
}
