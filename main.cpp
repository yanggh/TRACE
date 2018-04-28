#include "unistd.h"
#include <cstdio>
#include <thread>

#include "bz_ctl_test.h"

void pth_all_hd(int  fd)
{
	int nread;
	char buff[256]= {0};

	while (1) //循环读取数据    
	{      
		nread = read(fd, buff,99);    
		if(nread > 0)    
		{    
			buff[nread] = '\0';
			printf("receive data is %s\n",buff);
			printf("nread = %d\n",nread);
		} 
		else
		{
			printf("cannot receive data\n");
		}
		sleep(2);
	}
}

void pth_bz_hd(int fd)
{
	int nread;
	char buff[256]= {0};

	while (1) //循环读取数据    
	{      
		nread = read(fd, buff,99);    
		if(nread > 0)    
		{    
			buff[nread] = '\0';
			printf("receive data is %s\n",buff);
			printf("nread = %d\n",nread);
		} 
		else
		{
			printf("cannot receive data\n");
		}
		sleep(2);
	}
}

int main(int argc, char** argv)
{
	int i = 0;	
	int fd = -1;

	if((fd = open_port(fd,1))<0)
	{
		printf("open_port error\n");
		return 0;
	}

	if((i = set_opt(fd,115200,8,'N',1))<0){
		printf("set_opt error\n");
		return 0;
	}
	
	std::thread t1(pth_all_hd, fd);
	std::thread t2(pth_bz_hd, fd);

	t1.join();
	t2.join();

	close(fd);
	return 0;
}
