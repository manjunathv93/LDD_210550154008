#include<stdio.h>
#include<sys/types.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
int main()
{
	int fds;
	char ubuff[50];
	fds = open("/dev/mydevice",O_RDWR,0777);
	if(fds < 0)
	{
		printf("\ndevice not opened\n");
		exit(1);
	}
	read(fds,ubuff,sizeof(ubuff));
	printf("\n it is from write appln.... %s ....\n",ubuff);
	close(fds);
}
