#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#define SIZE 32

int main(int argc, char const *argv[])
{
    char buffr[SIZE];
   

    int fd,ret;
    fd = open("/dev/sample1",O_RDWR);

    if(fd<0){
    printf("Error in opening The file\n");
    }

    ret = read(fd,buffr,SIZE);
    printf("getting: %s No. of Bytes Read: %d\n",buffr,ret);
    

    return 0;
}
