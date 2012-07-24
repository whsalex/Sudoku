#ifndef _SUDOKU_
#define _SUDOKU_

/******************************************************************************/
/*                      Copyright (c) 2010, wanghs.                           */
/*                          All Rights Reserved                               */
/*                                                                            */
/*      THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF NEC CORPORATION        */
/*             The copyright notice above does not evidence any               */
/*            actual or intended publication of such source code.             */
/*                                                                            */
/*******************************************************************************
        File Description: Solution of sudoku
        -----------------
        Subsystem    :  
        Current Version/Date: 2010/5/16
*******************************************************************************/

#include    <stdio.h>
#include    <string.h>
#include    <sys/time.h>

using namespace std;


//#define DEBUG

static const int     SUDOKU_OK  = 0;
static const int     SUDOKU_NG  = -1;
static const int     SUDOKU_DE  = -2;
static const int     SUDOKU_RE  = -3;

timeval currentTime;   //calculate run time

//sudoku block
struct SuDoku_block{
    unsigned int number;                              //OK Number
    unsigned int point[9][9][2];                      //Point in block
                                                      //Possible numbers and
                                                      //how many
};

//sudoku list node
struct List{
    List* prior;                                      //Ponit of prior Node
    SuDoku_block block;                               //Sudoku node
};


/**
* main function
*
*/
int  main();

/**
* Read number
*
* @return List        Point of sudoku node.Return NULL is create error.
*/
List * readNumber();

/**
* Change number to binary form
*
* @param block          point of sudoku
*/
int doBinChange(SuDoku_block* block);

/**
* Change number to decimal form
*
* @param node          point of sudoku
*/
int doDecChange(SuDoku_block* block);

/**
* Find the possibility subset of sudoku
*
* @param block          point of sudoku
* return SUDOKU_DE      need to make decision
* return SUDOKU_REDE    need to regress
*/
int findSubset(SuDoku_block* block);

/**
* Make decision
*
* @param node          point of sudoku
* @return true         List need to findSubset
*/
List* makeDecision(List* node);

/**
* Decision regress
*
* @param node          point of sudoku
* @return List point   List need to findSubset
*/
List* regress(List* node);

/**
* Print sudoku block
*
* @param block         point of sudoku
*/
int printBlock(SuDoku_block* block);

#endif  
