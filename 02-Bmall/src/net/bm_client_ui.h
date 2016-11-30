/*
** bm_client_ui.h
** 客户端的界面: 登录，注册，查询等等
*/
#ifndef BM_CLIENT_UI_H_
#define BM_CLIENT_UI_H_ 

#include <stdio.h>
#include <unistd.h>

#include "bm_net.h"
#include "../db/bm_usr.h"


int show_hotlist(int sockfd);

void show_mainmenu();

int turnoff_echo(struct termios *tp);
int recover_echo(struct termios * tp);

int client_login(int sockfd,BM_ACNT *acnt) ;


int client_signup(int sockfd,BM_ACNT *acnt);

#endif /* BM_CLIENT_UI_H_ */
