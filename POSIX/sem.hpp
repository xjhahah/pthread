#ifndef __SEM__HPP__
#define __SEM__HPP__
#include <iostream>
#include <vector>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>
#include <unistd.h>

using namespace std;

class RingQueue
{
  private:
    vector<int> ring;
    int cap;
    sem_t blank_sem;  //格子
    sem_t data_sem;   //格子中存的数据
   
    int c_step;  //消费者脚步
    int p_step;  //生产者脚步
  private:
    void P(sem_t& sem)
    {
      sem_wait(&sem);
    }
    void V(sem_t& sem)
    {
      sem_post(&sem);
    }
  public:
    RingQueue(int _cap)
      :ring(_cap)
      ,cap(_cap)
    {
      c_step = p_step = 0;
     sem_init(&blank_sem,0,_cap);   //缺省为线程内共享
     sem_init(&data_sem,0,0);
    }
    void PushData(const int& data)
    {
      P(blank_sem);
      ring[p_step]=data;

      V(data_sem);

      p_step++;
      p_step %= cap;  //防止越界
    }
    void PopData(int& data)   //先申请数据在释放空格
    {
      P(data_sem);
      data = ring[c_step];
      V(blank_sem);
      c_step++;
      c_step %= cap;
    }
    ~RingQueue()
    {
      sem_destroy(&blank_sem);
      sem_destroy(&data_sem);

    }
};

#endif
