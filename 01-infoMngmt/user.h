#ifndef USER_H_
#define USER_H_

#include<stdbool.h>
//enum usertype {std,tch,adm,unset};
#define NAMELENGTH 30
#define FILENAMELENGTH 20
//typedef USRTYPE enum usertype;
/*type define*/
#define TYPEMASK 3
#define UNSET 0		//00 00 UNSET also represent unset scores in struct SCR member scores[3]
#define STUDENT 1		//00 01
#define ADMIN 3			//00 11
#define TEACH_CN 2		//00 10
#define TEACH_MH 6		//01 10
#define TEACH_EN 10		//10 10
//#define TEACH_OTHER   14 	//11 10 etc    
typedef struct account{
	char usrname[NAMELENGTH];
	char pwd[20];
	int type;
}Acnt;
typedef struct list{
	Acnt acnt;
	struct list * next;
}List;

List * load_list(char listname[FILENAMELENGTH]);


List * delete_list(List * h);
int add_usr(char listname[FILENAMELENGTH],Acnt acnt);
List * add_account(List * h, Acnt acnt);
List * search_name(List * h, char name[NAMELENGTH]);
bool show_all_records(List * h);
bool show_single_record(List * p);
List * remove_from_list(List * h, List * p);
bool revise_list_node(List *p,Acnt newinfo);
bool revise_pwd(Acnt * ap);
bool save_list_to_file(char listname[FILENAMELENGTH],List * h);

bool sign_in ( Acnt *ap);	//zhu ce
bool login(Acnt *ap);//deng ru
void logout(Acnt *ap);


#endif	
