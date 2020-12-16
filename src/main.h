#ifndef SIX_MAIN_H
#define SIX_MAIN_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sortings.h"

enum ERROR
{
    STATUS_OK,
    ERR_SECOND_PARAM,
    ERR_UNSUPPORTED_COMPARATOR,
    ERR_UNSUPPORTED_SORTING,
    ERR_READING,
    ERR_STRING_AMOUNT,
    ERR_PRINTING,
    ERR_PARAM_AMOUNT,
    ERR_OPEN_FILE,
    ERR_ALLOC
} errors;

static const char* word_bubble = "bubble";
static const char* word_merge = "merge";
static const char* word_quick = "quick";
static const char* word_insertion = "insertion";
static const char* word_radix = "radix";

static const char* asc = "asc";
static const char* des = "des";

#endif
