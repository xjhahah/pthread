#include <iostream>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

using namespace std;

pthread_mutex_t lock;
pthread_cond_t cond;

void* r1(void* arg)
{
  string t1=(char*)arg;
  while(1)
  {
    pthread_cond_wait(&cond,&lock);  //1，释放锁   2.去等待，回来时将锁带回来
    cout << t1 <<" is runing" << endl;
  }
}
void* r2(void* arg)
{
  while(1)
  {
    pthread_cond_signal(&cond);   //唤醒在该条件变量下的线程
    sleep(2);
  }
}
//模拟实现抢答器
int main()
{
  pthread_mutex_init(&lock,NULL);
  pthread_cond_init(&cond,NULL);

  //创建线程
  pthread_t t1,t2;
  pthread_create(&t1,NULL,r1,(void*)"thread 1");
  pthread_create(&t2,NULL,r2,(void*)"thread 2");

  pthread_join(t1,NULL);
  pthread_join(t2,NULL);

  pthread_mutex_destroy(&lock);
  pthread_cond_destroy(&cond);
  return 0;
}
