#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <omp.h>
//#define	READ
//#define WRITE

#define	cpy
//#define	flush

//#define REMAP_SIZE (0x100000000UL)
//#define REMAP_SIZE (5120UL<<20)
//#define REMAP_SIZE (4096UL<<20)
//#define REMAP_SIZE (10240ULL<<20)
//#define REMAP_SIZE (8192ULL<<20)
#define REMAP_SIZE (6144ULL<<20)

int main(int argc,void *argv[])
{
	char *buf;
	unsigned long *buf1, *buf2;
	char ch;
	int fd;
	unsigned long i,j,k=0;
        //unsigned int off;
 	int iteration = 1;
	if( argc > 1 ){
		iteration = atoi(argv[1]);
	}
	
        //off = atoi(argv[1]);
	
  	fd = open("/dev/ioremap",O_RDWR);
	if(fd<0)
		perror("open error\n");

	buf=(char *)mmap(0,REMAP_SIZE,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
//	buf=(char *)mmap(0,REMAP_SIZE,PROT_READ|PROT_WRITE,MAP_SHARED,fd,(1ULL<<30));
	
	//buf= (char*) malloc(REMAP_SIZE*(sizeof(char))); 
	//buf2= (double*) malloc(REMAP_SIZE/2*(sizeof(char))); 
	//buf1=(unsigned long *)mmap(0,REMAP_SIZE/2,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	//buf2=(unsigned long *)mmap(0,REMAP_SIZE/2,PROT_READ|PROT_WRITE,MAP_SHARED,fd,REMAP_SIZE*2);
	//memset(buf,0x0,REMAP_SIZE);
	//memset(buf2,2,REMAP_SIZE/2);
        if(buf<0)
		{
		perror("map error\n");
		goto error;
		}
	else
		printf("mmap ok to access the memory\n");
	
	printf("begin read cycles\n");

	//printf("%d, %d\n",sizeof(char),sizeof(unsigned long));
	struct timeval tv_pre,tv_after;
	gettimeofday(&tv_pre,NULL);

	printf("\nThe step is %d, the number of adress is %d.\n", 1ULL<<atoi(argv[1]), atoi(argv[2]));
	
	//omp_set_num_threads(4);
	if (strcmp(argv[2], "0") == 0)
		while(1)
		//for(j)
		{
		for(j=0;j<16;j++)
			//for(i=0;i<REMAP_SIZE;i+=64)
			for(i=0;i<REMAP_SIZE;i+=(1ULL<<atoi(argv[1])))
			{
	//			for(k=0;k<20;k++);
					ch += buf[i];
					//ch += buf[1<<atoi(argv[1])];
					printf("%d\n", i);
			}
		}
	else
		//while(1)
		//for(j)
		{
		//for(j=0;j<16;j++)
			//for(i=0;i<REMAP_SIZE;i+=64)
			for(i=0;i< atoi(argv[2])*(1ULL<<atoi(argv[1]));i+=(1ULL<<atoi(argv[1])))
			{
	//			for(k=0;k<20;k++);
					ch += buf[i];
					//ch += buf[1<<atoi(argv[1])];
					printf("%d\n", i);
			}
		}

	//return 0;
	gettimeofday(&tv_after,NULL);
	printf("%c\n",ch);
	printf("time:%.3fms\n",(double)(tv_after.tv_sec-tv_pre.tv_sec)*1000.0 + (double)(tv_after.tv_usec-tv_pre.tv_usec)/1000.0);
//	munmap(buf,REMAP_SIZE);
  //  	close(fd);
//	free(buf1);
	return 0;
error:
	close(fd);
	return 0;
} 
