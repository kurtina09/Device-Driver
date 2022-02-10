#include<stdio.h>
#include<sys/ioctl.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include "ioctl-magic.h"


int test_uppercase(void) {
	int fp;
	char buffer[] = "hello from userspace";
	char buffer1[sizeof(buffer)];

	fp = open("/dev/uppercase", O_RDWR);
	if(fp <0) {
		printf("failed to open uppercase file\n");
    return -1;
	}
	printf("Check the dmesg for successful file opening\n");
	write(fp,buffer, sizeof(buffer));
	lseek(fp,0,SEEK_SET);
	read(fp,buffer1,sizeof(buffer1));
	printf("%s\n",buffer1);
	close(fp);
	printf("File closed check dmesg\n");

return 0;
}

int test_nextchar(void) {
  char temp;
	int fd = open("/dev/nextchar",O_RDWR);
  if(fd<0) {
		printf("Failed to nextchar file\n");
		return -1;
	}

	if(write(fd,"A", 1) < 0) {
		printf("Failed to write file\n");
		exit(1);
  }

	lseek(fd,0,SEEK_SET);
  if(read(fd,&temp, 1) < 0) {
		printf("Failed to read file\n");
		return -1;
  }
	printf("Value from kernel space is %c\n",temp);
	close(fd);
return 0;
}

int test_finddup (void) {
  int fp,i=0;
  char c;
  int ret;

  fp = open("/dev/finddup",O_RDWR);
  FILE *fp2 =fopen("test.txt","r");

  if(fp<0 && fp2<0) {
    perror("unable to open on of the file\n");
    return -1;
  }
  else {
    printf("both file opened\n");
    ioctl(fp,FINDDUP_WR,&c);
  }

  while((c = fgetc(fp2)) != EOF) {
    ioctl(fp,FINDDUP_RD,&c);
    printf("%c",c);
  }
  printf("\n");
  close(fp);
  fclose(fp2);

return 0;
}


int main(void) {
  printf("Staring the String Processing driver test\n");
  printf("Testing UPPERCASE driver\n");
  if(test_uppercase() == -1){
    printf("Staring the String Processing : uppercase driver test: FAILED\n");
  }
  printf("Testing NEXTCHAR driver\n");
  if(test_nextchar() == -1){
    printf("Staring the String Processing : nextchar driver test: FAILED\n");
  }
  printf("Testing FINDDUP driver\n");
  if(test_finddup() == -1){
    printf("Staring the String Processing : finddup driver test: FAILED\n");
  }
return 0;
}
