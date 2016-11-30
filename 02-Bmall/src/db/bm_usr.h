#ifndef BM_USR_H_
#define BM_USR_H_
/**
 **
 mysql> describe Accounts;
 +-----------+--------------+------+-----+---------+----------------+
 | Field     | Type         | Null | Key | Default | Extra          |
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

 */
#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <stdbool.h>

#define BM_NAME_MAX 32
#define BM_KEY_MAX 32
#define BM_ID_MAX 32
#define BM_EM_MAX 32
#define BM_GENDER_MAX 4
#define BM_BIRTHDAY_MAX 12
#define BM_CELLPHONE_MAX 32
#define BM_ADDRESS_MAX 128

enum bm_acnt_states
{
	UNLOGGED = 0, LOGGED_IN , ACNT_STATE_REGISTERED
};

typedef struct bm_account{
	char id[BM_ID_MAX];
	char key[BM_KEY_MAX];
	char name[BM_NAME_MAX];
	enum bm_acnt_states state;
}BM_ACNT;
typedef struct bm_usr{
	BM_ACNT acnt;
	char email[BM_EM_MAX];
	char gender[BM_GENDER_MAX];
	char birthday[BM_BIRTHDAY_MAX];
	char cellphone[BM_CELLPHONE_MAX];
	char address[BM_ADDRESS_MAX];
}BM_USR;

/* 查询某用户名是否已经被注册 */
bool is_registered(MYSQL * db, const BM_ACNT * cnt);

/* 向bMall.Accounts新增用户 */
int insert_usr(MYSQL * db, BM_USR * usr);

/* 登录，密码帐号正确则成功，返回0;否则返回-1 */
int usr_login(MYSQL * db, BM_ACNT * anct);
/* 修改用户密码 */
int usr_key_update(MYSQL * pdb, BM_USR * usr,char newkey[BM_KEY_MAX]);
/* 修改用户信息，（不含密码） */
int usr_profile_update(MYSQL * pdb,BM_USR * usr);

#endif /* BM_USR_H_ */
