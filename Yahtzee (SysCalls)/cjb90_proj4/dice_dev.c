//Preprocessors
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/random.h>
#include <asm/uaccess.h>

/* read_byte is the function called when a process calls read() on
 * /dev/dice.  It writes a random byte to the buffer passed in the
 * read() call.*/
 
static ssize_t read_byte(struct file * file, char * buf, size_t count, loff_t *ppos) {
    char random_byte[10] = "";
    char* temp;
    
    get_random_bytes(&temp, sizeof(char));
    if(temp < 0)
    	temp = temp*-1;
    temp = (temp%6) + 1;
    temp = temp + 48;
    
    printk("[Random Byte = %c]\n",temp);
    
    random_byte[0] = temp;
    random_byte[1] = '\0';
    
	if (copy_to_user(buf, random_byte, len))
		return -1;
		
	*ppos = count;
    
    return count;
}



//The only file operation we care about is read.

static const struct file_operations dice_fops = {
	.owner		= THIS_MODULE,
	.read		= read_byte,
};

static struct miscdevice dice_dev = {
	/*
	 * We don't care what minor number we end up with, so tell the
	 * kernel to just pick one.
	 */
	MISC_DYNAMIC_MINOR,
	/*
	 * Name ourselves /dev/dice.
	 */
	"dice",
	/*
	 * What functions to call when a program performs file
	 * operations on the device.
	 */
	&dice_fops
};

static int __init
dice_init(void)
{
	int ret;

	/*
	 * Create the "dice" device in the /sys/class/misc directory.
	 * Udev will automatically create the /dev/dice device using
	 * the default rules.
	 */
	ret = misc_register(&dice_dev);
	if (ret)
		printk(KERN_ERR "Unable to register \"Hello, world!\" misc device\n");

	return ret;
}

module_init(dice_init);

static void __exit
dice_exit(void)
{
	misc_deregister(&dice_dev);
}

module_exit(dice_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Christian Boni <cjb90@pitt.edu>");
MODULE_DESCRIPTION("Return random byte minimal module");
MODULE_VERSION("dev");
