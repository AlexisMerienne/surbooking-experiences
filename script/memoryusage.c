#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <sys/wait.h>



//used = total - free - buffers - cache


int memoryinfo(){

    
    FILE * fp;

    char str1[256], str2[256];



    fp = fopen("/proc/meminfo", "r");
    
    if (fp == NULL)
        exit(EXIT_FAILURE);

    float memtotal,memfree,membuffer,memcached;
    while (fscanf(fp, "%s %s %*s ",str1,str2) != EOF ){
        if(strcmp(str1,"MemTotal:")==0){
            memtotal=atof(str2);
        }
        if(strcmp(str1,"MemFree:")==0){
            memfree=atof(str2);
        }
        if(strcmp(str1,"Buffers:")==0){
            membuffer=atof(str2);
        }
        if(strcmp(str1,"Cached:")==0){
            memcached=atof(str2);
        }
    }
    int memused = memtotal - memfree - membuffer - memcached;
  
    fclose(fp); 
    
    
    return memused;
}

int swapinfo(){

    
    FILE * fp;

    char str1[256], str2[256];


    fp = fopen("/proc/meminfo", "r");
    
    if (fp == NULL)
        exit(EXIT_FAILURE);

    float swaptotal,swapfree;
    while (fscanf(fp, "%s %s %*s ",str1,str2) != EOF ){
        if(strcmp(str1,"SwapTotal:")==0){
            swaptotal=atof(str2);
        }
        if(strcmp(str1,"SwapFree:")==0){
            swapfree=atof(str2);
        }
    }
    int swapused = swaptotal - swapfree;
  
    fclose(fp);
    
    
    return swapused;
}

int memprocinfo(){

    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;


    fp  = fopen("/proc/1761670/status", "r");
    
    if (fp  == NULL)
        exit(EXIT_FAILURE);

    double memRSS=0;
     while ((read = getline(&line, &len, fp )) != -1) {
        char *str;
        int bool=0;
        str = strtok(line,":");
        if(str)
        {
            if(strcmp(str,"VmHWM")==0){
                bool=1;
            }

        }
        str = strtok(NULL, ":");

        if(str){
            if (bool==1){
                memRSS = atof(str);
                break;
            }
        } 
    }

  
    fclose(fp ); 
    
    
    return (int)memRSS;

}






int main(int argc, char * argv[]){

    float time = 60;
    float finesse = round(500.0)/100.0;

    if (argc>1){
    	time = atof(argv[1]);
    }
    if (argc>2){
        finesse = round(atof(argv[2])*100.0)/100.0;
    }


    FILE *foutput = fopen("out.txt", "w");

    if (foutput == NULL)
    {
        printf("Error opening output file!\n");
        exit(1);
    }
    
    clock_t start = clock();
    
    int memused = memoryinfo();
    int swapused = swapinfo();
    int memproc = memprocinfo();

    printf("%lf %d %d %d\n",0.0,memused,swapused,memproc);
    fprintf(foutput, "%lf %d %d %d\n",0.0,memused,swapused,memproc);
    float n=finesse;


    while(0==0){
        clock_t end = clock();
        float diff_second =  (float)(end - start)/ CLOCKS_PER_SEC;
        diff_second = round(diff_second*100.0)/100.0;

    if(diff_second==n){
            memused = memoryinfo();
            swapused = swapinfo();
            memproc = memprocinfo();
            printf("%lf %d %d %d\n",diff_second,memused,swapused,memproc);
            fprintf(foutput, "%lf %d %d %d\n",diff_second,memused,swapused,memproc);
            n=round((finesse+n)*100.0)/100.0;
        }
        if (diff_second==time){break;}
    }
    fclose(foutput);


  
}
