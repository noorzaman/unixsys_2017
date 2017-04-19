#include <stdio.h>

#include "cfprocessor.h"

void extraSimple1();
void extraSimple2();

void testIfBlockStructure();
void testNestedBlockStructure();
void testNestedBlockStructure2();

void testNestedIfs();

int main() {

//    extraSimple1();
//    extraSimple2();
//
//    testIfBlockStructure();
//    testNestedBlockStructure();
//    testNestedBlockStructure2();
//
    testNestedIfs();

    return 0;
}

void extraSimple1() {
    printf ("\n\n== extraSimple1\n\n");

    char *args []= {"if", "true", NULL};
    add(args);
    process(NULL);
}
void extraSimple2() {
    printf ("\n\n== extraSimple2\n\n");

    char *args []= {"if", "ls -l", "$HOME", NULL};
    add(args);
    process(NULL);
}


/**
 if grep noor /etc/passwd
 then
    echo here then
 else
    echo here else
 fi
 */

void testIfBlockStructure() {
    printf ("\n\n== testIfBlockStructure\n\n");

    char * args1[] = { "if", "grep noor /etc/passwd", NULL };
    add(args1);

    char * args2[] = { "then", NULL };
    add(args2);

    char * args3[] = { "echo here then", NULL };
    add(args3);

    char * args4[] = { "else", NULL };
    add(args4);

    char * args5[] = { "echo here else", NULL };
    add(args5);

    char * args6[] = { "fi", NULL };
    add(args6);

    process(NULL);

}

/**
 if grep noor /etc/passwd
 then
    echo here then
    if ls -l
    then
        echo nested then
    fi
 else
    echo here else
 fi
 */
void testNestedBlockStructure() {
    printf ("\n\n== testNestedBlockStructure\n\n");

    char * args1[] = { "if", "grep noor /etc/passwd", NULL };
    add(args1);

    char * args2[] = { "then", NULL };
    add(args2);

    char * args3[] = { "echo here then", NULL };
    add(args3);


    char * args31[] = {"if", "ls -l", NULL};
    add(args31);
    char * args32[] = {"then", NULL };
    add(args32);
    char * args33[] = {"echo nested then", NULL};
    add(args33);
    char * args34[] = {"fi", NULL};
    add(args34);


    char * args4[] = { "else", NULL };
    add(args4);

    char * args5[] = { "echo here else", NULL };
    add(args5);

    char * args6[] = { "fi", NULL };
    add(args6);

    process(NULL);

}



/**
 if grep noor /etc/passwd
 then
    echo here then
    if ls -l
    then
        echo nested then
    else
        echo nested else
    fi
 else
    echo here else
 fi
 */
void testNestedBlockStructure2() {
    printf ("\n\n== testNestedBlockStructure2\n\n");

    char * args1[] = { "if", "grep noor /etc/passwd", NULL };
    add(args1);

    char * args2[] = { "then", NULL };
    add(args2);

    char * args3[] = { "echo here then", NULL };
    add(args3);

    char * args31[] = {"if", "ls -l", NULL};
    add(args31);
    char * args32[] = {"then", NULL };
    add(args32);
    char * args33[] = {"echo nested then", NULL};
    add(args33);
    char * args34[] = {"else", NULL};
    add(args34);
    char * args35[] = {"echo nested else", NULL};
    add(args35);
    char * args36[] = {"fi", NULL};
    add(args36);

    char * args4[] = { "else", NULL };
    add(args4);

    char * args5[] = { "echo here else", NULL };
    add(args5);

    char * args6[] = { "fi", NULL };
    add(args6);

    process(NULL);

}

/**
echo Output should consist of lines 1 2 3 4 5 6
if echo line 1
then
	echo line 2
	if false
	then
		echo error 1
		echo error 2
	else
		if echo line 3
		then
			echo line 4
		else
			echo error 3
		fi
	fi
	echo line 5
else
	if true
	then
		echo error 4
	else
		echo error 5
	fi
	echo error 6
fi
echo line 6
 */
void testNestedIfs() {

    char * args1[] = { "if", "echo line 1"};
    add(args1);

    char * args2[] = { "then" };
    add(args2);

    char * args3[] = { "echo line 2" };
    add(args3);

    char * args4[] = { "if", "false" };
    add(args4);

    char * args5[] = { "then" };
    add(args5);

    char * args6[] = { "echo error 1" };
    add(args6);

    char * args7[] = { "echo error 2" };
    add(args7);

    char * args8[] = { "else" };
    add(args8);

    char * args9[] = { "if",  "echo line 3" };
    add(args9);

    char * args10[] = { "then" };
    add(args10);

    char * args11[] = { "echo line 4" };
    add(args11);

    char * args12[] = { "else" };
    add(args12);

    char * args13[] = { "echo error 3" };
    add(args13);

    char * args14[] = { "fi" };
    add(args14);

    char * args15[] = { "fi" };
    add(args15);

    char * args16[] = { "echo line 5" };
    add(args16);

    char * args17[] = { "else" };
    add(args17);

    char * args18[] = { "if", "true" };
    add(args18);

    char * args19[] = { "then" };
    add(args19);

    char * args20[] = { "echo error 4" };
    add(args20);

    char * args21[] = { "else" };
    add(args21);

    char * args22[] = { "echo error 5" };
    add(args22);

    char * args23[] = { "fi" };
    add(args23);

    char * args24[] = { "echo error 6" };
    add(args24);

    char * args25[] = { "fi" };
    add(args25);

    char * args26[] = { "echo line 6" };
    add(args26);

    process(NULL);
}
