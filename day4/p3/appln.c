
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int8_t w_buf[1024];
int8_t r_buf[1024];

int main()
{
        int fds;
        char func;
        fds = open("/dev/my_dev", O_RDWR);
        if(fds < 0) 
	{
                printf("Cannot open device file...\n");
                return 0;
        }

        while(1) {
                printf("Please Enter the Functionality to be done...\n");
                printf("1. Write\n");
                printf("2. Read\n");
                printf("3. Exit\n");
                scanf(" %c", &func);
                printf("The functionality selected is %c\n", func);
                
                switch(func) 
		{
                        case '1':
                                printf("Enter data to be written:");
                                scanf(" %[^\t\n]s", w_buf);
                                printf("write in process...");
                                write(fds, w_buf, strlen(w_buf)+1);
                                printf("completed\n");
                                break;
                        case '2':
                                printf("Read in process...");
                                read(fds, r_buf, 1024);
                                printf("completed\n\n");
                                printf("data = %s\n\n", r_buf);
                                break;
                        case '3':
                                close(fds);
                                exit(1);
                                break;
                        default:
                                printf("No functionality is selected\n");
                                break;
                }
        }
        close(fds);
}