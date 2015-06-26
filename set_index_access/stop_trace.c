#include<stdio.h>
#include<sys/mman.h>
#include<sys/time.h>
#include<fcntl.h>
#include<stdlib.h>
#include<errno.h>
//#define NUMBER_OF_BITS 7
//#define TOTAL_NUMBER (1<<NUMBER_OF_BITS)
unsigned long long NUMBER_OF_BITS = 0;
unsigned long long TOTAL_NUMBER = 0;

unsigned long long begin_index = 0;
#define TYPE unsigned long long
#define FILL_SET
unsigned long long CACHE_LINE_SIZE = 0;
//unsigned long long location[NUMBER_OF_BITS];
//unsigned long long value[TOTAL_NUMBER];
unsigned long long *location = NULL;
unsigned long long *value = NULL;

// This is the command related data
#include "command.h"
void* config_space_base = NULL;
#define	CFG_SIZE	0x1000U
void begin_trace(){
	char ch;
    int command = CMD_START_TRACE;
	ch = *(char *)(config_space_base + 64*command);
	ch = *(char *)(config_space_base + 64*command);
	ch = *(char *)(config_space_base + 64*command);
	ch = *(char *)(config_space_base + 64*command);
	ch = *(char *)(config_space_base + 64*command);
	ch = *(char *)(config_space_base + 64*command);
	ch = *(char *)(config_space_base + 64*command);
	ch = *(char *)(config_space_base + 64*command);
	ch = *(char *)(config_space_base + 64*command);
	printf("Begin HMTT Trace success!\n");
}

void stop_trace(){
	char ch;
    int command = CMD_STOP_TRACE;
	ch = *(char *)(config_space_base + 64*command);
	ch = *(char *)(config_space_base + 64*command);
	ch = *(char *)(config_space_base + 64*command);
	ch = *(char *)(config_space_base + 64*command);
	ch = *(char *)(config_space_base + 64*command);
	ch = *(char *)(config_space_base + 64*command);
	ch = *(char *)(config_space_base + 64*command);
	ch = *(char *)(config_space_base + 64*command);
	ch = *(char *)(config_space_base + 64*command);
	printf("Begin HMTT Trace success!\n");
}

int main(int argc, char *argv[]){
    int fd = 0, command_fd = 0;
    	command_fd = open("/dev/config_space",O_RDWR);
	printf("Open Succeed\n");
    if (command_fd < 0) {
        printf("can not open /dev/config_space, errno = %d!\n", errno);
        return;
    }

    config_space_base = mmap( 0,CFG_SIZE,PROT_READ|PROT_WRITE,MAP_SHARED,command_fd,0 );
    if (config_space_base < 0) {
		printf("can not mmap /dev/config_space, errno=%d\n", errno);
		return;
    }
    
    stop_trace();
	munmap(config_space_base, CFG_SIZE);
	close(command_fd);
	return 1;
}

