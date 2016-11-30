/* socket服务端，epoll()进行I/O事件管理
 **  
 */

#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <mysql/mysql.h>

#include "bm_net.h"
#include "bm_services.h"

#define MAX_BUF BUFSIZ	/* Maximum bytes fetched by a single read */
#define MAX_EVENTS 3000	/* Maximum number of events to be returned from
			   a single epoll_wait() call */
#define BM_LISTENQUEUE_MAX 10000			   
void* serve_clients(void *arg)
{
	BM_MSG ** pFront = (BM_MSG **)arg;
	BM_MSG * pTemp = *pFront;
	BM_MSGHEAD mhead;
	//	char buf[BUFSIZ];
	//	int n;
	MYSQL db;
	MYSQL *conn_db = NULL;
	//MYSQL_ROW row = NULL;
	//	int row_count = -1;
	//	unsigned int num_fields;
	//	unsigned int i;
	//	unsigned long *lengths;
	//	MYSQL_RES * query_result = NULL;
	//	char sql_stmt[255]={ '\0' };


	mysql_init(&db);

	conn_db = mysql_real_connect(&db,"localhost","root","qwert","bMall",3306,NULL,0);
	if(! conn_db)
	{
		fprintf(stderr,"mysql_real_connect, Error: %s\n",mysql_error(&db));
		return NULL;
	}

	while(1)
	{
		pTemp = *pFront;
		if(pTemp == NULL)
		{
			continue;
		}
		else
		{
			memset(&mhead, '\0', sizeof(mhead));
			memcpy(&mhead, pTemp->package, sizeof(mhead));
			mhead.len = ntohs( mhead.len);
			mhead.msgtype = ntohs(mhead.msgtype);
			//memset(buf, '\0', BUFSIZ);
			printf("msgtype = %d, len = %d\n", mhead.msgtype, mhead.len);


			switch (mhead.msgtype)
			{
				case TYPE_HOTLIST:

					//write(pTemp,"results",7);

					/*	memset(sql_stmt,'\0',sizeof(sql_stmt));
						sprintf(sql_stmt,"set names utf8;");
						mysql_query(conn_db,sql_stmt);

						memset(sql_stmt,'\0',sizeof(sql_stmt));
						sprintf(sql_stmt,"select Books.BookID, Books.BookName, BookCategories.BookCategory,Books.Author,Books.UnitPrice, Books.CurrencyUnit,Books.Inventory,Books.SalesVolume from Books inner join BookCategories on Books.BookCategoryID = BookCategories.BookCategoryID order by Books.Inventory;");
						if(mysql_query(conn_db,sql_stmt))
						{
						fprintf(stderr,"Error: %s\n %s\n",sql_stmt,mysql_error(conn_db));
						break;
						}else
						{
						query_result = mysql_store_result(conn_db);
						if(query_result)
						{
						row_count = mysql_num_rows(query_result);
						num_fields = mysql_num_fields(query_result);
						memset(buf,'\0',BUFSIZ)	;

						while((row = mysql_fetch_row(query_result)))
						{ 
						sprintf(buf,"%s\n",buf);
						lengths = mysql_fetch_lengths(query_result);
						for(i = 0; i < num_fields; i++)
						sprintf(buf,"%s%s\t",buf, row[i] ? row[i]: "NULL");
						}
						sprintf(buf,"%s\n",buf);

						mysql_free_result(query_result);
						}
						memset(pTemp->package,'\0',sizeof(pTemp->package));				
						mhead.len = sizeof(mhead) + strlen(buf);
						mhead.len = htons(mhead.len);
						mhead.msgtype = TYPE_HOTLIST;
						mhead.msgtype = htons(mhead.msgtype);
						memcpy((pTemp->package),&mhead,sizeof(mhead));
						sprintf(pTemp->package+sizeof(mhead),"%s",buf);

						printf("send string%s",pTemp->package+sizeof(mhead));


						write(pTemp->fd,pTemp->package,sizeof(mhead)+strlen(buf));	

						}
					 */
					{


						on_type_hotlist(conn_db, pTemp);
						break;
					}
				case TYPE_LOGIN:
					{
						on_type_login(conn_db,pTemp);
						break;

					}
				case TYPE_SIGNUP_CHECK:
				{
					on_type_signup_check(conn_db,pTemp);
					break;
				}
				case TYPE_SIGNUP_PROFILE:
				{
					on_type_signup_profile(conn_db, pTemp);
					break;
				}

				default:
					write(pTemp->fd,"yes!", 4);


			} 
			//			write(pTemp->fd,"yes!", 4); 
			*pFront = pTemp->next;
			free(pTemp);
		}
	}
	mysql_close(conn_db); 
	return NULL;
}

int main(int argc,char *argv[])
{
	int server_sockfd,client_sockfd;
	struct sockaddr_in server_address;
	struct sockaddr_in client_address;
	socklen_t server_len = sizeof(server_address);
	socklen_t client_len = sizeof(client_address);

	int epfd, ready,  j, numopenFds;
	int s,fd;
	struct epoll_event ev;
	struct epoll_event evlist[MAX_EVENTS];
	char buf[MAX_BUF];

	pthread_t th_id;

	BM_MSG *queFront = NULL;
	BM_MSG *queEnd = NULL;
	BM_MSG *pTemp;
	BM_MSGHEAD mHead;

	if(argc != 2||strcmp(argv[1],"--help") == 0)
	{
		printf("Usage: %s portnumber\n用法： %s 端口号\n",argv[0],argv[0]);
		return -1;
	}
	printf("fghjk\n");

	/* 创建服务器socket */
	server_sockfd = socket(AF_INET,SOCK_STREAM,0);

	/* 绑定地址(naming) */
	memset(&server_address,'\0',sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(atoi(argv[1]));
	server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
	if(	bind(server_sockfd, (struct sockaddr *)&server_address, server_len))
	{
		perror("bind");
		exit(EXIT_FAILURE);
	}

	/* 创建socket监听队列 */
	if(listen(server_sockfd,BM_LISTENQUEUE_MAX) )
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}

	/* 将server_sockfd设置成非阻塞态 */
	//	setnonblocking(server_sockfd);

	/* 创建一个epoll对象 */
	epfd = epoll_create1(0);
	if(epfd == -1)
	{
		//	fprintf(stderr,"epoll_create");
		perror("error:epoll_create");
		return -1;
	}
	printf("epfd = %d\n",epfd);

	/* 让epfd管理server_sockfd的I/O事件 */
	ev.data.fd = server_sockfd;
	//	ev.events = EPOLLIN | EPOLLET;//server_sockfd 可读时通知，边沿触发
	ev.events = EPOLLIN;

	if(epoll_ctl(epfd,EPOLL_CTL_ADD,server_sockfd,&ev) == -1)	/* 注册 */
	{
		perror("epoll_ctl"); 
		printf("rtyuk errno %d\n",errno);
		exit(EXIT_FAILURE);
	} 

	/*	ev.data.fd = 0;
		ev.events = EPOLLIN;

		if(epoll_ctl(epfd,EPOLL_CTL_ADD,server_sockfd,&ev) == -1)	 
		{
		perror("epoll_ctl"); 
		printf("rtyuk errno %d\n",errno);
		exit(EXIT_FAILURE);
		} 
	 */

	/* Open each file on command line, and add it to the "interest list"
	   for the epoll instance--打开命令行输入的每个文件，将open()返回的
	   fd和关心的事件(EPOLLIN)填写到ev中, 调用epoll_ctl(),让epfd按照ev中
	   描述的内容,对指定的文件和事件进行监听 
	   for(j = 1;j < argc;j++)
	   {
	   fd = open(argv[j],O_RDONLY | O_NONBLOCK);
	   if (fd == -1)
	   {
	   fprintf(stderr,"open");
	   exit(EXIT_FAILURE);
	   }
	   printf("Opened \"%s\" on fd %d\n",argv[j],fd);

	   ev.events = EPOLLIN | EPOLLET;	 Only interested in input events 
	   ev.data.fd = fd;
	   if (epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&ev) == -1)
	   {
	   fprintf(stderr,"epoll_ctl");
	   exit(EXIT_FAILURE);
	   }
	   }
	 */

	pthread_create(&th_id,NULL,serve_clients,&queFront);

	numopenFds = 1;
	printf("epfd=%d,server_sockfd =%d\n",epfd,server_sockfd);

	while(numopenFds > 0)
	{
		/* Fetch up to MAX_EVENTS items from the ready list */
		/* epoll_wait()监听epfd对象中包含的fd和事件,发生则将事件的信息填写到清单evlist中，
		   epoll_wait()的返回值为成功写到evlist中的ev的个数 */

		//		printf("About to epoll_wait()\n");
		ready = epoll_wait(epfd,evlist,MAX_EVENTS,-1);
		//		sleep(2);
		if(ready == -1)
		{
			if(errno == EINTR)
			{
				printf("restarting epoll_wait\n");
				continue;	/* Restart if interrupted by signal */
			}

			else
			{
				//	fprintf(stderr,"epoll_wait");
				perror("epoll_wait");
				pthread_join(th_id,NULL);
				exit(EXIT_FAILURE);
			}
		}

		if(ready)
			printf("Ready: %d file descriptors\n",ready);

		/* Deal with returned list of events */
		/* for循环对evlist中的事件一一进行处理 */

		for(j = 0;j  < ready; j++)
		{
			printf("fd = %d; events: %s %s %s\n",evlist[j].data.fd,
					(evlist[j].events&EPOLLIN)?"EPOLLIN":"",
					(evlist[j].events&EPOLLHUP)?"EPOLLHUP":"",
					(evlist[j].events&EPOLLERR)?"EPOLLERR":"");
			if( evlist[j].data.fd == server_sockfd)
			{
				client_sockfd = accept(server_sockfd,(struct sockaddr *)&client_address,&client_len);

				if( client_sockfd < 0)
				{
					perror("client_sockfd < 0");
					pthread_join(th_id, NULL);
					exit(EXIT_FAILURE);
				}
				setnonblocking(client_sockfd);
				char *str = inet_ntoa(client_address.sin_addr);
				printf("accepted an connection from %s port: %d\n",str,ntohs(client_address.sin_port));

				ev.data.fd = client_sockfd;
				ev.events = EPOLLIN | EPOLLET;
				if(epoll_ctl(epfd,EPOLL_CTL_ADD,client_sockfd,&ev) == -1)
				{
					perror("epoll_ctl");
					pthread_join(th_id, NULL);
					exit(EXIT_FAILURE);
				}
				numopenFds += 1;
			}

			else if(evlist[j].events & EPOLLIN)
			{
				if(evlist[j].data.fd == 0)
				{
					memset(buf,'\0',MAX_BUF);
					s = read(0,buf,MAX_BUF);
					if(s == 0)
					{
						continue;
					}
					else if(strncmp(buf,"quit",4) == 0)
						return 0;

				}
				else
					fd = evlist[j].data.fd;
				if(fd < 0)
				{
					continue;
				}

				memset(buf,'\0',MAX_BUF);
				s = read(evlist[j].data.fd, buf, sizeof(BM_MSGHEAD));
				memcpy(&mHead, buf, sizeof(BM_MSGHEAD));
				mHead.len = ntohs(mHead.len);
				s+= read(evlist[j].data.fd, buf+sizeof(BM_MSGHEAD), mHead.len - sizeof(BM_MSGHEAD));
				if(s == -1)
				{
					if( errno == ECONNRESET) {
						close(fd);
						evlist[j].data.fd = -1;
					}
					else{ 
						perror("read(evlist[j].data.fd,buf, mHead.len - sizeof(BM_MSGHEAD))");
					}

					continue;
					//fprintf(stderr,"read\n");
					//exit(EXIT_FAILURE);
				}
				else if(s == 0){
					close(fd);
					evlist[j].data.fd = -1;
				}

				printf("  read %d bytes: %s\n",s,buf+sizeof(BM_MSGHEAD));
				//	printf("  read %d bytes: %.*s\n",s,s-sizeof(BM_MSGHEAD),buf+sizeof(BM_MSGHEAD));

				pTemp = (BM_MSG *)calloc(1,sizeof(BM_MSG));
				if(pTemp == NULL)
				{
					perror("calloc");
					break;
				}

				pTemp->fd = evlist[j].data.fd;
				//				pTemp->pdb = 
				memcpy(pTemp->package, buf,s);
				pTemp->next = NULL;
				if(queFront == NULL)
				{
					queFront = pTemp;
					queEnd = pTemp;
				}
				else
				{
					queEnd->next = pTemp;
					queEnd = pTemp;
				}






				//		write(evlist[j].data.fd,buf,s);
				//		write(evlist[j].data.fd,"hehe\n",5);
			}
			else if(evlist[j].events & (EPOLLHUP | EPOLLERR))
			{
				/* If EPOLLIN and EPOLLHUP were both set, then there might 
				   be more than MAX_BUF bytes to read. Therefor, we close 
				   the file descriptor only if EPOLLIN was not set.
				   We'll read further bytes after the next epoll_wait(). */
				printf("	closing fd %d\n",evlist[j].data.fd);
				if(close(evlist[j].data.fd) == -1)
				{
					fprintf(stderr,"close error\n");

				}
				numopenFds--;
			}
		}
	}


	printf("All file descriptors closed; bye\n");
	pthread_join(th_id, NULL);
	exit(EXIT_SUCCESS);
}

/* setnonblocking()实现 */
int setnonblocking(int sockfd)
{
	int opts;
	opts = fcntl(sockfd,F_GETFL);
	if(opts < 0)
	{
		perror("fcntl(sockfd,F_GETFL)");
		exit(EXIT_FAILURE);
	}
	opts = opts | O_NONBLOCK;
	if(fcntl(sockfd,F_SETFL,opts) < 0)
	{
		perror("(fcntl(sockfd,F_SETFL,opts)");
		exit(EXIT_FAILURE);
	}
	//	exit(EXIT_SUCCESS);
	return 0;
}
