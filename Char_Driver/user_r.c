#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#define SIZE 32

int main(int argc, char const *argv[])
{
    char buffr[SIZE];
    char buffw[] = "fromUser";
   

    int fd,ret;
    fd = open("/dev/sample0",O_RDWR);

    if(fd<0){
    printf("Error in opening The file\n");
    }

    ret = read(fd,buffr,SIZE);
    ret = write(fd,buffw,sizeof(buffw));

    
    

    return 0;
}