#include<stdio.h>
#include<sys/types.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
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
	write(fds, ubuff, strlen(ubuff)+1);
	printf("\n data is written....\n");
	close(fds);
}