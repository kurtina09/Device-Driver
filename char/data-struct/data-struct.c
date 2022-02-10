#include "data-struct.h"

struct file_operations fops_link_list = {
	.read = link_list_read,
	.unlocked_ioctl = link_list_ioctl,
	.open = link_list_open,
	.release = link_list_release
};

int major;

int __init data_struct_init(void){
  major = register_chrdev(0, DEVICE_NAME, &fops_link_list);

	if (major < 0) {
	  printk(KERN_ALERT "Registering char device link_list failed with %d\n", major);
	  return major;
  }

	printk(KERN_INFO " link_list major number %d ", major);
	printk(KERN_INFO "'mknod /dev/%s c %d 0'.\n", DEVICE_NAME, major);

	return SUCCESS;
}

void __exit data_struct_exit(void){
	 unregister_chrdev(major, DEVICE_NAME);
}

module_init(data_struct_init);
module_exit(data_struct_exit);
