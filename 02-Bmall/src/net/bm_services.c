#include <stdio.h>

#include "bm_services.h"
#include "../db/bm_usr.h"

int on_type_hotlist(MYSQL * conn_db, BM_MSG * pTemp)
{
	char buf[BUFSIZ];
	char sql_stmt[255] = { '\0' };
	int n;
	//	unsigned long len;
	BM_MSGHEAD mhead;
	MYSQL_ROW row = NULL;
	MYSQL_RES * query_result = NULL;
	MYSQL_FIELD * fields ;

	unsigned int num_fields;
	unsigned int  i;
	unsigned long *lengths;

	memset(sql_stmt,'\0',sizeof(sql_stmt));
	sprintf(sql_stmt,"set names utf8;");
	mysql_query(conn_db,sql_stmt);

	memset(sql_stmt,'\0',sizeof(sql_stmt));
	sprintf(sql_stmt,"select Books.BookID as 编号, Books.BookName as 书名, BookCategories.BookCategory as 分类,Books.Author as 作者,Books.UnitPrice as 单价, Books.CurrencyUnit as 币种,Books.Inventory as 库存,Books.SalesVolume as 销量 from Books inner join BookCategories on Books.BookCategoryID = BookCategories.BookCategoryID order by Books.Inventory;");
	if(mysql_query(conn_db,sql_stmt))
	{
		fprintf(stderr,"Error: %s\n %s\n",sql_stmt,mysql_error(conn_db));
		return -1;
		//break;
	}else
	{
		query_result = mysql_store_result(conn_db);
		if(query_result)
		{
			//		row_count = mysql_num_rows(query_result);
			num_fields = mysql_num_fields(query_result);
			fields = mysql_fetch_fields( query_result);
			memset(buf,'\0',BUFSIZ)	;
			//存储表头，' 编号'，'书名'等
			for(i = 0;i < num_fields; i++)
			{
				sprintf(buf,"%s%s\t",buf,fields[i].name);
			}

			while((row = mysql_fetch_row(query_result)))
			{ 

				/* 显示结果的每行 */
				//	memset(buf,'\0',BUFSIZ);
				sprintf(buf,"%s\n",buf);
				lengths = mysql_fetch_lengths(query_result);
				for(i = 0; i < num_fields; i++)
				{
					//		printf("[%.*s\t]",(int) lengths[i], row[i] ? row[i]: "NULL");
					sprintf(buf,"%s%s\t",buf, row[i] ? row[i]: "NULL");

				}

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
	return 0;
}

/* 逐行发送 
   memset(pTemp->package,'\0',sizeof(pTemp->package));
   mhead.len = sizeof(mhead) + strlen(buf);
   mhead.len = htons(mhead.len);
   mhead.msgtype = TYPE_HOTLIST;
   mhead.msgtype = htons(mhead.msgtype);
   memcpy(pTemp->package, &mhead, sizeof(mhead));
   sprintf(pTemp->package+sizeof(mhead), "%s", buf);
   write(pTemp->fd, pTemp->package,  sizeof(mhead) + sizeof(buf));

   printf("send string:\n%s\n",pTemp->package+sizeof(mhead));
 */	

int on_type_login(MYSQL * conn_db, BM_MSG * pTemp)
{
	BM_ACNT acnt;
	char buf[BUFSIZ];
	char sql_stmt[255] = { '\0' };
	char key[32];
	int n;
	BM_MSGHEAD mhead;
	MYSQL_ROW row = NULL;
	MYSQL_RES * query_result = NULL;

	/*	unsigned int num_fields;
		unsigned int  i;
		unsigned long *lengths;


		memset(sql_stmt,'\0',sizeof(sql_stmt));

		sprintf(sql_stmt,"set names utf8;");
		mysql_query(conn_db,sql_stmt);
	 */
	memcpy(&acnt, pTemp->package+sizeof(BM_MSGHEAD),sizeof(BM_ACNT));
	acnt.state = ntohs(acnt.state);

	if(! is_registered(conn_db, &acnt))
	{
		acnt.state = UNLOGGED;
	}
	else
	{
		if(! usr_login(conn_db, &acnt))
		{
			acnt.state = LOGGED_IN;
		}

	}

	mhead.msgtype = TYPE_LOGIN;
	mhead.msgtype = htons( mhead.msgtype);
	mhead.len = sizeof(BM_MSGHEAD) + sizeof(BM_ACNT);
	mhead.len = htons(mhead.len);
	acnt.state = htons(acnt.state);


	memcpy(pTemp->package, &mhead, sizeof(BM_MSGHEAD));
	memcpy(pTemp->package+sizeof(BM_MSGHEAD), &acnt, sizeof(BM_ACNT));
	write(pTemp->fd, pTemp->package, ntohs(mhead.len));

	return 0;
	//	memset(sql_stmt,'\0',sizeof(sql_stmt));

	//	sprintf(sql_stmt,"select key from Accounts where ")

}

int on_type_signup_check(MYSQL * conn_db, BM_MSG * pTemp)
{
	BM_ACNT acnt;
	BM_MSGHEAD mhead;
	/*	char buf[BUFSIZ];
		char sql_stmt[255] = { '\0' };
		char key[32];
		int n;
		MYSQL_ROW row = NULL;
		MYSQL_RES * query_result = NULL;
	 */

	memcpy(&mhead, pTemp->package, sizeof( BM_MSGHEAD));
	mhead.msgtype = ntohs( mhead.msgtype);
	if(mhead.msgtype != TYPE_SIGNUP_CHECK)
	{
		printf("消息通讯出错, fd = %d\n",pTemp->fd);
		return -1;
		//		ret = -1;
	}

	memcpy(&acnt, pTemp->package+sizeof(BM_MSGHEAD),sizeof(BM_ACNT));
	//	printf("state %d \n",acnt.state);
	acnt.state = ntohs(acnt.state);
	//	printf("state %d \n",acnt.state);

	if( is_registered(conn_db, &acnt))//用户名已经被注册
	{
		acnt.state = ACNT_STATE_REGISTERED;
	}
	else
	{
		/*	memset(sql_stmt,'\0',sizeof(sql_stmt));
			sprintf(sql_stmt,"set names utf8;");
			mysql_query(conn_db,sql_stmt);
		 */
		acnt.state = UNLOGGED;
	}

	mhead.msgtype = TYPE_SIGNUP_CHECK;
	mhead.msgtype = htons(mhead.msgtype);	
	mhead.len = sizeof( BM_MSGHEAD) + sizeof(BM_ACNT);
	mhead.len = htons(mhead.len);
	memcpy(pTemp->package, &mhead, sizeof( BM_MSGHEAD));
	acnt.state = htons(acnt.state);
	memcpy(pTemp->package+sizeof(BM_MSGHEAD), &acnt, sizeof(BM_ACNT));

	printf("reply TYPE_SIGNUP_CHECK\n");
	write(pTemp->fd, pTemp->package, ntohs(mhead.len));

	return 0;

}

int on_type_signup_profile(MYSQL * conn_db, BM_MSG * pTemp)
{
	BM_ACNT acnt;
	BM_MSGHEAD mhead;
	BM_USR user;
	/*	char buf[BUFSIZ];
		char sql_stmt[255] = { '\0' };
		char key[32];
		int n;
		MYSQL_ROW row = NULL;
		MYSQL_RES * query_result = NULL;
	 */
	memcpy(&mhead, pTemp->package, sizeof( BM_MSGHEAD));
	mhead.msgtype = ntohs( mhead.msgtype);
	if(mhead.msgtype != TYPE_SIGNUP_PROFILE)
	{
		printf("消息通讯出错, fd = %d\n",pTemp->fd);
		return -1;
		//		ret = -1;
	}

	memcpy(&user, pTemp->package+sizeof(BM_MSGHEAD),sizeof(BM_USR));
	memcpy(&acnt, &(user.acnt), sizeof( BM_ACNT));
	//	acnt.state = ntohs(acnt.state);

	if( insert_usr(conn_db, &user))
	{
		printf("error inserrt\n");
		return -1;
	}	
	acnt.state = htons(LOGGED_IN);

	/*	if( is_registered(conn_db, &acnt))//用户名已经被注册
		{
		acnt.state = ACNT_STATE_REGISTERED;
		}
		else
		{
		acnt.state = UNLOGGED;
		}
	 */

	memset(pTemp->package, '\0', BUFSIZ);
	mhead.msgtype = TYPE_SIGNUP_PROFILE;
	mhead.msgtype = htons(mhead.msgtype);	
	mhead.len = sizeof(BM_MSGHEAD)  + sizeof(BM_ACNT);
	mhead.len = htons(mhead.len);

	memcpy(pTemp->package, &mhead, sizeof( BM_MSGHEAD));
	memcpy(pTemp->package+sizeof(BM_MSGHEAD), &acnt, sizeof(BM_ACNT));

	printf("reply TYPE_SIGNUP_PROFILE\n");
	write(pTemp->fd, pTemp->package, ntohs(mhead.len));

	return 0;

}


