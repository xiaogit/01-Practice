#include <stdio.h>
#include <string.h>
#include "user.h"
#include "score.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

List *load_list(char listname[FILENAMELENGTH]){

	
	FILE * ifp;
	List * h = NULL;
	List * newp, *pRe;
	char name[NAMELENGTH];
	ifp = fopen(listname,"r");
	if(ifp == NULL){
		fprintf(stderr,"error in opening file %s \n",listname);
		exit(1);
	}
	
	while(fscanf(ifp,"%s",name) == 1){

		if((newp=(List *)malloc(sizeof(List))) != NULL)	{
	//		fscanf(ifp,"%s",newp->acnt.usrname);		
			strcpy(newp->acnt.usrname,name);
			fscanf(ifp,"%s",newp->acnt.pwd);
			fscanf(ifp,"%d",&newp->acnt.type);
			//newp->next = h;
			//h = newp;
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
			fprintf(stderr,"failed to allocate memory\n");
			exit(1);

		}
			
	}
	
	fclose(ifp);
	return h;
}
List * delete_list(List * h){
	List * pnext;
	
	while(h != NULL){
		pnext = h->next;
		free(h);
	//	h = h->next;
		h = pnext;
	}
	return h;	
}
int add_usr(char listname[FILENAMELENGTH],Acnt acnt){
	FILE * fp;
	fp = fopen(listname,"a+");
	if(fp != NULL){	
		fprintf(fp,"%s\n%s\n%d\n",acnt.usrname,acnt.pwd,acnt.type);
	}
	else{
		fprintf(stderr,"Unable to open file %s\n",listname);
		exit(1);
	}
	if(fclose(fp) != 0){
		fprintf(stderr,"error in closing files\n");
	}

	return 0;
}

bool show_all_records(List * h){
	List * p = h;
	if(p == NULL){
		fprintf(stderr,"list head h points to EMPTY list\n");
		return false;
	}
	printf("%-10s\t%-10s\t%-10s\n\n","name","password","type");
	while(p != NULL){
		printf("%-10s\t%-10s\t%-10d\n",p->acnt.usrname,p->acnt.pwd,p->acnt.type);
		p = p->next;
	}
	return true;
}
bool show_single_record(List * p){
	if(p == NULL){
		fprintf(stderr,"usrname doesn't exit\n");
		return false;
	}
	printf("%-10s\t%-10s\t%-10s\n\n","name","password","type");
	printf("%-10s\t%-10s\t%-10d\n",p->acnt.usrname,p->acnt.pwd,p->acnt.type);
	return true;
}

List * search_name(List *h,char name[NAMELENGTH]){
	List * p;
	p = h;
	
	while(p != NULL)
	{
		if(strcmp(p->acnt.usrname,name)==0){
			return p;
		}
		p = p->next;
	}
	return NULL;
}

/*remove node p from h */
List * remove_from_list(List * h,List *p){
	List * pRe =NULL;
	List * pCur = h;
	if(h == NULL||p == NULL){
		fprintf(stderr,"can't remove,wrong arguments\n");
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

/*in condition that source of newinfo is verified, we revise the value that *rp points to,
* use search_name() to get value of pointer rp */
bool revise_list_node(List * p,Acnt newinfo){
	if(p == NULL){		//if search_name() returns null,exit 
		fprintf(stderr,"can't revise, the usrname dosen't exist\n");
		return false;
	}
//	p->acnt = newinfo;
	strcpy(p->acnt.pwd,newinfo.pwd);


	return true;
}

/*change one's own password,acnt is current info of operating person*/
bool revise_pwd(Acnt *ap){
	char alllist[3][FILENAMELENGTH];
	char ch;
	List * h;
	List * p =NULL;
	char pwdbuff[3][20];
	int listid= ap->type;
	listid &=TYPEMASK;
	listid--;
	
	strcpy(alllist[0],"std.txt");
	strcpy(alllist[1],"tch.txt");
	strcpy(alllist[2],"adm.txt");

	h = load_list(alllist[listid]);
	p = search_name(h,ap->usrname);
	if(p == NULL){
		fprintf(stderr,"revise_pwd search name failed\n");
		return false;
	}
	while(1){	
		printf("input current password:");
		scanf("%s",pwdbuff[0]);
		if((strcmp(pwdbuff[0],p->acnt.pwd)) == 0){
			while(1){
				printf("\ninput new password:");
				scanf("%s",pwdbuff[1]);
				printf("\ninput again:");
				scanf("%s",pwdbuff[2]);
				if((strcmp(pwdbuff[1],pwdbuff[2])) == 0){
					printf("you can use new password now\n");
					strcpy(ap->pwd,pwdbuff[1]);
					strcpy(p->acnt.pwd,pwdbuff[1]);
					save_list_to_file(alllist[listid],h);
					break;
				}
				printf("error, try again\n");
			}
			break;
		}
		printf("wrong password,try again? y/n\n");
		ch=getchar();
		if(ch == 'y'){
			while(getchar() != '\n')
				continue;
			continue;
		}
		else break;
	}
	delete_list( h);
	return true;
}


bool save_list_to_file(char listname[FILENAMELENGTH],List * h){
	FILE * fp;
	List * pCur = h;
	fp = fopen(listname,"w");
	if(fp == NULL){
		fprintf(stderr,"Unable to open file %s\n",listname);
		return false;
	}
	if(h == NULL){
		fprintf(stderr,"Empty list!\n");
		return false;
	}

	while(pCur != NULL){
		fprintf(fp,"%s\n%s\n%d\n",pCur->acnt.usrname,pCur->acnt.pwd,pCur->acnt.type);
		pCur = pCur->next;
	}
	if(fclose(fp) !=0){
		fprintf(stderr,"Unable to close files\n");
		return false;
	}
	return true;
}
/*only stu/tch can create acnt via this func*//*can also revise to offer admin rsgt with invitations*/
bool sign_in ( Acnt *ap){		
	Acnt input;
	int type = UNSET;
	int listid;
	char ch;
	char alllist[2][FILENAMELENGTH];
	List *h = NULL;
	List *lp = NULL;
	char pwdbuff[2][20];
	//char alllist[3][];

	strcpy(alllist[0],"std.txt");
	strcpy(alllist[1],"tch.txt");
	//strcpy(alllist[2],"admin.txt");
		
	printf("select your identity, please:\n");
	printf("a) student\nb) teacher\nq) quit\n");
	printf("type in a letter to select, press q to quit\n");
	while((ch = getchar()) != 'q'){
		if('\n' == ch)
			continue;
		if(islower(ch)){	//shi bie xiao xie
			switch(ch){
				case 'a':
					type = STUDENT;
					break;
				case 'b':
					type = TEACH_CN;
					break;
				default:
					type = UNSET;
					break;
			}
			break;
		}
		else	
			printf("please use lower case letters\n");
		while(getchar() != '\n')
			continue;
		printf("please type another letter or a q.\n");
	}
	if(type == TEACH_CN){				/*teacher choose his/her own course*/
		while(getchar() != '\n') continue;
		printf("choose  the course you teach:\n");
		printf("a) Chinese b) Math c) English\n");
		 ch = getchar();
		 switch(ch){
			 case 'a':
			 	type = TEACH_CN;
				break;
			case 'b':
				type = TEACH_MH;
				break;
			case 'c':type = TEACH_EN;
				break;
			default: type = UNSET;
				break;
		 }
	}
	if(type ==UNSET){
		fprintf(stderr,"unable to select indentity,registeration failed!\n ");
		return false;
	}

/*load the coresponding list*/
	listid= (type & TYPEMASK)-1; 
	h = load_list(alllist[listid]);
	if(h == NULL){
		fprintf(stderr,"unable to open %s",alllist[listid]);
		return false;
	}
	
	/*get and check usrname*/
	printf("please input your username(1~%d bit combination of numbers and letters ).\n",NAMELENGTH-1);

	lp = h;
	do {

		scanf("%s",input.usrname);
		lp = search_name(h,input.usrname);
		if(lp == NULL){
			break;
		}
		printf("usrname %s has been occupied, input another one\n",input.usrname);

	}while(lp != NULL);
//	delete_list(h);
//	printf("we have %s \n",input.usrname);

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
	add_usr(alllist[listid],input);
	strcpy(ap->usrname,input.usrname);
	strcpy(ap->pwd,input.pwd);
	ap->type = input.type;
/*if a student signs in, add a record in to scores.txt*/
	if(type == STUDENT  ){
		if(! newstu_to_score(input)){
			fprintf(stderr,"unable to write new stu onto scores.txt\n");
			return false;
		}	
	}
	printf("congratulations, %s, you have signed in! keep your password:%s \n",input.usrname,input.pwd);
	delete_list(h);
	return true;		
}

void logout(Acnt *ap){
	ap->type = UNSET;
	strcpy(ap->usrname,"");
	strcpy(ap->pwd,"");
	return ;
}

bool login(Acnt *ap){
	
	Acnt input;
//	int type = UNSET;
	int num = 0; //to mark which file to load
	List * h = NULL;
	List * lp = NULL;
	char ch;
	char alllist[3][FILENAMELENGTH];

	//STRCPY
	strcpy(alllist[0],"std.txt");
	strcpy(alllist[1],"tch.txt");
	strcpy(alllist[2],"adm.txt");	

/*Quest the operator's type*/	//choose type after calling
	while(getchar() != '\n') continue;
	printf("welcome,please choose your identity\n");
	printf("a) student b) teacher c) administrator q) quit\n");
	ch = getchar();
	if(ch == 'q'){
		return false;
	}
	while(getchar() != '\n') continue;
	num =(int)( ch -'a');
//	printf("num = %d c =%c\n",num,ch);
	/*load file into a linked list*/
	h = load_list(alllist[num]);
	if(h == NULL){
		fprintf(stderr,"unable to read %s\n",alllist[num]);
		return false;

	}

	while(lp == NULL){


		printf("input user name!\n ");	//mi ma shu cuo hou de chongxin shuru, NOT OFFERING SUCH FUNCTN HERE
	
		scanf("%s",input.usrname);
	//	printf("input password:");
	//	scanf("%s",input.pwd);
		lp = search_name(h,input.usrname);
		if(lp == NULL){
			printf("unsolid usrname!\n");
			continue;
		}
		printf("input password:");
		scanf("%s",input.pwd);
		while(strcmp(lp->acnt.pwd,input.pwd) != 0){
			printf("wrong password,try again\n");
			scanf("%s",input.pwd);
		}
	}
	

//	delete_list(h);
	printf("welcome,dear %s !\n",input.usrname);

	strcpy(ap->usrname ,input.usrname);
	strcpy(ap->pwd,input.pwd);
	ap->type = lp->acnt.type;
	delete_list(h);
	return true;
}

