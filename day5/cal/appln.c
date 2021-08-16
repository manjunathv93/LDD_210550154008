#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/ioctl.h>


#define WR_VALUE _IOW('a','a',int32_t*)
#define RD_VALUE _IOR('a','b',int32_t*)


int main()
{  
 int32_t num;
    int fd;
    int32_t res,number1,number2;
 
	printf("Driver is opened...\n");
    fd = open("/dev/cal_device",O_RDWR);
    if(fd<0) {
        printf("Cannot open device file ...\n");
        return 0;
    }

    printf("Enter an option \n\n");
    printf("\n 1. Add \n  2. Subtract \n  3. Multiply \n  4. Divide \n\n\n ");
    scanf("%d",&num);   
    if(num > 4 && num < 1)
        {
        printf("Invalid option...try again");
        return 0;
        }

    ioctl(fd,WR_VALUE,(int32_t*) &num);
    printf("Enter the first number :\n");
    scanf("%d",&number1);
    ioctl(fd,WR_VALUE,(int32_t*) &number1);

    printf("Enter the second number :\n");
    scanf("%d",&number2);
    ioctl(fd,WR_VALUE,(int32_t*) &number2);

   
    printf("Data is read from driver \n");
    ioctl(fd,RD_VALUE,(int32_t*)&res);
    printf("value is %d\n",res);

    printf("Driver is closed...\n");
    close(fd);

}  
