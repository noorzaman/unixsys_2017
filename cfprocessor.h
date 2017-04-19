//
// Created by Nurul Zaman on 4/14/17.
//

#ifndef SMSH_CFPROCESSOR_H
#define SMSH_CFPROCESSOR_H

#include "flexstr.h"

enum cmdtype { IF, THEN, ELSE, FI };

struct Cmdargs {
    FLEXLIST * data;
    struct Cmdargs * next;
};

struct Cmdblock {
    int cmdtype;
    enum cmdtype offset;
    struct Cmdargs * argsb;
    struct Cmdblock * thencmd;
    struct Cmdblock * elsecmd;
    struct Cmdblock * nestedifcmd;
    struct Cmdargs * argsa;
    struct Cmdblock * parent;
    int terminated;
};


void add(char ** args);
void process(struct Cmdblock * curr);
#endif //SMSH_CFPROCESSOR_H
