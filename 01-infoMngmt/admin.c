#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "student.h"
#include "user.h"
#include "score.h"
#include "teacher.h"
#include "admin.h"

bool case_admin(Acnt *ap){
	char ch;
	printf("what to do now? \n");
	printf("a) view acounts\nb) search an account\nc) view scores\nd) add administrator \ne) change my password \nq) log out\n");
	while((ch = getchar()) != 'q'){
		if('\n' == ch) continue;//if user press Enter before typing any letter, get a letter again
		switch(ch){
			case 'a': 
				view_accounts();
				break;
			case 'b':
				search_account();
				break;
			case 'c':
				view_scores();	/*view and search in one function*/
				break;
			case 'd':
				add_admin();
				break;
			case 'e':
				revise_pwd(ap);
				break; 
			default:
				printf("this option haven't been defined\n");
		}
		while(getchar() != '\n') 
			continue;
		printf("what to do now?\n");
		printf("a) view acounts\nb) search an account\nc) view scores\nd) add administrator \ne) change my password \nq) log out\n");
	}
	logout(ap);
	return true;

}	

bool view_accounts(){
	char listnames[3][FILENAMELENGTH];
	List * h =NULL;
	int num;
	char ch =' ';//kong ge

	strcpy(listnames[0],"std.txt");
	strcpy(listnames[1],"tch.txt");
	strcpy(listnames[2],"adm.txt");

	printf("which type of accounts you want to see\n");
	printf("a) view all students\nb) view all teachers \nc) view all administrators\nq) quit\n");
	while((ch = getchar()) != 'q'){
		if('\n' == ch) continue;
		num =(int)(ch-'a');
		switch(ch){
			case 'a':{
					h = load_list(listnames[num]);
					show_all_records(h);
					delete_list(h);
				}
				break;
			case 'b':{
					h = load_list(listnames[num]);
					show_all_records(h);
					delete_list(h);				
				}
				break;
			case 'c':{
					h = load_list(listnames[num]);
					show_all_records(h);
					delete_list(h);				
				}
				break;
			default:
				printf("this option haven't been defined\n");
		}
		while(getchar() != '\n') 
			continue;
//		printf("what to do now?\n");
	printf("which type of accounts you want to see\n");
	printf("a) view all students\nb) view all teachers \nc) view all administrators\nq) quit\n");
	}
	return true;
}

bool search_account(){
	char listnames[3][FILENAMELENGTH];
	char name[NAMELENGTH];
	List * h = NULL;
	List * lp = NULL;
	int num;
	char ch =' ';//kong ge

	strcpy(listnames[0],"std.txt");
	strcpy(listnames[1],"tch.txt");
	strcpy(listnames[2],"adm.txt");

	printf("which type of account do you want to search\n");
	printf("a) student \nb) teacher \nc) administrator\nq) quit\n");
	while((ch = getchar()) != 'q'){
		if('\n' == ch) continue;

		printf("please input usrname:\n");
		scanf("%s",name);
		num =(int)(ch-'a');
		switch(ch){
			case 'a':{
					h = load_list(listnames[num]);
					lp = search_name(h,name);
					if(lp == NULL){
						printf("user doesn't exist\n");						
					}
					else show_single_record(lp);
					delete_list(h);
				}
				break;
			case 'b':{
					h = load_list(listnames[num]);
					lp = search_name(h,name);
					if(lp == NULL){
						printf("user doesn't exist\n");						
					}
					else show_single_record(lp);
					delete_list(h);			
				}
				break;
			case 'c':{
					h = load_list(listnames[num]);
					lp = search_name(h,name);
					if(lp == NULL){
						printf("user doesn't exist\n");						
					}
					else show_single_record(lp);
					delete_list(h);			
				}
				break;
			default:
				printf("this option haven't been defined\n");
		}
		while(getchar() != '\n') 
			continue;
//		printf("what to do now?\n");
	printf("which type of account do you want to search\n");
	printf("a) student \nb) teacher \nc) administrator\nq) quit\n");
	}	
	return true;
}

bool view_scores(){
	SCR * h = NULL;
	SCR * lp = NULL;
	char name[NAMELENGTH];
	char ch =' ';//kong ge

	printf("please choose\n");
	printf("a) search with usrname \nb) view all scores \nq) quit\n");
	while((ch = getchar()) != 'q'){
		if('\n' == ch) continue;
		switch(ch){
			case 'a':{
					printf("please input usrname:\n");
					if(scanf("%s",name) != 1){
						fprintf(stderr,"failed to get ursnme\n");
						return false;
					}
					h = load_scores();
					lp = search_sc_name(h,name);
					show_single_score(lp);
					delete_sc_list(h);
				}
				break;
			case 'b':{
					h = load_scores();

					show_all_scores(h);
					delete_sc_list(h);			
				}
				break;
			default:
				printf("this option haven't been defined\n");
		}
		while(getchar() != '\n') 
			continue;
//		printf("what to do now?\n");
	printf("please choose\n");
	printf("a) search with usrname \nb) view all scores \nq) quit\n");
	}
	return true;
}

bool add_admin(){

	Acnt input;
	int type = ADMIN;
	List *h = NULL;
	List *lp = NULL;
	char pwdbuff[2][20];	
/*load the coresponding list*/
//	listid= (type &3)-1; 
	h = load_list("adm.txt");
	if(h == NULL){
		fprintf(stderr,"unable to open adm.txt\n");
		return false;
	}
	
	/*get and check usrname*/
	printf("please input an username(1~%d bit combination of numbers and letters ).\n",NAMELENGTH-1);

	lp = h;
	do {

		scanf("%s",input.usrname);
		lp = search_name(h,input.usrname);
		if(lp == NULL){
			break;
		}
		printf("usrname %s has been occupied, input another one\n",input.usrname);

	}while(lp != NULL);

/*get password ---twice*/
	while(1){
		printf("please input password: ");
		scanf("%s",pwdbuff[0]);
		printf("\ninput again to confirm:");
		scanf("%s",pwdbuff[1]);
		if((strcmp(pwdbuff[0],pwdbuff[1])) ==0){
			break;
		}
		printf("error,type again\n");
	}
	strcpy(input.pwd,pwdbuff[1]);
	input.type = type;
	
	/*add to file*/
	add_usr("adm.txt",input);

	printf("congratulations,%s has signed in! his password:%s \n",input.usrname,input.pwd);
	delete_list(h);
	return true;	

}
