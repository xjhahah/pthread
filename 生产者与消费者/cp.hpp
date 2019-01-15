#ifndef __CP__HPP__
#define __CP__HPP__

#include <iostream>
#include <queue>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
using namespace std; 

class BlockQueue
{
  private:
    queue<int>  bq;
    int _capacity;
    pthread_mutex_t lock;
    pthread_cond_t cond_p;  //queue empty
    pthread_cond_t cond_c;  //queue full
    int water_line;
  private:
    void LockQueue()
    {
      pthread_mutex_lock(&lock);
    }
    void UnlockQueue()
    {
      pthread_mutex_unlock(&lock);
    }

    void ProductWait()
    {
      pthread_cond_wait(&cond_p,&lock);
    }
    bool IsFull()
    {
      return (_capacity == bq.size() ? true : false);
    }
    void ConsumeWait()
    {
      pthread_cond_wait(&cond_c,&lock);
    }
    bool IsEmpty()
    {
      return bq.size()==0 ? true : false;
    }
    void SignalConsume()
    {
      pthread_cond_signal(&cond_c);
    }
    void SignalProduct()
    {
      pthread_cond_signal(&cond_p);
    }
    bool IsHighWaterLine()
    {
      return bq.size() > water_line ? true : false;
    }
  public:
    BlockQueue(int cap)
      :_capacity(cap)
      ,water_line(cap*2/3)
    {
      pthread_mutex_init(&lock,NULL);
      pthread_cond_init (&cond_p,NULL);
      pthread_cond_init(&cond_c,NULL);

    }
    void PushData(const int& data)
    {
      //访问阻塞队列
      LockQueue();  //加锁
      while(IsFull())    //推荐用while  如果等待失败就可能会造成假唤醒状态
      {
        //SignalConsume(); //如果生产满了，通知消费者消费   
        ProductWait();
      }

      bq.push(data);
      if(IsHighWaterLine())
      {
        SignalConsume();
      }
      UnlockQueue();
    }

    void PopData(int& data)
    {
        LockQueue();
        while(IsEmpty())
        {
          SignalProduct();
          ConsumeWait();
        }

       data= bq.front();
       bq.pop();

        UnlockQueue();
    }
    ~BlockQueue()
    {
      pthread_mutex_destroy(&lock);
      pthread_cond_destroy(&cond_c);
      pthread_cond_destroy(&cond_p);

    }
};


#endif
