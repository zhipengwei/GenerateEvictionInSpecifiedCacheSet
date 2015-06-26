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
    int command;
    char ch;

    fd = open("/dev/config_space",O_RDWR);
	printf("Open Succeed\n");
    if(fd < 0)
    {
        printf("can not open /dev/config_space, errno = %d!\n", errno);
        return 0;
    }

    config_space_base = mmap( 0,CFG_SIZE,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0 );
    if( config_space_base < 0 ){
	printf("can not mmap /dev/config_space, errno=%d\n", errno);
	return -1;
    }

    command = CMD_BAR_RECFG;
    ch = *(char *)(config_space_base + 64*command);
    ch = *(char *)(config_space_base + 64*command);
    ch = *(char *)(config_space_base + 64*command);
    ch = *(char *)(config_space_base + 64*command);
    ch = *(char *)(config_space_base + 64*command);
    ch = *(char *)(config_space_base + 64*command);

    printf("Reconfig BAR success!\n");
    
    munmap( config_space_base, CFG_SIZE );	
    close(fd);

    return 0;
}
