#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "student.h"
#include "user.h"
#include "score.h"
#include "teacher.h"

bool case_teacher(Acnt * ap){
	char ch;
	printf("what to do now?\n");
	printf("a) check a student's scores \nb) check all students' scores \nc) update all scores\nd) change my password \nq) log out\n");

	while((ch = getchar()) != 'q'){
		if('\n' == ch) continue;
		switch(ch){
			case 'a': 
				tch_a_score(ap);//view ,can change if need to
				break;
			case 'b':
				tch_all_scores();
				break;
			case 'c':
				tch_input_scores(ap);
				break;
			case 'd':
				revise_pwd(ap);
				break;
			default:
				printf("this option haven't been defined\n");
		}
		while(getchar() != '\n') 
			continue;
		printf("what to do now?\n");
		printf("a) check a student's scores \nb) check all students' scores \nc) update all scores\nd) change my password \nq) log out\n");
	}
	logout(ap);
	return true;

}

/*tch can view and choose to revise or not */
bool tch_a_score(Acnt *ap){
	char namebuff[NAMELENGTH] ;
	SCR * h =NULL;
	SCR * lp =NULL;
	char ch;
	int courseid = ap->type;
	 
/*calculate courseid via type	*/
	courseid >>=2;
	courseid &=COURSEMASK;
//	printf("cosid= %d",courseid);

	h = load_scores();
	printf("input the student's name please:\n");
	scanf("%s",namebuff);
//	printf("input password:");
//	scanf("%s",input.pwd);
	lp = search_sc_name(h,namebuff);
	if(lp == NULL){
		printf("unsolid usrname!\n");
		return false;
	}
	show_single_score(lp);
	printf("you can revise the score of the course you teach\n ");
	printf("you want to revise?  y/n\n");
	while(getchar() != '\n')
		continue;
	ch = getchar();
	if(ch == 'y'){
//		printf("revsing\n");
		revise_sc_listnode(lp,courseid);
		if(!save_sc_list_to_file(h)) {
			printf("failed!\n");
			delete_sc_list(h);
			return false;
		}
		printf("completed!\n");	
	}
	delete_sc_list(h);
	return true;
}

/*view all scores of all courses and all students*/
bool tch_all_scores(){
	SCR *h = NULL;
//	SCR *lp;
//	char ch;
	h = load_scores();
	if(h == NULL){
		fprintf(stderr,"failed to load score file\n");
		return false;
	}
	show_all_scores(h);
	delete_sc_list(h);
	return true;
}

bool tch_input_scores(Acnt *ap){
	SCR *h =NULL;
	int courseid ;
	
	courseid = ap->type >>2;
	courseid &=COURSEMASK;
	h= load_scores();
	if(h ==NULL){
		fprintf(stderr,"failed to load score file");
		return false;
	}
	input_scores(h,courseid);
	save_sc_list_to_file(h);

	delete_sc_list(h);
	return true;
}

