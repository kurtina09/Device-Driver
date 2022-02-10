#include "string-process.h"

#define BUFFERSIZE 21
char kernelbuffer[BUFFERSIZE];
static int actual_count=0;

//read
ssize_t read_uppercase(struct file* filptr, char __user *buffer, size_t count, loff_t *offset){
	int result;

	result = copy_to_user(buffer,kernelbuffer,count);
	if(result>=0){
		printk(KERN_INFO"Return value of copy to user  is %d \n",result);
		printk(KERN_INFO"successfully data sent to user space %s of size %d\n ",kernelbuffer,(int)count);
		return count;
	}
	else{
		printk(KERN_ERR"failed to read................");
		return -EFAULT;
	}
}

//write
ssize_t write_uppercase(struct file* filptr, const char __user *buffer, size_t count, loff_t *offset){
	int result,i,len;
	memset(kernelbuffer,0,BUFFERSIZE);

	result = copy_from_user(kernelbuffer, buffer,(int)count);

	if(result>=0){
		printk(KERN_INFO"Return value of copy from user  is %d \n",result);
		printk(KERN_INFO"data from  user space is %s of size %d \n",kernelbuffer,(int)count);
		i=0;
		len = (int)count;

		while(i<len){
			if(isspace(kernelbuffer[i])){
				printk(KERN_INFO"I AM HERE");
			}
			else{
				kernelbuffer[i]= kernelbuffer[i]-32;
				//printk(KERN_INFO"%c",kernelbuffer[i]);
			}
			i++;
		}
		printk(KERN_INFO"%s",kernelbuffer);
		actual_count = count-result;
		return count;
	}
	else{
		printk(KERN_ERR"failed to read");
		return -EFAULT;
	}
}

//open
int open_uppercase(struct inode* inodeptr , struct file* filptr){
	unsigned int num;
	num = iminor(inodeptr);
	printk(KERN_ALERT"I AM IN OPEN %d \n",num );
	printk(KERN_INFO"Device opened Successfully");

	return 0;
}

//release
int release_uppercase(struct inode* inodeptr, struct file* filptr){
	printk(KERN_INFO "Device Closed Successfully" );
	return 0;
}
