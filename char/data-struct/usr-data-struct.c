#include<stdio.h>
#include<sys/ioctl.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include<signal.h>
#include"ioctl-magic.h"

#define BUFFSIZE 6

//===================Test stub for testing the character driver=================
int main(void){
int file_desc;
char read_buff[BUFFSIZE];
char *input = "abadefahikl";
char *c = "a";
const char *file_path ="/dev/link_list";

	printf("\nStarting\n ");

	if((file_desc = open(file_path,O_RDWR)) < 0){
		perror("\nFile Failed to open\n");
	}
	printf("\nFile: %s was succuessfully opened\n ",file_path);

printf("\n========printing=================\n");
if(ioctl(file_desc,PRINT_LIST)){
	perror("Failed to use IOCTL");
}

printf("\n========deleting=================\n");
if(ioctl(file_desc,DEL_NODE,c)){
 perror("Failed to use IOCTL");
}

printf("\n========Adding node to link_list=================\n");
int i = 0;
while(i < 6){
 if(ioctl(file_desc,ADD_NODE,&input[i])){
		perror("Failed to use IOCTL");
 }
 i++;
}

printf("\n========printing=================\n");
if(ioctl(file_desc,PRINT_LIST)){
	perror("Failed to use IOCTL");
}

printf("\nPerforming read operation on %s\n",file_path);
if((read(file_desc,&read_buff,sizeof(read_buff)))<0){
	perror("Failed to read");
}

printf("%s\n",read_buff);

printf("\n========Deleting single char %s =================\n", c);
if(ioctl(file_desc,DEL_NODE,c)){
 perror("Failed to use IOCTL");
 }

printf("\n========printing=================\n");
if(ioctl(file_desc,PRINT_LIST)){
 	perror("Failed to use IOCTL");
}

printf("\n========Deleting All=================\n");
i = 0;
while(i < 6){
if(ioctl(file_desc,DEL_NODE,&input[i])){
 perror("Failed to use IOCTL");
 }
i++;
}

printf("\n========printing=================\n");
if(ioctl(file_desc,PRINT_LIST)){
 perror("Failed to use IOCTL");
}

close(file_desc);
printf("\nFile: %s was succuessfully closed\n ",file_path);

return 0;
}
