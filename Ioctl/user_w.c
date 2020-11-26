#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/ioctl.h> // INclude this For IOCTL

#include "myioctl.h" // Hader file for IOCTL

#define SIZE 32

int main(int argc, char const *argv[])
{
  
    char buffw[] = "fromUser";

    int fd,ret;
    fd = open("/dev/sample0",O_RDWR);

    if(fd<0)
    {
        printf("Error in opening The file\n");
    }

    ret = ioctl(fd,CMD_1); // user IOCTL
    ret = ioctl(fd,CMD_2,1); // User space IOCTL

    
    ret = write(fd,buffw,sizeof(buffw));
    printf("Write %d byte\n",ret);

    return 0;
}
