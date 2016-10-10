#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>
#include"score.h"
#include"user.h"
#include "student.h"
#include "teacher.h"
#include "admin.h"
int main(){
	Acnt acur;
	Acnt * ap = &acur;
	int type = 0;
	char ch;

	acur.type = UNSET;
	strcpy(acur.usrname,"");
	strcpy(acur.pwd,"");
	
	printf("welcome!!!!press the letter to choose\n");
	printf("a) log in \nb) sign in \nq) log out\n");

	while((ch = getchar()) != 'q'){
		if('\n' == ch) continue;
		switch(ch){
			case 'a': 
				login(ap);//*ap to keep user info if succeed
				break;
			case 'b':
				sign_in(ap);
				break;
			default:
				printf("this option haven't been defined\n");
		}
		type =ap->type;
		type &= TYPEMASK;
		switch(type){
			case STUDENT:
				case_student(ap);
				break;
			case TEACH_CN:
				case_teacher(ap);
				break;
			case ADMIN :
				case_admin(ap);
				break;
			default: logout(ap);
		}
		while(getchar() != '\n') 
			continue;
	printf("What to do? Press the letter to choose\n");
	printf("a) log in \nb) sign in \nq) log out\n");
	}
	return 0;
}

