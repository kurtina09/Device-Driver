#include<linux/ioctl.h>
#include <linux/list.h>
#include "data-struct.h"

static int device_open = 0;
static int num_nodes;

struct node{
	char data;
	struct list_head list;
};

static LIST_HEAD(link_list);

static int add_node_link_list(char *data){
  struct node *temp;
	// allocate the copied data to link list
	if(!data)
		return FAILURE;

	temp = kmalloc(sizeof(struct node), GFP_KERNEL);
	if (!temp)
    return -ENOMEM;

	temp->data = *data;
	list_add_tail(&temp->list, &link_list);

	num_nodes++;

	printk(KERN_INFO "Node added in link list: data -> %c,  num_of_nodes:%d",temp->data, num_nodes);

	return SUCCESS;
}


static int delete_node_link_list( char* data){
  struct node *temp;
  int notfound;
	notfound = 1;
	if (!data)
		return FAILURE;

	list_for_each_entry(temp, &link_list, list) {
		  if (temp->data  == *data){
					printk(KERN_INFO "data ->: %c Found: Deleting ",temp->data);
				  list_del(&temp->list);
					kfree(temp);
					num_nodes--;
					printk(KERN_INFO "Entry deleted successfully");
					notfound = 0;
					break ;
      }
		}

	if(notfound){
			printk(KERN_INFO "Not Found");
			return FAILURE;
    }
	return SUCCESS;
}

static int copy_link_list_to_buff(char *data_frame, int size){
	struct node *temp;
  int num_bytes_copied;
	num_bytes_copied = 0;
	if (!data_frame)
		return FAILURE;

	list_for_each_entry(temp, &link_list, list) {

		if (num_bytes_copied < size){
			printk(KERN_INFO "data ->: %c ",temp->data);
			*data_frame = temp->data;
			data_frame++;
		}
		else
			break;

		num_bytes_copied++;
	}
	return num_bytes_copied;
}

static void print_link_list(void){
	struct node *temp;
	list_for_each_entry(temp, &link_list, list) {
		printk(KERN_INFO "data ->: %c ",temp->data);
	}
}

static void clean_up_link_list(void){
	struct list_head *temp, *pos;
	struct node *t;
	list_for_each_safe(pos, temp , &link_list) {
		t = list_entry(pos, struct node, list);
		list_del(pos);
		kfree(t);
	}
	printk(KERN_INFO "All memory used for link list completed de-allocated\n");
}

ssize_t link_list_read(struct file *filp, char *buff, size_t length,
	loff_t * offset) {
	int res;
	int ret;
	int size;
	char *kernelbuff;
  res = 0;
	size = num_nodes;

	if(length>num_nodes)
		size = num_nodes;
  else
	  size = length;

	kernelbuff = kmalloc(size, GFP_KERNEL);
	if (!kernelbuff)
			return -ENOMEM;

	ret = copy_link_list_to_buff(kernelbuff,size);

	if((ret == FAILURE) || (size != ret)){
		printk(KERN_ERR"failed to populate to kernelbuff");
		return -EFAULT;
	}

	res = copy_to_user(buff,kernelbuff,size);
	kfree(kernelbuff);

	if(res>=0){
		 printk(KERN_INFO"Return value of copy to user  is %d \n",res);
		 printk(KERN_INFO"successfully data sent to user space of size %d\n",
		        size-res);
		 return res;
	}
	else {
		printk(KERN_ERR"failed to copy data to userspace");
		return -EFAULT;
	}

	return res;
}

long  link_list_ioctl(struct file *filp,unsigned int cmd, unsigned long arg){
	int retval;
	char data;
	retval = 0;
	switch(cmd){

		case ADD_NODE:
									retval = get_user(data,(char *)arg);
									printk(KERN_ALERT"Data from the userspace%c\n",data);
									retval = add_node_link_list(&data);
									break;

		case DEL_NODE:
		              retval = get_user(data,(char *)arg);
									printk(KERN_ALERT"Data from the userspace to delete %c",data);
									delete_node_link_list(&data);
								  break;

		case PRINT_LIST:
		              print_link_list();
									break;

		default:
									return -ENOTTY;
	}

	return retval;
}

int link_list_open(struct inode *inode, struct file *file){
	if (device_open)
		return -EBUSY;

	device_open++;
	return SUCCESS;
}

int link_list_release(struct inode *inode, struct file *file){
	//free all the kmalloc in the code
	clean_up_link_list();
	device_open--;
	return 0;
}
