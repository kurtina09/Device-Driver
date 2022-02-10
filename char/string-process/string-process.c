#include "string-process.h"

// file operations to give functionality to the driver
struct file_operations uppercase = {
	.open = open_uppercase,
	.release = release_uppercase,
	.read = read_uppercase,
	.write = write_uppercase,
};

struct file_operations nextchar = {
	.open = open_nextchar,
	.release = release_nextchar,
	.read = read_nextchar,
	.write = write_nextchar,
};


struct file_operations finddup={
	.unlocked_ioctl=finddup_ioctl,
	.open=finddup_open,
	.release=finddup_release,
};


int uppercase_major;
int finddup_major;
int nextchar_major;

int __init string_process_init(void) {
  uppercase_major = register_chrdev(0, UPPERCASE, &uppercase);
  finddup_major = register_chrdev(0, FINDDUP, &finddup);
  nextchar_major = register_chrdev(0, NEXTCHAR, &nextchar);

	if (uppercase_major < 0) {
	  printk(KERN_ALERT "Registering char device uppercase failed with %d\n", uppercase_major);
	  return uppercase_major;
  }
	if (nextchar_major < 0) {
	  printk(KERN_ALERT "Registering char device nextchar failed with %d\n", nextchar_major);
	  return nextchar_major;
  }
	if (finddup_major < 0) {
	  printk(KERN_ALERT "Registering char device finddup failed with %d\n", finddup_major);
	  return finddup_major;
  }

	printk(KERN_INFO " uppercase major number %d ", uppercase_major);
	printk(KERN_INFO "'mknod /dev/%s c %d 0'.\n", UPPERCASE, uppercase_major);
	printk(KERN_INFO " nextchar major number %d ", nextchar_major);
	printk(KERN_INFO "'mknod /dev/%s c %d 0'.\n", NEXTCHAR, nextchar_major);
	printk(KERN_INFO " finddup major number %d ", finddup_major);
	printk(KERN_INFO "'mknod /dev/%s c %d 0'.\n", FINDDUP, finddup_major);

	return SUCCESS;
}

void __exit string_process_exit(void){
	 unregister_chrdev(uppercase_major, UPPERCASE);
	 unregister_chrdev(nextchar_major, NEXTCHAR);
	 unregister_chrdev(finddup_major, FINDDUP);
}

module_init(string_process_init);
module_exit(string_process_exit);
