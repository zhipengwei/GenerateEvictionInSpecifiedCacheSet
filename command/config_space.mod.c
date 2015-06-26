#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
 .arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x14522340, "module_layout" },
	{ 0x42e80c19, "cdev_del" },
	{ 0xc45a9f63, "cdev_init" },
	{ 0x6980fe91, "param_get_int" },
	{ 0xd8e484f0, "register_chrdev_region" },
	{ 0x7485e15e, "unregister_chrdev_region" },
	{ 0xff964b25, "param_set_int" },
	{ 0xea147363, "printk" },
	{ 0xb4390f9a, "mcount" },
	{ 0xa6d1bdca, "cdev_add" },
	{ 0x42c8de35, "ioremap_nocache" },
	{ 0xc911f7f0, "remap_pfn_range" },
	{ 0xedc03953, "iounmap" },
	{ 0x29537c9e, "alloc_chrdev_region" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "A5D5048FFAFFA1DB0DAA6D9");

static const struct rheldata _rheldata __used
__attribute__((section(".rheldata"))) = {
	.rhel_major = 6,
	.rhel_minor = 5,
};
