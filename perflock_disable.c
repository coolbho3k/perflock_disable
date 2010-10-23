/*
 * Copyright (c) 2010 Michael Huang
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>

#define DRIVER_AUTHOR "Michael Huang <coolbho3000@gmail.com>"
#define DRIVER_DESCRIPTION "Setcpu perflock driver disabler"
#define DRIVER_VERSION "1.1"

MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESCRIPTION);
MODULE_VERSION(DRIVER_VERSION);
MODULE_LICENSE("GPL");

static uint perflock_notifier_call_addr = 0x0;

module_param(perflock_notifier_call_addr, uint, 0444);
MODULE_PARM_DESC(perflock_notifier_call_addr, "Memory address of perflock_notifier_call");

static char code[] =
		"\x0D\xC0\xA0\xE1" //mov r12, sp
		"\x00\xD8\x2D\xE9" //stmdb sp!, {r11, r12, lr, pc}
		"\x04\xB0\x4C\xE2" //sub r11, r12, #4
		"\x00\x00\xA0\xE3" //mov r0, #0
		"\x00\xA8\x9D\xE8" //ldmia sp, {r11, sp, pc}
		;

static int __init perflock_disable_init(void)
{
	printk(KERN_INFO "setcpu: %s version %s\n", DRIVER_DESCRIPTION, DRIVER_VERSION);
	printk(KERN_INFO "setcpu: by %s\n", DRIVER_AUTHOR);
	
	if(perflock_notifier_call_addr != 0x0)
	{	
		unsigned char *func;
		int i;
		for(i = 0; i <= 19; i++)
		{
			func = (char *)perflock_notifier_call_addr+i;
			*func = code[i];
		}
	}

	return 0;
}

static void __exit perflock_disable_exit(void)
{
	printk(KERN_INFO "setcpu: perflock disabler module unloaded\n");
}

module_init(perflock_disable_init);
module_exit(perflock_disable_exit);
