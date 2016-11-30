#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>

#include "bm_net.h"
#include "bm_client_ui.h"

int main(int argc,char* argv[])
{
	int sockfd;
	int running = 1;
	socklen_t len;
	struct sockaddr_in address;
	int result;
	char ch[128] = "hi, servant\n";
	char opt ='\0';
	BM_ACNT acnt;

	if(argc != 3)
	{
		printf("too few args\nUsage: %s\
				[IP_address] [port_number]\n参数不足，用法： %s [服务器IP] [端口号]\n ",argv[0],argv[0]);
		return -1;
	}

	//创建客户端套接字
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	//指定服务器
	address.sin_family = AF_INET;
	//	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	//	address.sin_port = 9734;
	address.sin_addr.s_addr = inet_addr(argv[1]);
	address.sin_port =htons( atoi(argv[2]));
	len = sizeof(address);
	//	printf("%d--sockfd\n",sockfd);
	result = connect(sockfd,(struct sockaddr *)&address,len);//请求连接
	if(result == -1)
	{
		perror("oops:client3");
		exit(1);
	}

	//	system("clear");
	printf("\033[K\033[%dm欢迎！！！！\n",32);
	sleep(1);
	
	memset(&acnt,'\0',sizeof(BM_ACNT));

	while(running)
	{
		// load_cart

		/* 打印主菜单 */
		show_mainmenu();
		//	scanf("%c",&opt);
		opt = getchar();
		while(getchar() != '\n')continue;

		switch(opt)
		{
			case '0':
				running = 0;
				//				printf("opt %c\n",opt);
				printf("\033[%d;%dH\033[K\033[7m0.退出\n\033[0m",6,25);
				sleep(1);

				break;
			case '1':
				printf("\033[%d;%dH\033[K\033[7m1.浏览商品\n\033[0m",3,25);
				sleep(1);
				//	view_hotlist();
				show_hotlist(sockfd);
				break;
			case '2':
				//	client_login();
				printf("\033[%d;%dH\033[K\033[7m2.登录\n\033[0m",4,25);
				sleep(1);
				if(0 == client_login(sockfd, &acnt))
				{
					//
					printf("personal\n");
				}
				else
				{
				//	printf("返回\n");
				//	sleep(1);
					continue;
				}

				break;
			case '3':
				//	client_signup()
				printf("\033[%d;%dH\033[K\033[7m3.申请帐号\n\033[0m",5,25);
				sleep(1);
				//			client_signup(sockfd,&acnt);
				if(0 == client_signup(sockfd, &acnt))
				{
					//show post login page
			//		printf("用户信息待完善\n");
					sleep(1)	;
				} 
				break;
			default:
				printf("没想好？随便逛逛吧? y/n :\n");
				//	scanf("%c",&opt);
				opt = getchar();
				while(getchar() != '\n') continue;
				if(opt == 'y'){


					//view_hotlist()
					printf("opt %c",opt);
				}	
				else if(opt == 'n')
				{
					/*
					   printf("输入序号并回车进行选择\n");
					   printf(">---------------------<\n"); 
					   printf("     1.随便逛逛\n     2.登录\n     3.申请帐号\n     0.退出\n");
					   printf(">---------------------<\n");
					 */
					printf("opt %c",opt);
					//	continue;

				}
		}

	}
	//读写
	//	write(sockfd,ch,strlen(ch));
	//	memset(ch,'\0',128);
	//	read(sockfd,ch,127);
	//	printf("string from server = %s\n",ch);
	close(sockfd);

	exit(0);


}

