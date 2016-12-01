#include <termios.h>
#include <unistd.h>

#include "bm_client_ui.h"
#include "bm_net.h"

int show_hotlist(int sockfd)
{
	BM_MSG send;
	//	BM_MSG * queRear  = NULL;
	BM_MSG receive;
	BM_MSGHEAD mHead ;
	int reading = 1;
	int n =0;
	char text[32] = { 0 };
	char msg[1024]= { 0 };
	char opt = '\0';

	system("clear");
	printf("\033[31m热销榜单\033[0m\n");
	//	sleep(1);


	memset(&send,'\0',sizeof(BM_MSG));
	strcpy(text,"hotlist, no membership required");
	mHead.len = (sizeof(mHead)+ strlen(text));
	//	printf("len = %d\n",mHead.len);
	mHead.msgtype = TYPE_HOTLIST;
	mHead.len = htons(mHead.len);
	mHead.msgtype = htons(mHead.msgtype);

	memcpy(send.package, &mHead, sizeof(mHead));
	memcpy(send.package+sizeof(mHead),text,sizeof(text));

	n = write(sockfd,send.package,strlen(text)+sizeof(mHead));
	printf("%d  bytes written\n",n);


	//	while(reading)
	//	{
	memset(&receive,'\0',sizeof(receive));
	n = read(sockfd,receive.package,sizeof(BM_MSGHEAD));

	printf("%d bytes read\n",n);

	memset(&mHead,'\0', sizeof(BM_MSGHEAD));
	memcpy(&mHead,receive.package,sizeof(mHead));
	mHead.msgtype = ntohs(mHead.msgtype);
	mHead.len = ntohs(mHead.len);
	n += read(sockfd, receive.package+sizeof(BM_MSGHEAD), mHead.len - sizeof(BM_MSGHEAD));

	printf("msgtype = %d\n",mHead.msgtype);

	if(mHead.msgtype == TYPE_END_OF_PACKS)
	{
		reading = 0;
	}
	else if(mHead.msgtype = TYPE_HOTLIST){
		printf("%s\n",receive.package+sizeof(mHead));
	}

	printf("\n\033[32m键入数字并回车进行选择\n\033[33m");
	printf("1-购买 2-搜索 0-返回上一级\033[0m\n");
	
	opt = getchar();
	while(getchar() != '\n');

	switch(opt)
	{
		case '0':
		{
			//break;
			break;
		}
		case '1':
		{
			break;
		}
		case '2':
			break;
		default:
		{
			printf("该选项未定义,自动跳转\n");
		}

		//跳转
	}


	//	}
	//	n = read(sockfd,msg,1024);
	//	printf("n= %d\n",n);
	//	printf("%s\n",msg);
	//	printf("%s",msg+sizeof(mHead));
	//	printf("%s\n",receive.package+sizeof(BM_MSGHEAD));

	//	sleep(2); 
	printf("按回车(Enter)返回主菜单\n");
	//	getchar();
	getchar();

	//free(queFront);
	return 0;

}

void show_mainmenu()
{
	system("clear");
	//		printf("\033[%d;%dH\033[K\033[%dm%s is not found,press PageDn to update\n",1,0,30+(int)Color::cyan,wd.word().c_str());
	printf("\033[%d;%dH\033[K\033[%dm输入序号并回车进行选择\033[0m\n",1,20,34);
	printf("\033[%d;%dH\033[K\033[%dm>---------------------<\033[0m\n",2,20,34);
	printf("\033[%d;%dH\033[K1.浏览商品\n",3,25);
	printf("\033[%d;%dH\033[K2.登录\n",4,25);
	printf("\033[%d;%dH\033[K3.申请帐号\n",5,25);
	printf("\033[%d;%dH\033[K0.退出\n",6,25);
	//printf(">---------------------<\n");
	printf("\033[%d;%dH\033[K\033[%dm>---------------------<\033[0m\n",7,20,34);


}

//For example, we can use the following sequence to turn terminal echoing off:
/*
   struct termios tp;
   if (tcgetattr(STDIN_FILENO, &tp) == -1)
   errExit("tcgetattr");
   tp.c_lflag &= ~ECHO;
   if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &tp) == -1)
   errExit("tcsetattr");
 */
int turnoff_echo(struct termios *tp)
{
	struct termios new_tp;

	/* 关闭打字回显 */
	if(tcgetattr(0,tp) == -1)//0--fd---stdin,备份
	{
		perror("tcgetattr");
		return -1;
	}
	memcpy(&new_tp,tp,sizeof(struct termios));
	new_tp.c_lflag &= ~ECHO; //修改
	if(tcsetattr(0, TCSAFLUSH, &new_tp))//应用设定
	{
		perror("tcsetattr");
		return -1;
	}

	return 0;

}

int recover_echo(struct termios * tp)
{
	if(tcsetattr(0, TCSAFLUSH, tp))
	{
		perror("tcsetattr");
		return -1;
	}

	return 0;


}
int client_login(int sockfd, BM_ACNT *acnt)
{
	//short int cnt =0;
	int ret;
	char name[32],key[32];
	//char buf[32] ="abcd";
	struct termios tp;
	BM_MSG send;
	//	BM_MSG * queRear  = NULL;
	BM_MSG receive;
	BM_MSGHEAD mHead ;
	//	struct termios new_tp;	



	//printf("login\n");
	system("clear");

	if(acnt->state == LOGGED_IN)
	{
		printf("\n您已经登录\n");
		printf("当前用户名: %s\n自动跳转回主菜单\n",acnt->name);
		sleep(1);
		return -1;
	}

	printf("\033[%d;%dH\033[K\033[%dm>---------登录--------<\033[0m\n\n",1,2,34);
	printf("\033[%d;%dH\033[K 请输入用户名:",4,2);
	scanf("%s",name);

	printf("请输入密码, (1~16位数字字母组合): ");
	/* 关闭打字回显 */
	turnoff_echo(&tp);
	scanf("%s",key);

	strncpy(acnt->name, name, sizeof(name));
	strncpy(acnt->key, key, sizeof(key));

	memset(&send,'\0', sizeof(BM_MSG));
	memset(&mHead, '\0', sizeof( BM_MSGHEAD));
	mHead.msgtype = TYPE_LOGIN;
	mHead.msgtype = htons(mHead.msgtype);
	mHead.len = sizeof(BM_MSGHEAD) + sizeof(BM_ACNT);
	mHead.len = ntohs(mHead.len);

	memcpy(send.package, &mHead, sizeof(BM_MSGHEAD));

	memcpy(send.package+sizeof(BM_MSGHEAD), acnt, sizeof(BM_ACNT));
	write(sockfd, send.package, sizeof(mHead)+sizeof(BM_ACNT));

	read(sockfd,receive.package, sizeof(BM_MSGHEAD));

	memcpy(&mHead, receive.package, sizeof(mHead));
	mHead.msgtype = ntohs(mHead.msgtype);
	mHead.len = ntohs(mHead.len);

	read(sockfd, receive.package+sizeof(BM_MSGHEAD), mHead.len - sizeof(BM_MSGHEAD));
	memcpy(acnt, receive.package+sizeof(BM_MSGHEAD),sizeof(BM_ACNT));
	acnt->state = ntohs(acnt->state);



	if(mHead.msgtype == TYPE_LOGIN && acnt->state == LOGGED_IN )
	{

		printf("\n登录成功!\n");
		ret = 0;
	}
	else
	{
		printf("\n用户名或密码错误\n");
		ret = -1;
	}
	/*	if(tcgetattr(0,&tp) == -1)//0--fd---stdin
		{
		perror("tcgetattr");
		return -1;
		}
		memcpy(&new_tp,&tp,sizeof(tp));
		new_tp.c_lflag &= ~ECHO;
		if(tcsetattr(0, TCSAFLUSH, &new_tp))
		{
		perror("tcsetattr");
		return -1;
		}
	 */
	/*	while(cnt < 3)
		{

		printf("请输入密码, (1~16位数字字母组合): ");
		scanf("%s",key);

		if(strcmp(buf,key))
		{
		printf("\n\033[31m密码错误，请重试:\n\033[0m");
		cnt += 1;

		}
		else
		{
		printf("\n登录成功!\n");
		break;
	//	return 0;
	}

	}
	printf("\n\033[33m密码输入错误次数过多，请稍后重试\n\033[0m");
	 */
	while(getchar() != '\n') 
		;
	/* 恢复设置 */
	recover_echo(&tp);
	/*	if(tcsetattr(0, TCSAFLUSH, &tp))
		{
		perror("tcsetattr");
		return -1;
		}
	 */

	sleep(1);
	return ret;
}

int client_signup(int sockfd,BM_ACNT *acnt)
{
	struct termios tp;//用于保存当前终端设定
	char	key[32];
	char	buf[32];
	char	name[32];
	int ret;
	int bytes_read;
	int s = 0;
	int len = 0;
	BM_MSG send;
	//	BM_MSG * queRear  = NULL;
	BM_MSG receive;
	BM_MSGHEAD mHead ;
	BM_USR user;


	memset(name, '\0', 32);
	
	memset(&user, '\0', sizeof(BM_USR));

	system("clear");

	printf("\033[%d;%dH\033[K\033[%dm>---------注册--------<\033[0m\n\n",1,2,34);
	printf("\033[%d;%dH\033[K 请输入用户名，(数字和字母的组合):",4,2);
	scanf("%s",name);
//	printf("%s, checking singularity\n",name);
	//send name for checking

	//	printf("\033[%d;%dH\033[K 请输入用户名，(数字和字母的组合):",4,25);

	/* 关闭打字回显 */
	turnoff_echo(&tp);
	/*
	   if(tcgetattr(0,&tp) == -1)//0--fd---stdin
	   {
	   perror("tcgetattr");
	   return -1;
	   }
	   memcpy(&new_tp,&tp,sizeof(tp));
	   new_tp.c_lflag &= ~ECHO;
	   if(tcsetattr(0, TCSAFLUSH, &new_tp))
	   {
	   perror("tcsetattr");
	   return -1;
	   }
	 */
	while(1)
	{ 
		memset(key, '\0', 32);
		memset(buf, '\0', 32);
		printf("请输入密码, (1~16位数字字母组合): ");
		bytes_read = scanf("%s",key);

		printf("\n请再次输入密码: ");
		scanf("%s",buf);

		if( strncmp(key, buf, bytes_read) == 0)
		{
			//		printf("恭喜,%s,您已注册成功!\n",name);
			//		printf("%s 请保管好密码\n",buf);
			break;
		}
		else
		{
			printf("密码错误,请重新输入\n");
			printf("%s\n %s\n",key, buf);
			continue;
		}
	}

	/* 恢复输入回显 */
	recover_echo(&tp);

	strncpy(acnt->name, name, strlen(name));
	strncpy(acnt->key, key, strlen(key));
	//	memcpy(send.package+sizeof(mHead), acnt, sizeof(BM_ACNT));

	mHead.msgtype = TYPE_SIGNUP_CHECK;
	mHead.msgtype = htons(mHead.msgtype);
	len = sizeof(BM_ACNT) + sizeof(BM_MSGHEAD);
	mHead.len = htons(len);

	memcpy(send.package, &mHead, sizeof( BM_MSGHEAD));
	memcpy(send.package+sizeof(BM_MSGHEAD), acnt, sizeof(BM_ACNT));

 	s = write(sockfd, send.package, len);
//	printf("\n%d bytes sent\n",s);

	len = sizeof(BM_MSGHEAD) + sizeof(BM_ACNT);
	read(sockfd, receive.package, len);
	memcpy(&mHead, receive.package, sizeof(BM_MSGHEAD));
	memcpy(acnt, receive.package+sizeof(BM_MSGHEAD),sizeof(BM_ACNT));

	mHead.msgtype = ntohs(mHead.msgtype);
	acnt->state = ntohs(acnt->state);

	printf("\nstate = %d, name = %s, key = %s\n", acnt->state, acnt->name, acnt->key);
	if(mHead.msgtype == TYPE_SIGNUP_CHECK)
	{
		if(acnt->state == ACNT_STATE_REGISTERED)
		{
			printf("\033[34m抱歉，名字: %s 已经被注册，请换个名字重新注册\n\033", name);
			//sleep(2);
			while(getchar() != '\n') ;
			printf("按回车返回上一级\n");
			//getchar();
			while(getchar() != '\n') ;
			return -1;	
		}else
		{
			memset(&user, '\0', sizeof(BM_USR));
			printf("\n请输入您的电子邮箱:");
			scanf("%s",user.email);

			printf("请输入你的性别,FM代表女，M代表男 : ");
			scanf("%s",user.gender);

			printf("请输入您的生日，格式: yyyy-mm-dd,如 1990-09-01):");
			scanf("%s",user.birthday);
			printf("请输入您的手机号:");
			scanf("%s",user.cellphone);
			getchar();
			printf("请输入您的地址:");
//			scanf("%s",user.address);
			fgets(user.address,BM_EM_MAX-1, stdin);

			memset(user.address+strlen(user.address)-1,'\0', 1);

			printf("%s\n%s\n%s\n%s\n%s\n",user.email,user.gender,user.birthday, user.cellphone,user.address);
	//		sleep(1);
//			getchar();

		}
	}
	else
	{
		printf("通讯错误\n");
		return -2;
	}

	mHead.msgtype = htons(TYPE_SIGNUP_PROFILE);
	memcpy(&(user.acnt), acnt, sizeof( BM_ACNT));
	len = sizeof(BM_MSGHEAD) + sizeof(BM_USR);
	mHead.len = htons(len);

	memset(&send, '\0', sizeof( BM_MSG));
	memcpy(send.package, &mHead, sizeof(BM_MSGHEAD));
	memcpy(send.package+sizeof(BM_MSGHEAD), &user, sizeof( BM_USR));

	bytes_read = write(sockfd, send.package, len);
//	printf("%d, BUFSIZ = %d",bytes_read, BUFSIZ);

	memset(&receive, '\0', sizeof( BM_MSG));
	read(sockfd, receive.package, sizeof(BM_MSGHEAD)+sizeof(BM_ACNT));

	memcpy(&mHead, receive.package, sizeof(BM_MSGHEAD));
	memcpy(acnt, receive.package+sizeof(BM_MSGHEAD), sizeof(BM_ACNT));
	mHead.msgtype = ntohs(mHead.msgtype);

//	printf("mHead.msgtype = %d", mHead.msgtype);
//	sleep(1);

	acnt->state = ntohs(acnt->state);
	if(mHead.msgtype == TYPE_SIGNUP_PROFILE)
	{
		switch(acnt->state)
		{
			case LOGGED_IN:
				{
					printf("\n恭喜,注册成功\n");
					printf("用户名: %s\n",acnt-> name);
					printf("密码: %s\n",acnt-> key);
			//		printf("stzte = %d\n", acnt->state);


					ret = 0;
					break;
				}
			case ACNT_STATE_REGISTERED:
				{
					printf("用户名: %s 已经被注册过，请使用其他名字\n",name);
					ret = -3;
					break;
				}
			default:
				{
					printf("\n注册失败\n");
					ret = -4;
				}
		}
	}
	/*
	   if(tcsetattr(0, TCSAFLUSH, &tp))
	   {
	   perror("tcsetattr");
	   return -1;
	   }
	 */

	//	printf("进入书城\n");
	printf("按回车回上一级\n");
	getchar();
	//	scanf("%s",buf);

	return ret;
}

