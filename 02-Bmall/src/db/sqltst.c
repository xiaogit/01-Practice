// gcc -o pst sqltst.c -lmysqlclient -Wall
#include <mysql/mysql.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
//#include 

int main()
{
	char sql_stmt[256];
	MYSQL_FIELD * fields;
	MYSQL_RES *result;
	MYSQL db;
	MYSQL * pdb;
	MYSQL_ROW row;
	unsigned int num_fields;
	unsigned int num_row;
	unsigned int i;

	mysql_init(&db);
	pdb = mysql_real_connect(&db,"localhost","root","qwert","bMall",3306,NULL,0);
	if(!pdb)
	{
		fprintf(stderr,"mysql_real_connect failed, Error: %s\n",mysql_error(&db));
		exit(EXIT_FAILURE);

	}

	memset(sql_stmt,'\0',256);
	sprintf(sql_stmt,"select BookID as ID,Books.BookName as Name,UnitPrice as Price from Books;");

	if( mysql_query(pdb,sql_stmt))
	{
		fprintf(stderr,"%s error\n%s\n",sql_stmt,mysql_error(pdb));
		return -1;
	}
	else{
		result = mysql_store_result(pdb);
		if(result)
		{
			fields = mysql_fetch_fields( result);
			num_fields = mysql_num_fields( result);
			for(i = 0; i < num_fields; i++)
			{
				printf("%32.20s ",fields[i].name);
			}
			printf("\n");
		/*	while ((num_row = mysql_num_rows(result)))
			{
				for(i)
			}
		*/	

			mysql_free_result( result);
		}
		else
		{
			fprintf(stderr,"%s",mysql_error(pdb));
			return -1;
		}


	}


	mysql_close(&db);
}
