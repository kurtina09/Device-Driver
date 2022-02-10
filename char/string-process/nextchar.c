#include "string-process.h"

char temp;
static int actual_count=0;


//read
ssize_t read_nextchar(struct file* filptr, char __user *buffer, size_t count, loff_t *offset){
int result;

 result = copy_to_user(buffer,&temp,(int)count);
  if(result>=0){
	printk(KERN_INFO"Retrun value of copy to user %d\n",result);
	printk(KERN_INFO"successfully data sent to user space of is %c of size %d\n",temp,(int)count);
	return count;
	}
	else{
	printk(KERN_ERR"failed to read................");
	return -EFAULT;
	}
}

//write
ssize_t write_nextchar(struct file* filptr, const char __user *buffer, size_t count, loff_t *offset){
int result;

result = copy_from_user(&temp, buffer,count);

	if(result>=0){
	printk(KERN_INFO"Retrun value of copy from user %d\n",result);
	printk(KERN_INFO"successfully got data from the userspace is %c requested %d\n",temp,(int)count);
	temp = temp+1;
	printk(KERN_INFO"%c",temp);
	actual_count = count-result;
	return count;
	}
	else{
	printk(KERN_ERR"failed to read");
	return -EFAULT;
	}
}

//open
int open_nextchar(struct inode* inodeptr , struct file* filptr){

unsigned int num;
num = iminor(inodeptr);
printk(KERN_ALERT"I AM IN OPEN %d \n",num );

	printk(KERN_INFO"Device opened Successfully");

return 0;
}

//release
int release_nextchar(struct inode* inodeptr, struct file* filptr){
	printk(KERN_INFO "Device Closed Successfully" );

return 0;
}
