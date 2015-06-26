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
};

static const struct modversion_info ____versions[]
__attribute_used__
__attribute__((section("__versions"))) = {
	{ 0xf8e3dbd2, "struct_module" },
	{ 0x3762cb6e, "ioremap_nocache" },
	{ 0x8a7d1c31, "high_memory" },
	{ 0xeb0ece80, "register_chrdev" },
	{ 0xc192d491, "unregister_chrdev" },
	{ 0xedc03953, "iounmap" },
	{ 0xec5f438f, "remap_pfn_range" },
	{ 0xdd132261, "printk" },
};

static const char __module_depends[]
__attribute_used__
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "53CFD921E331FAC8DA72F42");
