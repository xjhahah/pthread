#include "cp.hpp"

const int num = 6;

//临界资源  互斥与同步
void* consume_routine(void* arg)
{
  int data;
  BlockQueue* bqp=(BlockQueue*)arg;
  while(1)
  {
    bqp->PopData(data);
    cout<< "Consume done , data is: "<< data << endl;
  }
}

void* product_routine(void* arg)
{
  BlockQueue* bqp=(BlockQueue*)arg;
  srand((unsigned long)time(NULL));
  while(1)
  {
    int data=rand()%100+1;
    bqp->PushData(data);
    cout << "Product done , data is " << data << endl;
    sleep(1);
  }
}
//基于 blockqueue 的生产者与消费者模型
int main()
{
  //创建交易场所
  BlockQueue* bqp=new BlockQueue(num);

  pthread_t c,p;
  pthread_create(&c,NULL,consume_routine,(void*)bqp);
  pthread_create(&p,NULL,product_routine,(void*)bqp);

  pthread_join(c,NULL);
  pthread_join(p,NULL);

  delete(bqp);
  return 0;
}
