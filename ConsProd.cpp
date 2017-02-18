#include <pthread.h>
#include <iostream>
#include <stdlib.h>
#include <queue>
#include <unistd.h>

std::queue<int> q; //queue for products
const int max=10; //max size of our queue
pthread_mutex_t mutex; // a lock
pthread_cond_t cond=PTHREAD_COND_INITIALIZER; // a condition

void* Producer(void*) 
{
  int ret;
  int number=1; // number of our product
  while(number!=2*max) // how many products will be pushed (can be changed)
 {
   if(q.size()!=max)
    {
     double prodtime=rand()%3; // a specific time in seconds for each item to be produced (or pushed) 
     if(pthread_mutex_lock(&mutex)) exit(-1);
     
     q.push(number);
     std::cout<<"pushed "<<number<<std::endl;
     if(pthread_cond_signal(&cond)) exit(1); //signaling the other thread, waiting on cond , to wake up
	 
     
     if(pthread_mutex_unlock(&mutex)) exit(-1);
     sleep(prodtime);
     ++number;
    }
   else
    {
     if(pthread_mutex_lock(&mutex)) exit(-1);
     if(pthread_cond_wait(&cond,&mutex)) exit(2); //waiting for some products to be consumed (or popped)
     if(pthread_mutex_unlock(&mutex)) exit(-1);
    }
 } 
}
void* Consumer(void*)
{
 while(q.front()!=2*max-1)
 {
   if(!q.empty())
    {
     int constime=rand()%3;
     if(pthread_mutex_lock(&mutex)) exit(-1);
     
     int popedn=q.front();
     q.pop();
     std::cout<<"          poped "<<popedn<<std::endl;
     if(pthread_cond_signal(&cond)) exit(1);
     
     if(pthread_mutex_unlock(&mutex)) exit(-1);
     sleep(constime);
    }
   else
    {
     if(pthread_mutex_lock(&mutex)) exit(-1);
     if(pthread_cond_wait(&cond,&mutex)) exit(2);
     if(pthread_mutex_unlock(&mutex)) exit(-1);
    }
 }
 
}

int main()
{
 pthread_t prod;
 pthread_t cons;
 if(pthread_create(&prod,NULL,Producer,NULL)) exit(3);
 if(pthread_create(&cons,NULL,Consumer,NULL)) exit(3); 
 if(pthread_join(prod,NULL)) exit(4);
 if(pthread_join(cons,NULL)) exit(4);
}
