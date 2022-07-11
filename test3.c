#include<stdio.h>
#include<stdint.h>
#include<pthread.h>
#include<time.h>
#include<string.h>
#include<stdlib.h>
#include <unistd.h>
// global
long cycle;
struct timespec now;


//main of thread sample
void *currently_time(void *time)
{  
     cycle = (long) time;
     char buff[100];
      clock_gettime(CLOCK_REALTIME,&now);
      strftime(buff, sizeof buff, "%D %T", gmtime(&now.tv_sec));
      printf("Current time: %s.%09ld \n", buff, now.tv_nsec);
  return NULL;
}

//main of thread input
void *check_time(void *time)
{ 
      long x = ((long)time);
      struct timespec timecheck;
      FILE *file;
      file = fopen("freq.txt","r");
      char buff[100];
      fgets(buff,sizeof(buff),file);
      //convert from string to long
      char *eptr;
      long data;
      data = strtol(buff,&eptr,10);
      fclose(file);


    long checktime = data;
    if(checktime != x) //check parameters
    {
      FILE *file;
        file = fopen("freq.txt","w");
        fprintf(file,"%ld",x);
        fclose(file);
        cycle = x;
        timecheck.tv_nsec = data; // attach
    }
      else
      {
        return NULL;
      } 
        return NULL;
}

//main of thread logging
void *save_time(void *time)
{
    FILE *fp;
    fp = fopen("time_and_interval.txt","r");
    char buff[100];
    fgets(buff,sizeof(buff),fp);
    char *t_nsec;
    //read to first element to "-" element
    const char s[2] = "-";
    t_nsec = strtok(buff,s);
    fclose(fp);

    //convert to long 
    char *eptr;
    long old_nsec = 0;
    old_nsec = strtol(t_nsec,&eptr,10);

      printf("old_value %ld\n", old_nsec);
      printf("new_value %ld\n", now.tv_nsec);

    //save time
    FILE *file;
    FILE *file1;
    file1 = fopen("save_value.txt","a+"); // save all of offset values in file
    file = fopen("time_and_interval.txt","w+");
   
    long interval;
    interval = (abs)(now.tv_nsec - old_nsec);
    fseek( file, 0, SEEK_SET );
    fprintf(file,"%ld-",now.tv_nsec);
    fprintf(file,"%ld",interval);
    fprintf(file1,"%ld\n",interval);  
    fclose(file);
}
int main(){
    int j = 1;
    int i = 1;
    int* ptr;
    pthread_t sample;
    pthread_t input;
    pthread_t logging;
    
    while(1){
      pthread_create(&input,NULL,check_time,&i);
      pthread_create(&sample, NULL, currently_time,&j);
      pthread_create(&logging,NULL,save_time,&i);
      pthread_join(input,(void**)&ptr);
      pthread_join(logging,(void**)&ptr);
      pthread_join(sample, (void**)&ptr);
      sleep(2);
    }
   return 0;
}