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
	{ 0x6bc3fbc0, "__unregister_chrdev" },
	{ 0xfa0d49c7, "__register_chrdev" },
	{ 0xea147363, "printk" },
	{ 0xb4390f9a, "mcount" },
	{ 0x8a7d1c31, "high_memory" },
	{ 0xe52947e7, "__phys_addr" },
	{ 0xc911f7f0, "remap_pfn_range" },
	{ 0xedc03953, "iounmap" },
	{ 0x68c7263, "ioremap_cache" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "9F0EBBC7D91CF462C75B0E6");

static const struct rheldata _rheldata __used
__attribute__((section(".rheldata"))) = {
	.rhel_major = 6,
	.rhel_minor = 5,
};
