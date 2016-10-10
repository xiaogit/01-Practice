#include "student.h"
#include "user.h"
#include "score.h"
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool case_student(Acnt * ap){
	char ch;
	printf("what to do now? \n");
	printf("a) check scores b) change password q) log out\n");

	while((ch = getchar()) != 'q'){
		if('\n' == ch) continue;
		switch(ch){
			case 'a': 
				stu_score(*ap);
				break;
			case 'b':
				revise_pwd(ap);
				break;
			default:
				printf("this option haven't been defined,try again\n");
		}
		while(getchar() != '\n') 
			continue;
		printf("what to do now?\n");
		printf("a) check scores b) change password q) log out\n");	
	}
	logout(ap);
	return true;

}
void stu_score(Acnt acur){
	SCR * h =NULL;
	SCR * p =NULL;
	h = load_scores();
	p= search_sc_name( h,acur.usrname);
	show_single_score(p);
	delete_sc_list(h);
	return;	
}













//sort_on_name()
//sort_on_courseid()
