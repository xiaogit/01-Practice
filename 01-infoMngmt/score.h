#ifndef SCORE_H_
#define SCORE_H_
#define COURSENUMBER 3
#define COURSEMASK 3
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "user.h"

typedef struct scores{
	//Acnt acnt;
	char name[NAMELENGTH];
	int scores[COURSENUMBER];
	struct scores * next;
}SCR;

//everytime a new student registered, a initial record will be added to score
bool newstu_to_score(Acnt acnt);


SCR * load_scores();
SCR * delete_sc_list(SCR * h);
void show_all_scores(SCR * h);
bool show_single_score(SCR * p);
SCR * search_sc_name(SCR * h,char name[NAMELENGTH]);
SCR * remove_sc_fromlist(SCR * h, SCR * p);
bool revise_sc_listnode(SCR * p,int courseid);
bool save_sc_list_to_file(SCR *h);

bool input_scores(SCR *h,int courseid);
#endif
