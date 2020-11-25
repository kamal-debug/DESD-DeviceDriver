#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#define SIZE 32

int main(int argc, char const *argv[])
{
  
    char buffw[] = "fromUser";

    int fd,ret;
    fd = open("/dev/sample1",O_RDWR);

    if(fd<0)
    {
        printf("Error in opening The file\n");
    }

    
    ret = write(fd,buffw,sizeof(buffw));
    printf("Write %d byte\n",ret);

    return 0;
}
