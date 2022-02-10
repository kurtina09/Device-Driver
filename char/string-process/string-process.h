#ifndef STRING_PROCESS_H
#define STRING_PROCESS_H
#include<linux/fs.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/uaccess.h>
#include<linux/ctype.h>

#define FINDDUP_MAGIC   'X'
#define FINDDUP_NOP _IO(FINDDUP_MAGIC,0)
#define FINDDUP_RD  _IOR(FINDDUP_MAGIC,1,int)
#define FINDDUP_WR  _IOW(FINDDUP_MAGIC,2,int)

#define SUCCESS 0

#define UPPERCASE "uppercase"
#define FINDDUP "finddup"
#define NEXTCHAR "nextchar"

extern struct file_operations uppercase;
extern struct file_operations nextchar;
extern struct file_operations finddup;

extern int uppercase_major;
extern int nextchar_major;
extern int finddup_major;

int open_uppercase(struct inode* inodeptr , struct file* filptr);
ssize_t read_uppercase(struct file* filptr, char __user *buffer, size_t count, loff_t *offset);
ssize_t write_uppercase(struct file* filptr, const char __user *buffer, size_t count, loff_t *offset);
int release_uppercase(struct inode* inodeptr, struct file* filptr);

ssize_t read_nextchar(struct file* filptr, char __user *buffer, size_t count, loff_t *offset);
ssize_t write_nextchar(struct file* filptr, const char __user *buffer, size_t count, loff_t *offset);
int open_nextchar(struct inode* inodeptr , struct file* filptr);
int release_nextchar(struct inode* inodeptr, struct file* filptr);

int finddup_open(struct inode *ip,struct file *fmychar);
long  finddup_ioctl(struct file *filp,unsigned int cmd, unsigned long arg);
int finddup_release(struct inode *ip,struct file *fmychar);



MODULE_AUTHOR("Vishal Verma");
MODULE_DESCRIPTION("Registering a string processing driver");
MODULE_LICENSE("GPL");
#endif
