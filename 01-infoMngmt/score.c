
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "score.h"
#include "user.h"

//everytime a new student registered, a initial record will be added to score
bool newstu_to_score(Acnt acnt){
	FILE *fp;
	int i =0;
	if(acnt.type != STUDENT){	//avoid writing wrong account to score file
		fprintf(stderr,"passed a wrong type of acnt while using newstu_to_score()\n");
		return false;
	}
	fp = fopen("scores.txt","a+");
	
	if(fp != NULL){
		fprintf(fp,"\n%s\n",acnt.usrname);
		for(i = 0;i<COURSENUMBER;i++){
		 	fprintf(fp,"%d ",UNSET);
			if(i+1 == COURSENUMBER) printf("\n");
		}
	}
	fclose(fp);
	return true;
}

/*read scores from file*/
SCR * load_scores(){
	SCR * h = NULL;
	SCR * newp,* pRe;
	FILE * ifp;
	char name[NAMELENGTH];
	int i =0;
//	int typbuf;
//	char pwdbuff[20];
	
	ifp = fopen("scores.txt","r");
	if(NULL == ifp){
		fprintf(stderr,"unable to open scores.txt\n");
		exit(1);
	}

	while((fscanf(ifp,"%s",name)) == 1){
		if((newp=(SCR *)malloc(sizeof(SCR))) != NULL){
			strcpy(newp->name,name);
//			fscanf("%s %d",pwdbuff,typbuf);//skip acnt.pwd acnt.type
			for(i=0;i<COURSENUMBER;i++){
				fscanf(ifp,"%d",&(newp->scores)[i]);
			}
			if(h == NULL){
				h = newp;
				newp->next = NULL;
				pRe = newp;
				
			}
			else{
				pRe->next = newp;
				pRe = newp;
				newp->next = NULL;
			}
		}
		else{
			fprintf(stderr,"unable to allocate memory\n");
			exit(1);
		}
	}
	fclose(ifp);
	return h;
}


//free all memory *h holds, allocated while loading scores
SCR * delete_sc_list(SCR * h){
	SCR * pnext;
	while(h != NULL){
		pnext = h->next;
		free(h);
		h = pnext;
	}
	return h;
}


/*show all scores */
void show_all_scores(SCR * h){
	SCR * p = h;
	printf("%-10s\t%-10s\t%-10s\t%-10s\n","name","Chinese","Math","English");
	while( p != NULL){
		printf("%-10s\t%-10d\t%-10d\t%-10d\n",p->name,p->scores[0],p->scores[1],p->scores[2]);
		p = p->next;
	}
	return ;

}


/*get record of score for a specific student,p can be acquireed through search_sc_name()*/
bool show_single_score(SCR * p){
	if(p == NULL){
		fprintf(stderr,"user dosen't exist in score list\n");
		return false;
	}
	printf("%-10s\t%-10s\t%-10s\t%-10s\n","name","Chinese","Math","English");
	printf("%-10s\t%-10d\t%-10d\t%-10d\n",p->name,p->scores[0],p->scores[1],p->scores[2]);
	return true;

}

/*search name on link list of scores*/
SCR * search_sc_name(SCR * h,char name[NAMELENGTH]){
	SCR * p = h;
	
	while(p != NULL){
		if((strcmp(p->name,name)) == 0){
			return p;
		}
		p = p->next;
	}
	return NULL;
}

/*remove from score list*/
SCR * remove_sc_fromlist(SCR * h, SCR * p){
	SCR * pRe = NULL;
	SCR * pCur = h;
	if(h == NULL|| p == NULL){
		fprintf(stderr,"can't remove, wrong arguments\n");
		return h;
	}

	while(pCur != p ){
		pRe = pCur;
		pCur = pCur->next;
	}
	if(pCur == h){
		h = pCur->next;
		free(pCur);
	}
	else{
		pRe->next = pCur->next;
		free(pCur);
	}
	return h;
}


/*revise score*/
bool revise_sc_listnode(SCR * p,int courseid){
	int newsc=0;

	if(p == NULL){
		fprintf(stderr,"can't revise score,the usrname doesn't exist\n");
		return false;;

	}
	printf("courseid:%d,to revise %s's score, input new score:",courseid,p->name);
	scanf("%d",&newsc);
	p->scores[courseid] = newsc;

	return true;
}


/*save score list to file*/
bool save_sc_list_to_file(SCR *h){
	FILE *fp;
	SCR * pCur =h;
	int i =0;
	fp = fopen("scores.txt","w");

	if(fp == NULL){
		fprintf(stderr,"unable to open scores.txt\n");
		return false;
	}
	if(h == NULL){
		fprintf(stderr,"empty link list!\n");
		return false;
	}

	while(pCur != NULL){
		fprintf(fp,"\n%s\n",pCur->name);
		for(i=0;i<COURSENUMBER;i++){
			fprintf(fp,"%d ",pCur->scores[i]);
			if(i+1 == COURSENUMBER)  printf("\n");
			
		}
		pCur = pCur->next;
	}

	if(fclose(fp) !=0){
		fprintf(stderr,"unable to close scores.txt \n");
		return false;
	}
	return true;
}

/*write scores of all students on file*/
/*before calling this functn, type of acnt*/
/* should have been verified as leagal */
/*beware that link list was't saved to file here*/
bool input_scores(SCR *h,int courseid){
	SCR * pcur = h;
	if(pcur == NULL){
		fprintf(stderr,"link list is emptyy\n");
		return false;
	}
	printf("start inputing scores now,course id: %d\n",courseid);
	while(pcur != NULL){
		printf("name: %s\ninput score:",pcur->name);
		scanf("%d",&(pcur->scores[courseid]));
		pcur = pcur->next;

	}
	printf("---Done !\n");
	return true;
}
