#include<stdio.h>
#include<stdint.h>
#include<pthread.h>
#include<time.h>
#include<string.h>
#include<stdlib.h>


struct timespec now;
long freq;
long check_freq;

 void *currently_time(void *time)
{  
  
    clock_gettime(CLOCK_REALTIME,&now); 
     char buff[100];
     strftime(buff, sizeof buff, "%D %T", gmtime(&now.tv_sec));
     printf("Current time: %s.%09ld \n", buff, now.tv_nsec);
  //   printf("%09ld\n",now.tv_sec);
  return NULL;
}
void *check_time(void *check)
{
  // long old_freq = get_freq();
   long x = (long)check;
  // if(old_freq == x){
  //   return NULL;
  // }
  //else{
  FILE *fp;
  fp = fopen("freq.txt","w");
  fprintf(fp,"%ld",x);
  fclose(fp);
  freq = x;
  now.tv_sec = 0;
  now.tv_nsec = freq;
  return NULL;
//  }
}
void *save_time(void *args)
{
 struct timespec *tpx = (struct timespec *)args;

 //get time from time_and_interval;
 FILE *fp;
 fp = fopen("time_and_interval.txt","r");
 char buff[100];
 fgets(buff,sizeof(buff),fp);
 char *t_sec;
 char *t_nsec;
 const char s[2] = "-";
 t_nsec = strtok(buff,s);
// t_nsec = strtok(NULL,".");
  
 fclose(fp);
 char *eptr;
 long old_nsec = 0;
 //long old_sec = 0;
 old_nsec = strtol(t_nsec,&eptr,10);
//  printf("tin %ld", old_nsec);
//  old_sec = strtol(t_sec,&eptr,10); 
   printf("gia tri cu %ld\n", old_nsec);
   printf("gia tri moi %ld\n", now.tv_nsec);
//   printf("tin %ld", old_sec);

  //  //save time
 FILE *file;
 file = fopen("time_and_interval.txt","w+");
 long diff_sec;
 long diff_nsec;
 diff_sec = (abs)(now.tv_sec - old_sec);
 diff_nsec = (abs)(now.tv_nsec - old_nsec);
 fprintf(fp,"%ld-",diff_nsec);
 //fprintf(fp,"%ld",diff_sec);
 fclose(file);
 printf("offset %ld", diff_nsec);
}

long get_freq(){
  FILE *fp;
  fp = fopen("freq.txt","r");
  char buff[100];
  fgets(buff,sizeof(buff),fp);
  fclose(fp);
 }
int main()
{
   pthread_t sample;
   pthread_t input;
   pthread_t logging;
   pthread_create(&input,NULL,check_time,&check_freq);
   pthread_create(&sample, NULL, currently_time,&freq);
   pthread_create(&logging,NULL,save_time,&now);

   pthread_join(input,NULL);
   pthread_join(sample,NULL);
   pthread_join(logging,NULL);
   return 0;
}
