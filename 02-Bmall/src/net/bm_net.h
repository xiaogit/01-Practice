/*
** 服务端和客户端消息通讯约定
*/


#ifndef  BM_NET_H_
#define BM_NET_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>


#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <mysql/mysql.h>

enum bm_msg_types
{
	TYPE_END_OF_PACKS = 0,TYPE_USR, TYPE_BOOK, TYPE_CART,TYPE_ORDER,TYPE_HOTLIST,TYPE_LOGIN,TYPE_SIGNUP_CHECK,TYPE_SIGNUP_PROFILE, TYPE_ADMIN_SHUTDOWN
};

typedef  struct bm_msg
{
	int fd; //I/O ID
//	MYSQL * pdb;
	char package[BUFSIZ];//消息头，消息体
	struct bm_msg * next;
}BM_MSG;

typedef struct msg_head
{
	int len;
	enum bm_msg_types msgtype;
}BM_MSGHEAD;

enum consumer_states
{
	STATE_OFFLINE = 0, STATE_ONLINE
};
typedef struct consumer{
	enum consumer_states state;
	char name[32];
}CONSUMER_INFO;
int setnonblocking(int sockfd);

#endif /* BM_NET_H_ */
