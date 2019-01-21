#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <vector>

using namespace std;

volatile int tickets = 100;   //临界资源，需要加锁，否则多个线程访问临界资源会发生问题
pthread_mutex_t lock;  //互斥锁

void* BuyTickets(void* arg)
{
  int* id=(int*)arg;
  int number = *id;
  while(1)
  {
    usleep(100);
    pthread_mutex_lock(&lock);
    if(tickets > 0)
    {
      usleep(10000);
      cout<<" thread "<< number << " get a ticket: "<< tickets <<endl;
      tickets--;
      pthread_mutex_unlock(&lock);
    }
    else{
      pthread_mutex_unlock(&lock);
      cout<<"tickets is full..."<<endl;
      return (void*)0;
    }
  }
  return (void*)0;
}
struct attr{
  int _num;
  pthread_t tid;
};
int main()
{
  int num = 5;
  vector<attr> thread_list(num);
  pthread_mutex_init(&lock,NULL);

  for(int i=0;i<num;++i)
  {
    pthread_create(&(thread_list[i].tid),NULL,BuyTickets,(void*)&i);
    thread_list[i]._num = i;
  }
  for(int i=0;i<num;++i)
  {
    pthread_join(thread_list[i].tid,NULL);
  }
  pthread_mutex_destroy(&lock);
  return  0;
}
