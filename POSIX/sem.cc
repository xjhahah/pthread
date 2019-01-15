#include "sem.hpp"

const int num=12;

void* consume_routine(void* arg)
{
  RingQueue* q=(RingQueue*)arg;
  int data;
  for(;;)
  {
    usleep(1000);
    q->PopData(data);
    cout<<"consume has done, data is " << data << endl;
  }
}
void* product_routine(void* arg)
{
  RingQueue* q=(RingQueue*)arg;
  srand((unsigned long)time(NULL));
  for(;;)
  {
    int data=rand()%100+1;
    q->PushData(data);
    cout << "product has done, data is " << data << endl;
   sleep(1);
  }
}

//基于环形队列的生产消费模型
 
int main()
{
  RingQueue* rq=new RingQueue(num);
  pthread_t c,p;
  pthread_create(&c,NULL,consume_routine,(void*)rq);
  pthread_create(&p,NULL,product_routine,(void*)rq);

  pthread_join(c,NULL);
  pthread_join(p,NULL);
  delete (rq);
  return 0;
}
