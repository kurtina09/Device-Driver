#ifndef DATA_STRUCT_H
#define DATA_STRUCT_H

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

#define SUCCESS 0
#define FAILURE -1
#define DEVICE_NAME "link_list"

#define LINK_LIST_MAGIC   'X'
#define DEL_NODE     _IOW(LINK_LIST_MAGIC, 0, char)
#define PRINT_LIST   _IO(LINK_LIST_MAGIC, 1)
#define ADD_NODE     _IOW(LINK_LIST_MAGIC, 2, char)


int __init data_struct_init(void);
void __exit data_struct_exit(void);

int link_list_open(struct inode *, struct file *);
int link_list_release(struct inode *, struct file *);
ssize_t link_list_read(struct file *, char *, size_t, loff_t *);
long  link_list_ioctl(struct file *filp, unsigned int cmd, unsigned long arg);

extern struct file_operations fops_link_list;

extern int major;

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Vishal Verma");
MODULE_DESCRIPTION("Single Character Driver for Data Structures");
#endif
