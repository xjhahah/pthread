#include "tp.hpp"

const int num = 5;
int add(int x,int y)
{
  return x+y;
}
int main()
{
  ThreadPool* tp =new ThreadPool(num);
  tp->InitThreadPool();
  int count=1;
  while(1)
  {
    Task t(count,count-1,add);
    count++;
    tp->AddTask(t);
    sleep(1);
  }
  return 0;
}
