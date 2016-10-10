#ifndef ADMINISTRATER_H_
#define ADMINISTRATER_H_
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "student.h"
#include "user.h"
#include "score.h"
#include "teacher.h"

bool case_admin(Acnt *ap);
bool view_accounts();
bool search_account();
bool view_scores();
bool add_admin();
#endif
