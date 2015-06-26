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

void display_value(TYPE *data, int length, char *message){
    int i = 0;
    printf("%s\n", message);
    while(i < length){
        printf("%llx\n", *(data + i));
       // if((i % 8) == 7){
       //     putchar('\n');
       // }
        i++;
    }
    putchar('\n');
}
// The trace sequence is 0 1 2 3 4 5 6 7 
//void generate(){
//    int i = 0, j = 1, mask = 0;
//    int mask_result;
//    while(i < TOTAL_NUMBER){
//        j = 0;
//        mask = 1;
//        value[i] = 0;
//        while(j < NUMBER_OF_BITS){
//             mask_result = mask & i;
//             if(mask_result != 0)
//                   value[i] += (1<<location[NUMBER_OF_BITS - j - 1]);
//                   j++;
//                   mask = mask << 1;
//        }
//        i++;
//    }                                                                                                                                
//}

// The trace sequence is 0 2 4 1 3 5
//void generate(){
//    int i = 0, j = 1, mask = 0;
//    int mask_result;
//    while(i < TOTAL_NUMBER){
//        j = 0;
//        mask = 1;
//        value[i] = 0;
//        while(j < NUMBER_OF_BITS){
//             mask_result = mask & i;
//             if(mask_result != 0)
//                   value[i] += (1<<location[NUMBER_OF_BITS - j - 1]);
//                   j++;
//                   mask = mask << 1;
//        }
//        i++;
//    }                                                                                                                                
//}

// This is used to test 8 dimm
void generate(unsigned long long set_index)
{
    int i = 0;
    while(i < TOTAL_NUMBER){
    	value[i] += i* (1ULL << 17) + (set_index << 6);
        i++;
    }
}


int my_rand(){
    int random = rand();
//    printf("%d\n", random);
    //getchar();
    return random;
}

void discretise(unsigned long long *value, int length){
    int i = 1, j = 0;
    unsigned long long tmp = 0;
    printf("This is the discretise function.\n");
    while(i < length){
        j = my_rand() % i;
        tmp = *(value + j);
        *(value + j) = *(value + i);
        *(value + i) = tmp;
        i++;
    }
}

void my_fopen(TYPE **buf, unsigned long long array_size, int *fd){
    printf("This is the open function.\n");
    *fd = open("/dev/ioremap",O_RDWR);
    if(*fd < 0){
        //perror("open error\n");
        printf("Fail to open ioremap device!\n");
        *buf = NULL;
        exit(-1);
    }    
    //*buf=(TYPE*)mmap(0, array_size, PROT_READ|PROT_WRITE, MAP_SHARED, *fd, 0);
    *buf=(TYPE*)mmap((void*)(0x4fff00000000ULL), array_size, PROT_READ|PROT_WRITE, MAP_SHARED, *fd, 0);
    printf("Virtual address: %llx", *buf);
    
    if(*buf == MAP_FAILED){
        printf("mmap failed!\n");
        exit(-1);
    }
//    *buf=(char*)malloc(array_size);
}

void init_buffer(unsigned long long *buffer, unsigned long long array_size, unsigned long long *value, unsigned long long length){
    unsigned long long i = 0;
    TYPE address = 0, index = 0, next_address = 0, next_address_index = 0;
    printf("This is the initial function.\n");
    while(i < length - 1){
        address = *(value + i);
        index = address / sizeof(unsigned long long);
        //printf("origin: address: %llu index: %llu\n", address, index);
        next_address = *(value + i + 1);
        next_address_index = next_address / sizeof(unsigned long long);
       // *(buffer + index) = (TYPE)(&(*(buffer + next_address_index)));
        *(buffer + index) = (TYPE)(((buffer + next_address_index)));
        //printf("%llu\n", (TYPE)(((buffer + next_address_index))));
        //printf("next: address: %llu index: %llu target_value %llu\n", next_address, next_address_index, *(buffer + index));
        i++;
    }
    address = *(value+ length - 1);
    index = address / sizeof(unsigned long long);
    //*(buffer + index) = *value;
    next_address = *value;
    next_address_index = next_address / sizeof(unsigned long long);
    *(buffer + index) =(TYPE)(buffer + next_address_index);
    //printf("buffer[0]: %llu\n", buffer[0]);
    printf("This is the end of init buffer!\n");
	begin_index = *(value + i) / sizeof(unsigned long long);
}

void statistics(unsigned long long size, unsigned long long count, struct timeval *start, struct timeval *stop){
    struct timeval interval;
    if(stop->tv_usec > start->tv_usec){
        interval.tv_usec = stop->tv_usec - start->tv_usec;
        interval.tv_sec = stop->tv_sec - start->tv_sec;
    }
    else {
        interval.tv_usec = 1000000 - start->tv_usec + stop->tv_usec;
        interval.tv_sec = stop->tv_sec - start->tv_sec - 1;
    }

    printf("start_time: %d %d\nstop_time: %d %d\n", start->tv_sec, start->tv_usec, stop->tv_sec, stop->tv_usec); 
    printf("interval: %d %d\n", interval.tv_sec, interval.tv_usec);
    printf("Average time: %lf\n", (double)(interval.tv_sec * 1000000 + interval.tv_usec)/ count);
}

void test(unsigned long long *buffer, unsigned long long array_size, unsigned long long iteration){
    struct timeval start, stop;
    unsigned long long count = array_size / CACHE_LINE_SIZE;
    unsigned long long i = 0, j = 0, k = 0;
    unsigned long long addr = 0, next = 0;
//It took me half a day to solve this bug!
    addr = buffer[begin_index];
    printf("Test function is going to begin! The begin_index is %llx\n", begin_index);
	begin_trace();
    gettimeofday(&start, 0);
    while(j < iteration){
		
        addr = *(TYPE*)(addr);   
        *(TYPE*)(addr+16) = 1;   
		while(k++ < 1000);	
	    //printf("*addr %llx *(addr+1):%llx\n", addr, *(TYPE*)(addr+8)); 
        //fflush(stdin);
        //getchar();
        j++;k=0;
    }
    
    gettimeofday(&stop, 0);
	stop_trace();
    statistics(array_size, iteration, &start, &stop);
}

int main(int argc, char *argv[]){
    unsigned long long *buffer = NULL;    
    int fd = 0, command_fd = 0;
    unsigned long long array_size = 0;
    array_size = 1ULL << 30; 
    CACHE_LINE_SIZE = 64;
    
    unsigned long long total_access = 1ULL << atoi(argv[3]);
    unsigned long long iteration = total_access;

    printf("array_size:      %llx\n", array_size);
    printf("iteration        %llu\n", iteration); 
    //getchar();
    NUMBER_OF_BITS = atoi(argv[4]);
    TOTAL_NUMBER = 1ULL << NUMBER_OF_BITS;
    
    if(argc != (5 + NUMBER_OF_BITS + 1)){
        printf("The number of parameters is %d, You need specify parameters!\n", argc);
        exit(-1);
    }

	//TOTAL_NUMBER = ((62ULL<<30) - (1ULL<<20)) >> 17;
	TOTAL_NUMBER = ((1ULL<<30) >> 17); 
	 	
    value = (unsigned long long*)malloc(sizeof(unsigned long long) * TOTAL_NUMBER);
    location = (unsigned long long*)malloc(sizeof(unsigned long long) * NUMBER_OF_BITS);
    if((!value) || (!location)){
        printf("Malloc fail!\n");
        exit(-1);
    }

    int i = 0;
    for(i = 0; i < NUMBER_OF_BITS; i++){
        location[i] = atoi(argv[5+i]);
    }    
   	unsigned long long set_index = ((unsigned long long)(atoi(argv[5+NUMBER_OF_BITS]))); 
	printf("Left bound is %llx\n", set_index);
    printf("value and location malloc.\n");
    display_value(location, NUMBER_OF_BITS, "This is the bits choosed.\0");
    generate(set_index);
    display_value(value, TOTAL_NUMBER, "This is the addresses before begin hashed.\0");
    //discretise(value, TOTAL_NUMBER);
    display_value(value, TOTAL_NUMBER, "This is the addresses after being hashed.\0");
    //getchar();
    my_fopen(&buffer, array_size, &fd);

	command_fd = open("/dev/config_space",O_RDWR);
	printf("Open Succeed\n");
    if (command_fd < 0)
    {
        printf("can not open /dev/config_space, errno = %d!\n", errno);
        return;
    }

    config_space_base = mmap( 0,CFG_SIZE,PROT_READ|PROT_WRITE,MAP_SHARED,command_fd,0 );
    if (config_space_base < 0)
	{
		printf("can not mmap /dev/config_space, errno=%d\n", errno);
		return;
    }


    init_buffer(buffer, array_size, value, TOTAL_NUMBER);
    test(buffer, array_size, iteration);
    free(location);
    munmap(buffer, array_size);
	munmap(config_space_base, CFG_SIZE);
    free(value);
	close(command_fd);
    close(fd);
	return 1;
}

