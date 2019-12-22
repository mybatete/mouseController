
#include <linux/kernel.h>
#include <linux/keyboard.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/input.h>

MODULE_LICENSE("GPL");
struct input_dev *jerry;


int n = 3;
int kb_notifier_fn(struct notifier_block *kb, unsigned long action, void *data){
	struct keyboard_notifier_param *kp = data;
	printk("val %x", kp->value);	 
	switch(kp->value){
		case 0x67:
			input_report_rel(jerry, REL_Y, -n);
			break;
		case 0x6c:
			input_report_rel(jerry, REL_Y, n);
			break;
		case 0x6a:
			input_report_rel(jerry, REL_X, n);
			break;
		case 0x69:
			input_report_rel(jerry, REL_X, -n);
			break;
		case 0x39: 
			if(kp->down)
				input_report_key(jerry, BTN_LEFT, n);
			else
				input_report_key(jerry, BTN_LEFT, 0);
		break;
	}
	input_sync(jerry);

	return NOTIFY_DONE;	

}



static struct notifier_block nb = {
	.notifier_call = kb_notifier_fn,
};


static int __init mm_init(void)
{
	jerry = input_allocate_device();
	register_keyboard_notifier(&nb);
	jerry->name = "jerry";
	set_bit(EV_REL, jerry->evbit);
	set_bit(REL_X, jerry->relbit);
	set_bit(REL_Y, jerry->relbit);

	set_bit(EV_KEY, jerry->evbit);
	set_bit(BTN_LEFT, jerry->keybit);
	
	input_register_device(jerry);
	return 0;
}

static void __exit mm_remove(void)
{
	unregister_keyboard_notifier(&nb);
	input_unregister_device(jerry);
}

module_init(mm_init);
module_exit(mm_remove);
