#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <vector>

using namespace std;

volatile int tickets = 100;   //临界资源，需要加锁，否则多个线程访问临界资源会发生问题
pthread_mutex_t lock;  //互斥锁

void* BuyTickets(void* arg)
{
  int number=(int)arg;
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
      break;
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
  int num = 3;
  vector<attr> thread_list(num);
  int i=1;
  for(;i<=thread_list.size();++i)
  {
    thread_list[i]._num = i;
    pthread_create(&(thread_list[i].tid),NULL,BuyTickets,(void*)thread_list[i]._num);
  }
  int j=1;
  while(j++ && j<=thread_list.size())
  {
    pthread_join(thread_list[j].tid,NULL);
  }
  return  0;
}
