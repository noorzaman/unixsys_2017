//
// Created by Nurul Zaman on 4/14/17.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "flexstr.h"
#include "smsh.h"
#include "cfprocessor.h"

#define D 1
#define UNEXPECTED "unexpected"

char *cmdtypestr[] = {"if", "then", "else", "fi"};

int ifcount = 0;
static enum cmdtype prevt;
static int expected[10];


static struct Cmdblock *root = NULL;
static struct Cmdblock *prev = NULL;
static struct Cmdblock *curr = NULL;

static FLEXLIST *flexit(char **);

static int islegal(int newtype);

static struct Cmdblock *create_new_cmdblock(enum cmdtype, char **args);
static struct Cmdargs *create_new_argsblock(char ** args);

void add_if(char **args);
void add_then(char **args);
void add_else(char **args);
void add_fi(char **args);
void add_cmd(char **args);

/**
 if grep noor /etc/passwd
 then
    echo here true
 else
    echo here false
 fi
 */
void add(char **args) {

    char *newtype = args[0];

    if (root == NULL && strcmp(newtype, "if") != 0)
        fatal("unexpected start of starting block", newtype, 1);


    if (strcmp(newtype, "if") == 0) {
        add_if(args);

    } else if (strcmp(newtype, "then") == 0) {
        add_then(args);

    } else if (strcmp(newtype, "else") == 0) {
        add_else(args);


    } else if (strcmp(newtype, "fi") == 0) {
        add_fi(args);


    } else {
        add_cmd(args);
    }

    if (D) {
        printf (" | process = ");
        process(root);
        printf (" | \n");

    }
}


/**
 if grep noor /etc/passwd
 then
    echo here true
 else
    echo here false
 fi
 */
void process(struct Cmdblock *curr) {

    if (curr == NULL) curr = root;

    printf("%s ", cmdtypestr[curr->cmdtype]);

    struct Cmdargs *ifca = curr->argsb;
    while (ifca != NULL) {
        char **stored = fl_getlist(ifca->data);
        int i;
        for (i = 0; i < fl_getcount(ifca->data); i++)
            printf ("%s ", stored[i]);
        ifca = ifca->next;
    }


    if (curr->thencmd != NULL) {
        printf("%s ", "then");

        struct Cmdargs *ca = curr->thencmd->argsb;
        while (ca != NULL) {
            char **stored = fl_getlist(ca->data);
            int i;
            for (i = 0; i < fl_getcount(ca->data); i++)
                printf("%s ", stored[i]);
            ca = ca->next;
        }

        if (curr->thencmd->nestedifcmd != NULL)
            process(curr->thencmd->nestedifcmd);

    }

    if (curr->elsecmd != NULL) {
        printf("%s ", "else");

        struct Cmdargs *ca = curr->elsecmd->argsb;
        while (ca != NULL) {
            char **stored = fl_getlist(ca->data);
            int i;
            for (i = 0; i < fl_getcount(ca->data); i++)
                printf("%s ", stored[i]);
            ca = ca->next;
        }

        if (curr->elsecmd->nestedifcmd != NULL)
            process(curr->elsecmd->nestedifcmd);
    }

    if (curr->terminated == 1) {
        printf("%s", "fi ");
    }

}

static struct Cmdblock *create_new_cmdblock(enum cmdtype type, char **args) {

    struct Cmdblock *newb = malloc(sizeof(struct Cmdblock));
    newb->cmdtype = type;
    newb->offset = type;
    newb->argsb = NULL;
    newb->argsa = NULL;
    newb->elsecmd = NULL;
    newb->thencmd = NULL;
    newb->nestedifcmd = NULL;
    newb->parent = NULL;
    newb->terminated = 0;

    if (args != NULL)
       newb->argsb = create_new_argsblock(args);

    return newb;
}


static struct Cmdargs *create_new_argsblock(char ** args) {
    struct Cmdargs *ca = malloc(sizeof(struct Cmdargs));
    ca->data = flexit(args);
    ca->next = NULL;
    return ca;
}
static FLEXLIST *flexit(char **src) {

    FLEXLIST *fl = malloc(sizeof(FLEXLIST));
    fl_init(fl, 0);

    int i = 0;
    while (src[i] != NULL) {
        if (D) printf("src[i]: %s\n", src[i]);
        fl_append(fl, strdup(src[i]));
        i++;
    }
    if (D)printf("src[i]: %s\n", "done");

    return fl;

}

static int islegal(int newtype) {

    if (newtype == IF)
        ifcount++;
    if (newtype == FI)
        ifcount--;

    if (ifcount < 0)
        return 0;

    if (curr != NULL) {
        if (newtype != FI && newtype == curr->offset)
            return 0;
    }

    int i = 0;
    while (expected[i] != -1) {
        if (newtype == expected[i])
            return 1;
        i++;
    }
    return 0;

}


void add_if(char **args) {

    if (!islegal(IF)) fatal("unexpected", "if", 1);
    if (args[1] == NULL) fatal("unexpeced naked if without conditions", "if", 1);

    struct Cmdblock *newifb = create_new_cmdblock(IF, args + 1);

    if (root == NULL) {
        root = newifb;
        curr = root;
        return;
    }

    if (curr->offset == THEN) {
        if (curr->thencmd == NULL) {
            struct Cmdblock *newthencmd = create_new_cmdblock(THEN, NULL);
            curr->thencmd = newthencmd;
        }
        curr->thencmd->nestedifcmd = newifb;

    } else if (curr->offset == ELSE) {
        if (curr->elsecmd == NULL) {
            struct Cmdblock *newelsecmd = create_new_cmdblock(ELSE, NULL);
            curr->elsecmd = newelsecmd;
        }
        curr->elsecmd->nestedifcmd = newifb;
    }

    newifb->parent = curr;
    curr = newifb;

    expected[0] = THEN;
    expected[1] = -1;
}

void add_then(char **args) {

    if ( !islegal(THEN) )
        fatal("unexpected", "then", 1);

    curr->offset = THEN;

    expected[0] = IF;
    expected[1] = ELSE;
    expected[2] = FI;
    expected[3] = -1;
}


void add_else(char **args) {

    if (!islegal(ELSE))
        fatal("unexpected", "else", 1);

    curr->offset = ELSE;

    expected[0] = IF;
    expected[1] = FI;
    expected[2] = -1;
}

void add_fi(char **args) {

    if (!islegal(FI))
        fatal("unexpected", "fi", 1);
    curr->offset = FI;
    curr->terminated = 1;
    if (curr->parent != NULL)
        curr = curr->parent; //pop if and switch to parent

    expected[0] = IF;
    expected[1] = ELSE;
    expected[2] = FI;
    expected[3] = -1;
}

void add_cmd(char **args) {

    struct Cmdargs *cargs = create_new_argsblock(args);

    if (curr->offset == IF) {
        if (curr->argsb != NULL)
            curr->argsb->next = cargs;
        else
            curr->argsb = cargs;

    } else if (curr->offset == FI && ifcount > 0) {
        if (curr->argsa != NULL)
            curr->argsa->next = cargs;
        else
            curr->argsa = cargs;

    } else if (curr->offset == THEN) {
        if (curr->thencmd != NULL) {
            if (curr->thencmd->argsb == NULL)
                curr->thencmd->argsb = create_new_argsblock(args);
            curr->thencmd->argsb->next = cargs;
        } else {
            struct Cmdblock *newthen = create_new_cmdblock(THEN, args);
            curr->thencmd = newthen;
            newthen->parent = (curr != NULL ? curr : NULL);
            newthen->offset = THEN;
        }

    } else if (curr->offset == ELSE) {
        if (curr->elsecmd != NULL) {
            if (curr->elsecmd->argsb == NULL)
                curr->elsecmd->argsb = create_new_argsblock(args);
            curr->elsecmd->argsb->next = cargs;
        } else {
            struct Cmdblock *newelse = create_new_cmdblock(ELSE, args);
            newelse->parent = (curr != NULL ? curr : NULL);
            newelse->offset = ELSE;
            curr->elsecmd = newelse;
        }
    }
}

void clear() {

}