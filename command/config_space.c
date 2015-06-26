/************memory_dev.c**************/
#include <linux/kernel.h>
#include <linux/errno.h>
//#include <linux/config.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/mm.h>
//#include <linux/wrapper.h> /* for mem_map_(un)reserve */
#include <asm/io.h> /* for virt_to_phys */
#include <linux/slab.h> /* for kmalloc and kfree */
#include "cfg_space.h"


/****************************************************************/
//clc added: lack the following two header files, error in kernel 2.6.32
/************************************************************/
#include <linux/fs.h>
#include <linux/cdev.h>

#ifndef __KERNEL__ 
#define __KERNEL__ 
#endif
/******************************************************************************/
//end of clc added
/****************************************************************************/

MODULE_LICENSE("GPL");
MODULE_AUTHOR("LL");
MODULE_DESCRIPTION("MTT test");


//MODULE_PARM(mem_start, "i");
//MODULE_PARM(mem_size, "i");
//module_param(mem_start, int, 0644); 
//module_param(mem_size, int, 0644); 
/*
module_param(myshort, short, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);

MODULE_PARM_DESC(myshort, "A short integer");

module_param(myint, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

MODULE_PARM_DESC(myint, "An integer");

module_param(mylong, long, S_IRUSR);

MODULE_PARM_DESC(mylong, "A long integer");

module_param(mystring, charp, 0000);

MODULE_PARM_DESC(mystring, "A character string");
*/
/*struct memory_dev_dev{
	struct semaphore sem; // mutual exclusion semaphore 
	struct cdev cdev; // Char device structure 
};*/

//the start address of the buffer is DMA_CONTROL_REGISTER, the size of the buffer is (DMA_BUFFER_ADDRESS-DMA_CONTROL_REGISTER+DMA_BUFFER_TOTAL_SIZE)B
static unsigned long  long mem_start = cfg_start, mem_size = cfg_end - cfg_start;


static char *reserve_virt_addr;
static int major = 0;
module_param(major, int, 0);

static dev_t dev_config_space;
static struct cdev cdev_config_space;

static int mmapdrv_init(void);
static void  mmapdrv_clean(void); 
int mmapdrv_open(struct inode *inode, struct file *file);
int mmapdrv_release(struct inode *inode, struct file *file);
int mmapdrv_mmap(struct file *file, struct vm_area_struct *vma);

static struct file_operations mmapdrv_fops =
{
	.owner = THIS_MODULE,
	.mmap = mmapdrv_mmap,
	.open = mmapdrv_open,
	.release = mmapdrv_release
//	ioctl:		mmapdrv_ioctl,
};


#ifdef MODULE_NO
EXPORT_SYMBOL(mmapdrv_init);
EXPORT_SYMBOL(mmapdrv_clean);
EXPORT_SYMBOL(mmapdrv_open);
EXPORT_SYMBOL(mmapdrv_release);
EXPORT_SYMBOL(mmapdrv_mmap);
#endif



static int __init mmapdrv_init(void)
{
	
	int result;
	int err;
	if( major ){
		dev_config_space = MKDEV( major,0 );//first dev
		result = register_chrdev_region( dev_config_space, 1, "config_space" );
	}
	else{
		result = alloc_chrdev_region( &dev_config_space, 0, 1, "config_space" );
		major = MAJOR( dev_config_space );
	}
	if( result < 0 )
	{
		printk("<0>config_space: unable to get major %d\n", major);
		return result;
	}
	if( major == 0 )
	{
		major = result;
	}
	cdev_init( &cdev_config_space, &mmapdrv_fops);
	cdev_config_space.owner = THIS_MODULE;
	cdev_config_space.ops = &mmapdrv_fops;
	err = cdev_add ( &cdev_config_space, dev_config_space, 1);
	/* Fail gracefully if need be */
	if (err)
	{
		printk ("<0>Error %d adding config_space cdev--%d:%d", err, major,MINOR(dev_config_space) );
	}


	printk("<0>config_space device major = %d\n", major);

	printk("<0>%llx	%llx\n",mem_start,mem_size);

	reserve_virt_addr = ioremap_nocache(mem_start , mem_size );
	printk("<0>reserve_virt_addr = 0x%lx\n", (unsigned long)reserve_virt_addr);
	if (reserve_virt_addr==NULL)
	{
		printk("<0>error happened in config_space\n");
		//unregister_chrdev(major, "mmapdrv");
		cdev_del( &cdev_config_space );
        	unregister_chrdev_region( dev_config_space, 1 );
		return - ENODEV;
	}
	
	return 0;
}

/* remove the module */
static void __exit mmapdrv_clean(void)
{
	if (reserve_virt_addr)
	iounmap(reserve_virt_addr);
	
	cdev_del( &cdev_config_space );
	unregister_chrdev_region( dev_config_space, 1 );
	//unregister_chrdev(major, "mmapdrv");
	printk("<0>config_space module exit\n");
	return ;
}
module_init(mmapdrv_init);
module_exit(mmapdrv_clean);
int mmapdrv_open(struct inode *inode, struct file *file)
{
//	MOD_INC_USE_COUNT;
	return (0);
}

int mmapdrv_release(struct inode *inode, struct file *file)
{
//	MOD_DEC_USE_COUNT;
	return (0);
}

int mmapdrv_mmap(struct file *file, struct vm_area_struct *vma)
{
	unsigned long offset = vma->vm_pgoff << PAGE_SHIFT;
	unsigned long size = vma->vm_end - vma->vm_start;
	unsigned long prot = pgprot_val(vma->vm_page_prot);
	prot |= (_PAGE_PCD | _PAGE_PWT | _PAGE_PROTNONE); 
	//prot |= (_PAGE_PWT | _PAGE_PROTNONE); 
	printk("<1>prot=%lx\n",prot);
	if (size > mem_size )
	{
		printk("<0>size too big,required size is %lld,avilable size is %lld\n",size,mem_size);
		return ( - ENXIO);
	}
	printk("<1>offset = %lx,size=%lx\n",offset,size);
	offset = offset + mem_start ;

/* we do not want to have this area swapped out, lock it */
	vma->vm_flags |= VM_LOCKED;
 	vma->vm_flags |= VM_IO;
	vma->vm_flags |= VM_RESERVED;
	vma->vm_flags |= VM_READ;
	vma->vm_flags |= VM_WRITE;
	vma->vm_flags |= VM_MAYREAD;
	vma->vm_flags |= VM_MAYWRITE;
	vma->vm_flags |= VM_DONTEXPAND;
	vma->vm_flags |= VM_GROWSUP;
	vma->vm_flags |= VM_SHARED;
	if (io_remap_pfn_range(vma, vma->vm_start, offset>>PAGE_SHIFT, size,__pgprot(prot)))
	{
		printk("<0>remap page range failed\n");
		return - ENXIO;
	}
	printk("<1>keep mem mmaped\n");
	return (0);
}

