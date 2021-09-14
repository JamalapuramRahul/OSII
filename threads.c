#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/resource.h>

/* 
Name:  Rahul Jamalapuram
Cwid:  A20303740
email: rahul.jamalapuram@okstate.edu
*/

pthread_mutex_t mutex;              //this is the mutex variable used for locking and unlocking the threads.
struct rusage mytiming;
struct timeval mytimeval;
long sum=0;                         //this variable is used to store the counter value, Shared variable for both the threads
long count=0;                       //this variable is 
int count_1=0;                      //this variable is used to count the number of times the counter value is incremented by 100 in thread 1.
long count_2=0;                     //this variable is used to count the number of times the counter value is incremented in thread2.

/*
This is the function thread 1 executes
no return value
*/
void * threadFunction1(void * arg){
    int k=0;                                     //Temporary variable for iteration
    printf("Thread 1 started \n");               //printing when thread starts
 for(k=0;k<2500000;k++){                         //iterating the loop for 2.5M times
    if(sum%100==0&&count<2500000){               //checking the condition for divisible by 100 and the counter is less the 2500000, and incrementing by 100.
    
    while(pthread_mutex_lock(&mutex)!=0)         //Locking the thread 1 for its part of execution.
        ;
    sum+=100;                                    //incrementing the counter by 100.
    count_1+=1;                                  //counting the number of times the incrementing happens.
    pthread_mutex_unlock(&mutex);                //Unlocking the mutex, so that other threads can solve their part on shared variable.
    k=k+100;
    count+=100;                                  //incrementing count for condition check.
        }
    if(count<2500000){                           //checking the condition for other circumstances     
            while(pthread_mutex_lock(&mutex)!=0) //locking this thread to compute.
                ;
            sum+=1;                              //incrementing the counter value by 1. 
            count++;                             //used for the condition
            pthread_mutex_unlock(&mutex);        //Unlocking the mutex, so that other threads can solve their part on shared variable.
        }
 }
}

/*
This is the function thread 2 executes
no return value
*/
void * threadFunction2(void * arg){
    printf("Thread 2 started \n");               //printing when thread starts
    for(int k=0;k<2500000;k++){                  //iterating for the condition of 2.5M for the thread.

    while(pthread_mutex_lock(&mutex)!=0)         //locking the thread for no other thread to access the shared value
        ;
        sum+=1;                                  //incrementing the counter
        count_2++;                               //checking for number of times thread 2 increments the counter value
    pthread_mutex_unlock(&mutex);                //unlocking the thread.
}
}

int main(void)
{   
    pthread_t thread1;                           //initialising the thread 1.
    pthread_t thread2;                           //initialising the thread 2.


    pthread_create(&thread1,NULL,threadFunction1,NULL );    //Creating the thread and passing the arguments accordingly
    pthread_create(&thread2,NULL,threadFunction2,NULL );    //Creating the thread and passing the arguments accordingly

    pthread_join(thread1,NULL);                         //Joining the thread.
    pthread_join(thread2,NULL);                         //Joining the thread
    
    printf("Total Counter incremented: %d\n",sum);                      //printing the total counter
    printf("Thread 1 incremented counter by %d times \n",count_1);      //Printing no of times the counter is increased by 100 in thread 1
    printf("Thread 2 incremented counter by %d times \n",count_2);      //Printing no of times the counter is incremented.
    
    getrusage(RUSAGE_SELF, &mytiming); 
    
    printf("Time used : sec: %ld, usec %d\n",mytiming.ru_utime.tv_sec, mytiming.ru_utime.tv_usec); 
    printf("System Time used : sec: %ld, usec %d\n",mytiming.ru_stime.tv_sec, mytiming.ru_stime.tv_usec);    //code used from given assignment
    
    return 0;
}
