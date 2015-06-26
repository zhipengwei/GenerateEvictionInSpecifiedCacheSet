/*************mmap_ioremap.c**************/
#include <linux/kernel.h>
#include <linux/errno.h>
//#include <linux/config.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/mm.h>
//#include <linux/wrapper.h> /* for mem_map_(un)reserve */
#include <asm/io.h> /* for virt_to_phys */
#include <linux/slab.h> /* for kmalloc and kfree */
#include <linux/fs.h> // zhuyan
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
//static unsigned long long mem_start = 4096, mem_size = 6144;
//static unsigned long long mem_start = 4096, mem_size = 2048;
static unsigned long long mem_start = 4096, mem_size = 2048;
//static unsigned long long mem_start = 4096, mem_size = 2048;
//static unsigned long long mem_start = 2048, mem_size = 2048;
//static unsigned long long mem_start = 10240/*(4096 + 1024)*/, mem_size = 6144;  //4096
static char *reserve_virt_addr,ch_tmp;
static int major;
static int mmapdrv_init(void);
static void  mmapdrv_clean(void); 
int mmapdrv_open(struct inode *inode, struct file *file);
int mmapdrv_release(struct inode *inode, struct file *file);
int mmapdrv_mmap(struct file *file, struct vm_area_struct *vma);

static struct file_operations mmapdrv_fops =
{
	.owner =	THIS_MODULE, 
	.mmap  =	mmapdrv_mmap,
	.open  =	mmapdrv_open, 
	.release =	mmapdrv_release,
//	ioctl:		mmapdrv_ioctl,
};
/*
#ifdef MODULE
EXPORT_SYMBOL(mmapdrv_init);
EXPORT_SYMBOL(mmapdrv_clean);
EXPORT_SYMBOL(mmapdrv_open);
EXPORT_SYMBOL(mmapdrv_release);
EXPORT_SYMBOL(mmapdrv_mmap);
#endif
*/


static int mmapdrv_init(void)
{
	if ((major = register_chrdev(0, "ioremap", &mmapdrv_fops)) < 0)
	{
		printk("mmapdrv: unable to register character device\n");
		return ( - EIO);
	}
	printk("mmap device major = %d\n", major);

	printk("high memory physical address %ldM\n", virt_to_phys(high_memory) /1024 / 1024);
printk("<0>%llx %llx\n",mem_start*1024*1024,mem_size*1024*1024);
	//reserve_virt_addr = ioremap_nocache(mem_start *1024 * 1024, mem_size *1024 * 1024);
//	reserve_virt_addr = ioremap(mem_start *1024 * 1024, mem_size *1024 * 1024);
	reserve_virt_addr = ioremap_cache(mem_start *1024 * 1024, mem_size *1024 * 1024);
	printk("reserve_virt_addr = 0x%lx\n", (unsigned long)reserve_virt_addr);
	if (reserve_virt_addr)
	{
		int i;
	for (i = 0; i < 17; i += 1)

	{
		ch_tmp = reserve_virt_addr[64+(i%2)*64];
	}
	printk("mem init as 'ncic'\n");
	}
	else
	{
		unregister_chrdev(major, "mmapdrv");
		return - ENODEV;
	}
	
	return 0;
}

/* remove the module */
static void mmapdrv_clean(void)
{
	if (reserve_virt_addr)
	iounmap(reserve_virt_addr);

	unregister_chrdev(major, "mmapdrv");
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
	//prot |= (_PAGE_PCD | _PAGE_PWT | _PAGE_PROTNONE); 
//	prot |= ( _PAGE_PWT | _PAGE_PROTNONE); 
	//prot |= ( _PAGE_PROTNONE); 
	printk("prot=%llx\n",prot);
	if (size > mem_size *1024 * 1024)
	{
		printk("size too big\n");
		return ( - ENXIO);
	}
	printk("offset = %x,size=%x\n",offset,size);
	offset = offset + mem_start * 1024 * 1024;

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
	if (remap_pfn_range(vma, vma->vm_start, offset>>PAGE_SHIFT, size,__pgprot(prot)))
	{
		printk("remap page range failed\n");
		return - ENXIO;
	}
	printk(" keep mem mmaped\n");
	return (0);
}

//module_init(init_module);
//module_exit(cleanup_module);
