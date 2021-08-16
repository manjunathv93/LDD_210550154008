#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
int main()
{
	int Fds;
	Fds = open("/dev/mycdev",O_RDWR,0777);
	if(Fds < 0)
	{
		printf("Device is not opened...error occured\n");
		exit(1);
	}
	close(Fds);
}

