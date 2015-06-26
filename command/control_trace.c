#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <errno.h>

#include "command.h"

#define	CFG_SIZE	0x1000U

void* config_space_base = NULL;


main(int argc,char ** argv)
{
    int fd;
    int command = 0;
    char ch;
	
	/*********************************************************************************************/
	if(argc != 2){
		printf("usage:./control_trace cmd(0:idle,1:start,2:stop,3:pause,4:soft_reset,5:hard_reset)\n");
		exit(-8);
	}
	command = atoi(argv[1]);	
	/*******************************************************************************************/

    fd = open("/dev/config_space",O_RDWR);
    if(fd < 0)
    {
        printf("can not open /dev/config_space, errno = %d!\n", errno);
        return 0;
    }
	printf("Open /dev/config_space Successfully\n");

    config_space_base = mmap( 0,CFG_SIZE,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0 );
    if( config_space_base < 0 ){
	printf("can not mmap /dev/config_space, errno=%d\n", errno);
	return -1;
    }
	printf("mmap /dev/config_space into process addr space successfully\n");
 	

	switch(command){
		case 0:
			printf("\n");
			break;
		case 1:
			printf("Begin HMTT Trace successfully\n");
			break;
		case 2:
			printf("Stop HMTT Trace successfully\n");
			break;
		case 3:
			printf("Pause HMTT Trace successfully\n");
			break;
		case 4:
			printf("Soft Reset HMTT Trace successfully\n");
			break;
		case 5:
			printf("Hard Reset HMTT Trace successfully\n");
			break;
		default:
			printf("Invalid parameter for control_trace\n");
			printf("usage:./control_trace cmd(0:idle,1:start,2:stop,3:pause,4:soft_reset,5:hard_reset)\n");
			exit(-8);
	}
	
    ch = *(char *)(config_space_base + 64*command);

    //printf("Begin HMTT Trace success!\n");

    
    munmap( config_space_base, CFG_SIZE );	
    close(fd);
	
	printf("Exit from control_trace\n");

    return 0;
}
