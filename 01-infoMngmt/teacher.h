#ifndef TEACHER_H_
#define TEACHER_H_
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "student.h"
#include "user.h"
#include "score.h"

bool case_teacher(Acnt * ap);
bool tch_a_score(Acnt *ap);
bool tch_all_scores();
bool tch_input_scores(Acnt *ap);

#endif
