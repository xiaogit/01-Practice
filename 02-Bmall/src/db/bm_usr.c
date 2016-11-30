/*
 **bm_usr.c
 **义服务端用户帐号相关操作，用于服务器管理数据库中的帐号信        
 +-----------+--------------+------+-----+---------+----------------+
 | UserID    | int(11)      | NO   | PRI | NULL    | auto_increment |
 | UserName  | varchar(32)  | NO   |     | NULL    |                |
 | Email     | varchar(32)  | NO   |     | NULL    |                |
 | Password  | varchar(32)  | NO   |     | NULL    |                |
 | Gender    | varchar(16)  | YES  |     | NULL    |                |
 | Birthday  | date         | YES  |     | NULL    |                |
 | CellPhone | varchar(32)  | NO   |     | NULL    |                |
 | Address   | varchar(128) | NO   |     | NULL    |                |
 +-----------+--------------+------+-----+---------+----------------+
 **
 */

#include "bm_usr.h"
// #define BM_USR_DEBUG
bool is_registered(MYSQL * pdb,const BM_ACNT * pcnt)
{
	char sql_stmt[255];
	int row_count = -1;
	MYSQL_RES * query_result= NULL;

	if(pdb == NULL)     /* mysql_real_connect()执行成功时，返回值为&db的值,失败则返回NULL */
	{

		fprintf(stderr,"is_registered, Error: %s\n",mysql_error(pdb));
		//	exit(EXIT_FAILURE);
		return false;
	}
	else
	{
		memset(sql_stmt,'\0',sizeof(sql_stmt));

		sprintf(sql_stmt,"select UserID from Accounts where UserName =\'%s\';",pcnt->name);
		if( mysql_query(pdb,sql_stmt))
		{ 
			fprintf(stderr,"Error: %s\n %s\n",sql_stmt,mysql_error(pdb));
			return false;
			//			exit(EXIT_FAILURE);
		}
		else
		{
			query_result = mysql_store_result(pdb);
			/*		mysql_free_result(query_result);
					query_result = NULL;
			 */
			if(query_result )
			{
				row_count = mysql_num_rows(query_result);
				if(row_count > 0)
				{

					mysql_free_result(query_result);
					return true;
				}
				else
				{
					mysql_free_result(query_result);
					return false; 
				}

			}
			else
			{
				if(mysql_errno(pdb))
				{
					fprintf(stderr,"Error:%s\n",mysql_error(pdb));
					mysql_free_result(query_result);
					return false;
				} 
				fprintf(stderr,"Error:%s\n",sql_stmt);
				mysql_free_result(query_result);
				return false; 
			}
		} 
	}
}

int insert_usr(MYSQL * pdb, BM_USR * usr)
{
	char sql_stmt[255];
	//	int row_count = -1;
	//	MYSQL_RES * query_res = NULL;

	if(pdb == NULL)     /* mysql_real_connect()执行成功时，返回值为&db的值,失败则返回NULL */
	{
		fprintf(stderr,"is_registered, Error: %s\n",mysql_error(pdb));
		//	exit(EXIT_FAILURE);
		return -1;
	}
	else
	{
		printf("插入用户\n");
		sleep(1);
		
		memset(sql_stmt,'\0',sizeof(sql_stmt));
		sprintf(sql_stmt,"set names utf8;");
		mysql_query(pdb,sql_stmt);

		memset(sql_stmt,'\0',sizeof(sql_stmt));

		sprintf(sql_stmt,"insert into Accounts(UserName,Email,Password,Gender,Birthday,CellPhone,Address) values(\
			\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\');",usr->acnt.name,usr->email,usr->acnt.key,usr->gender,usr->birthday,usr->cellphone,usr->address);
		if( mysql_query(pdb,sql_stmt))
		{ 
			fprintf(stderr,"Error: %s\n %s\n",sql_stmt,mysql_error(pdb));
			return -1;
			//			exit(EXIT_FAILURE);
		}
		else
		{
			//if new account enlisted successfully
			return 0;
		} 
	}

}


int usr_login(MYSQL * pdb, BM_ACNT * acnt)
{
	BM_ACNT account;
	char password[BM_KEY_MAX+1];
	char sql_stmt[128];
	int row_count = -1;
	MYSQL_RES * query_result = NULL;
	MYSQL_ROW row = NULL;
	unsigned int  i ;
	unsigned int num_fields;
	unsigned long *lengths;

	if(pdb == NULL)     /* mysql_real_connect()执行成功时，返回值为&db的值,失败则返回NULL */
	{

		fprintf(stderr,"usr_login, Error: %s\n",mysql_error(pdb));
		//	exit(EXIT_FAILURE);
		return -1;
	}
	else
	{
		memset(sql_stmt,'\0',sizeof(sql_stmt));
		sprintf(sql_stmt,"set names utf8;");
		memset(sql_stmt,'\0',sizeof(sql_stmt));

		sprintf(sql_stmt,"select Password from Accounts where UserName =\'%s\';",acnt->name);
		if( mysql_query(pdb,sql_stmt))
		{ 
			fprintf(stderr,"Error: %s\n %s\n",sql_stmt,mysql_error(pdb));
			return -1;
			//			exit(EXIT_FAILURE);
		}
		else
		{
			//if new account enlisted successfully
			query_result = mysql_store_result (pdb);
			if(query_result)
			{

				row_count = mysql_num_rows(query_result);//the input id already exist, prompt to try again()
				num_fields = mysql_num_fields(query_result);
				//      printf("query_result:%p\n",query_result);
				if(row_count == 1)
				{
					if(row = mysql_fetch_row(query_result))
					{
						lengths = mysql_fetch_lengths(query_result);
						memset(password,'\0',BM_KEY_MAX+1);
						sprintf(password,"%.*s",(int)lengths[0],row[0]);
					}
					if(strncmp(password,acnt->key,BM_KEY_MAX))//输入与密码比较
					{
						mysql_free_result(query_result);
						return -1;
					}
					mysql_free_result(query_result);
					return 0;
				}
				else
				{
					fprintf(stderr,"Error:usr_login:%s\n",sql_stmt);	
					mysql_free_result(query_result);
					return -1;
				} 
			}
			else//mysql_store_result() returned nothing, should it have?
			{

				if(mysql_errno(pdb))
				{

					fprintf(stderr,"Error: %s\n",mysql_error(pdb));
					//                                              eixt(EXIT_FAILURE);
				}
				else if(mysql_field_count(pdb) == 0)
				{

					//query does not return data, it's not a SELECT
					;
				} 
			} 
			//			mysql_free_result(query_result);
			//			query_result = NULL;

			return -1;
		} 
	}
}
//int usr_key_update(MYSQL * pdb, BM_USR * usr)
int usr_profile_update(MYSQL * pdb,BM_USR * usr)
{
	char sql_stmt[255];
	//	int row_count = -1;
	//	MYSQL_RES * query_res = NULL;

	if(pdb == NULL)   
	{

		fprintf(stderr,"is_registered, Error: %s\n",mysql_error(pdb));
		//	exit(EXIT_FAILURE);
		return -1;
	}
	else
	{
		printf("updating\n");
		sleep(1);
		memset(sql_stmt,'\0',sizeof(sql_stmt));

		sprintf(sql_stmt,"update Accounts set Email=\'%s\',Gender=\'%s\',Birthday=\'%s\',CellPhone=\'%s\',Address=\'%s\' where\
				UserName = \'%s\'and Password =\'%s\';",usr->email,usr->gender,usr->birthday,usr->cellphone,usr->address,usr->acnt.name,usr->acnt.key);
		if( mysql_query(pdb,sql_stmt))
		{ 
			fprintf(stderr,"Error: %s\n %s\n",sql_stmt,mysql_error(pdb));
			return -1;
			//			exit(EXIT_FAILURE);
		}
		else
		{
			//if new account enlisted successfully
			return 0;
		} 
	}


}
int usr_key_update(MYSQL * pdb,BM_USR * usr,char newkey[BM_KEY_MAX])
{
	char sql_stmt[255];
	//	int row_count = -1;
	//	MYSQL_RES * query_res = NULL;

	if(pdb == NULL)   
	{

		fprintf(stderr,"is_registered, Error: %s\n",mysql_error(pdb));
		//	exit(EXIT_FAILURE);
		return -1;
	}
	else
	{
		printf("updating password\n");
		sleep(1);
		memset(sql_stmt,'\0',sizeof(sql_stmt));

		sprintf(sql_stmt,"update Accounts set Password=\'%s\' where\
				UserName = \'%s\' and Password=\'%s\';",newkey,usr->acnt.name,usr->acnt.key);
		if( mysql_query(pdb,sql_stmt))
		{ 
			fprintf(stderr,"Error: %s\n %s\n",sql_stmt,mysql_error(pdb));
			return -1;
			//			exit(EXIT_FAILURE);
		}
		else
		{
			//if new account enlisted successfully
			printf("password updated!\n");
			return 0;
		} 
	}


}

#ifdef BM_USR_DEBUG
int main()
{
	MYSQL db,*conn_db;
	int ret =0;
	BM_USR newUsr;
	BM_ACNT tst_acnt;
	char newkey[BM_KEY_MAX];

	mysql_init(&db);

	conn_db = mysql_real_connect(&db,"localhost","root","qwert","bMall",3306,NULL,0);
	if(conn_db == NULL)
	{
		fprintf(stderr,"mysql_real_connect failed, Error: %s\n",mysql_error(&db));
		exit(EXIT_FAILURE);

	}

	memset((newUsr.acnt.name),'\0',BM_NAME_MAX);
	memset((newUsr.acnt.key),'\0',BM_KEY_MAX);
	memset(newUsr.email,'\0',BM_EM_MAX);
	memset(newUsr.gender,'\0',BM_GENDER_MAX);
	memset(newUsr.birthday,'\0',BM_BIRTHDAY_MAX);
	memset(newUsr.cellphone,'\0',BM_CELLPHONE_MAX);
	memset(newUsr.address,'\0',BM_ADDRESS_MAX);
	strncpy(newUsr.acnt.name,"newusr",6);
	strcpy(newUsr.acnt.key,"helloword");
	strcpy(newUsr.email,"qwer@dmail.com");
	strcpy(newUsr.gender,"M");
	strcpy(newUsr.birthday,"1291-09-23");
	strcpy(newUsr.cellphone,"155800003");
	strcpy(newUsr.address,"1023 Northtown, Jinmen, Hubei Province, Japan");

	/*	printf("inserting\n");
		sleep(1);
		if(!insert_usr(conn_db,&newUsr))
		{
		printf("%s is enlisted from now on\n",newUsr.acnt.name);
		}

	 */	memset(tst_acnt.name,'\0',BM_NAME_MAX);
	memset(tst_acnt.key,'\0',BM_KEY_MAX);
	strcpy(tst_acnt.name,"usr_2");
	strcpy(tst_acnt.key,"111222");
	if(is_registered(conn_db,&tst_acnt))
	{
		printf("registered\n");

	}
	else
	{
		printf("not found\n");
	}
	if(!usr_login(conn_db,&tst_acnt))
	{
		printf("welcome: %s\n",tst_acnt.name);
	}
	else
		printf("error:loggin\n");

	/*	memset(newUsr.cellphone,'\0',BM_CELLPHONE_MAX);
		strcpy(newUsr.cellphone,"2112");
	 */	usr_profile_update(conn_db,&newUsr);

	memset(newkey,'\0',BM_KEY_MAX);
	strncpy(newkey,"whyutry",BM_KEY_MAX);
	usr_key_update(conn_db,&newUsr,newkey);

	mysql_close(conn_db);
	return 0;

}
#endif /* BM_USR_DEBUG */




