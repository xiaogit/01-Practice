#ifndef BM_SERVICES_H_
#define BM_SERVICES_H

#include <mysql/mysql.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "bm_net.h"
#include "bm_client_ui.h"

int on_type_hotlist(MYSQL *conn_db, BM_MSG * pTemp);

int on_type_login(MYSQL * conn_db, BM_MSG * pTemp);

int on_type_signup_check(MYSQL * conn_db, BM_MSG * pTemp);

int on_type_signup_profile(MYSQL * conn_db, BM_MSG * pTemp);
#endif /* BM_SERVICES_H_ */
