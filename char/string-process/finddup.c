
#include <linux/ioctl.h>
#include "string-process.h"

char data;
char cache=1;
int flag = 0;

//ioctl
long  finddup_ioctl(struct file *filp,unsigned int cmd, unsigned long arg){
	int retval;
	switch(cmd) {
		case FINDDUP_NOP:
									printk(KERN_ALERT"You have given the NOP command\n");
									return 0;
									break;

		case FINDDUP_RD:
									if(!data) {
										return -EINVAL;
									}
									if(flag) {
										data = '@';
										flag = 0;
										printk(KERN_ALERT"Reading");
									}
									retval=put_user(data,(char *)arg);
									printk(KERN_ALERT"Reading Data.........%c\n",data);
									break;

		case FINDDUP_WR:
									retval=	get_user(data,(char *)arg);
									if(cache == data) {
										flag = 1;
										printk(KERN_ALERT"cache == data");
									}
									else {
										cache = data;
									}
									printk(KERN_ALERT"Writing Data..........%c\n",data);
									break;
		default:
									return -ENOTTY;
  }

	return retval;
}

//open
int finddup_open(struct inode *ip,struct file *fmychar) {
	unsigned int num;
	num = iminor(ip);
	printk(KERN_ALERT" I am in driver open %d", num);
	return 0;
}

//release
int finddup_release(struct inode *ip,struct file *fmychar) {
	unsigned int num;
	num = iminor(ip);
	printk(KERN_ALERT" I am in driver release %d", num);
	return 0;
}
