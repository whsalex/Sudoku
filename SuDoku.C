
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


#include "SuDoku.H"


//Read number
List* readNumber()
{
#if defined DEBUG
    printf ("Enter readNumber().\n");
#endif
    
    int i,j;
    int ret     = 0;
    int flag    = 0;
    List* node  = NULL;

    //Creat LIST
    try {
        node = new List;
    } catch ( ... ) {
        printf ("Initialize new LIST failed,return NULL point.");
        goto EXIT;
    }
    
    //Creat successful,initialize to 0
    node->prior = NULL;
    memset( &(node->block),0x00,sizeof(SuDoku_block));  
    
    //initialize sudoku block
    while ( 0 == flag )
    {
        printf ("Please enter sudoku number 9*9.\n");
        
        for (i=0;i<9;i++) { 
            printf ("Please enter 9 numbers in line %d.\n",i+1);
            for (j=0;j<9;j++) {
                scanf ("%d",&(node->block.point[i][j][0]));
            }
        }
        
        printf ("Confirm input.ReType:0, Type_OK:1, Exit:2.\n");
        scanf ("%d",&flag);
        
#if defined DEBUG
        printf ("Flag is %d.\n",flag);
#endif  

    }//end while
    
    //Don't want to continue
    if ( 2 == flag ) {
        memset( &(node->block),0x00,sizeof(SuDoku_block));  
        delete node;
        node = NULL;
        goto EXIT;
    }
    
#if defined DEBUG
    printBlock ( &(node->block) );
#endif
    ret = gettimeofday( &currentTime, NULL );
    
    if (-1 == ret){
        printf ("Call gettimeofday failed.\n");
        goto EXIT;
    }
/*
#if defined DEBUG
    printf ("Sec is %u,Usec is %u.\n",currentTime.tv_sec,
        currentTime.tv_usec);
#endif
*/
    
EXIT:
#if defined DEBUG
    printf ("ReadNumber() ended.\n");
#endif
    return node;
}



//Change number to binary form
int doBinChange(SuDoku_block* block)
{
#if defined DEBUG
    printf ("Enter doBinChange().\n");
#endif

    int i,j;
    int temp = 0;
    
    if (NULL == block){
        printf ( "Block is NULL.\n" );
        return SUDOKU_NG;
    }
    
    //change Number to binary system
    for (i=0;i<9;i++){
        for (j=0;j<9;j++){
            if ( 0==block->point[i][j][0] ){
                block->point[i][j][0] = 511;
                block->point[i][j][1] = 9;
            } else {
                temp = block->point[i][j][0] - 1;
                block->point[i][j][0] = 1 << temp;
                block->point[i][j][1] = 1;
                block->number ++;
            }
        }//end for j
    }//end for i
    
#if defined DEBUG
    printf ( "blockOKNumber init is %d.\n",block->number );
    printBlock ( block );
#endif
    
#if defined DEBUG
    printf ("DoBinChange() ended.\n");
#endif
    return SUDOKU_OK;
}

// Change number to decimal form
int doDecChange(SuDoku_block* block)
{
#if defined DEBUG
    printf ("Enter doDecChange().\n");
#endif

    int i,j;
    
    if (NULL == block){
        printf ( "Block is NULL.\n" );
        return SUDOKU_NG;
    }

#if defined DEBUG
    printf ("blockOKNumber is %d.\n",block->number );
#endif
    
    //change Number to decimal system
    for (i=0;i<9;i++){
        for (j=0;j<9;j++){
            switch( block->point[i][j][0] ){
            case 1:
                block->point[i][j][0] = 1;
                break;
            case 2:
                block->point[i][j][0] = 2;
                break;
            case 4:
                block->point[i][j][0] = 3;
                break;
            case 8:
                block->point[i][j][0] = 4;
                break;
            case 16:
                block->point[i][j][0] = 5;
                break;
            case 32:
                block->point[i][j][0] = 6;
                break;
            case 64:
                block->point[i][j][0] = 7;
                break;
            case 128:
                block->point[i][j][0] = 8;
                break;
            case 256:
                block->point[i][j][0] = 9;
                break;
            default :
                printf ("Wrong number in %d,%d is %d [binary form].\n",i+1,j+1
                        ,block->point[i][j][0]);
            }//end switch
        }//end for j
    }//end for i

#if defined DEBUG
    printf ( "blockOKNumber final is %d.\n",block->number );
    printBlock ( block );
#endif  
    
#if defined DEBUG
    printf ("DoDecChange() ended.\n");
#endif
    return SUDOKU_OK;
}

//Find the possibility subset of sudoku
int findSubset(SuDoku_block* block)
{
#if defined DEBUG
    printf ("Enter findSubset().\n");
#endif

    int i,j,m;
    int left                = 0;
    int right               = 0;
    int up                  = 0;
    int down                = 0;
    unsigned int temp_number = 0;
    
    if (NULL == block){
        printf ( "Block is NULL.\n" );
        return SUDOKU_NG;
    }

#if defined DEBUG
    printf ( "blockOKNumber before findsubset is %d.\n",block->number );
    printBlock ( block );
#endif  
    
    do {
        //Findsubset when one point refresh
        temp_number = block->number ;
        // Search every point
        for (i=0;i<9;i++){
            for (j=0;j<9;j++){ 
                
                // Find a subset of unknown point
                if ( 1 != block->point[i][j][1] ){
                    // Eliminate the impossible vertical number 
                    for ( m=0; m<9; m++ ){                        
                        // Only compare to the known point 
                        if ( (1 == block->point[i][m][1]) && ( m != j ) ) {
                            
                            if ( 0 != (block->point[i][j][0] & 
                                block->point[i][m][0]) ){
                                //Eliminate this number
                                block->point[i][j][0] &= 
                                    (~block->point[i][m][0]);
                                block->point[i][j][1]--; 

                                if ( 0 == block->point[i][j][0] ){
#if defined DEBUG
                                    // Failed to decision,remake decision
                                    printf ( "Need re make decision.\n" );
#endif
                                    return SUDOKU_RE;
                                }
                            }
                        }//end find known point
                    } //end for m
                    
                    // Eliminate the impossible horizontal number  
                    for ( m=0; m<9; m++ ){                        
                        // Only compare to the known point 
                        if ( (1 == block->point[m][j][1]) && ( m != i ) ) {
                            if ( 0 != (block->point[i][j][0] & 
                                block->point[m][j][0]) ){
                                
                                //Eliminate this number
                                block->point[i][j][0] &= 
                                    (~block->point[m][j][0]);
                                block->point[i][j][1]--; 
                                
                                if ( 0 == block->point[i][j][0] ){
#if defined DEBUG
                                    // Failed to decision,remake decision
                                    printf ( "Need re make decision.\n" );
#endif  
                                    return SUDOKU_RE;
                                }
                            }
                        }//end find known point
                    } //end for m
                    
                    // Eliminate the impossible matrix number
                    left        = (i%3+1)%3 + i/3*3;
                    right       = (i%3+2)%3 + i/3*3;
                    up          = (j%3+1)%3 + j/3*3;
                    down        = (j%3+2)%3 + j/3*3;
                    
                    //upleft
                    if ( (1 == block->point[left][up][1]) ) {
                        if ( 0 != (block->point[i][j][0] & 
                            block->point[left][up][0]) ){
                                
                            //Eliminate this number
                            block->point[i][j][0] &= 
                                (~block->point[left][up][0]);
                            block->point[i][j][1]--; 

                            if ( 0 == block->point[i][j][0] ){
#if defined DEBUG
                                // Failed to decision,remake decision
                                printf ( "Need re make decision.\n" );
#endif  
                                return SUDOKU_RE;
                            }
                        } //end compare
                    }
                    
                    //downleft
                    if ( (1 == block->point[left][down][1]) ) {
                        if ( 0 != (block->point[i][j][0] & 
                            block->point[left][down][0]) ){
                                
                            //Eliminate this number
                            block->point[i][j][0] &= 
                                (~block->point[left][down][0]);
                            block->point[i][j][1]--; 
                                
                            if ( 0 == block->point[i][j][0] ){
#if defined DEBUG
                                // Failed to decision,remake decision
                                printf ( "Need re make decision.\n" );
#endif  
                                return SUDOKU_RE;
                            }
                        } //end compare
                    }
                    
                    //upright
                    if ( (1 == block->point[right][up][1]) ) {
                        if ( 0 != (block->point[i][j][0] & 
                            block->point[right][up][0]) ){
                                
                            //Eliminate this number
                            block->point[i][j][0] &= 
                                (~block->point[right][up][0]);
                            block->point[i][j][1]--; 
                                
                            if ( 0 == block->point[i][j][0] ){
#if defined DEBUG
                                // Failed to decision,remake decision
                                printf ( "Need re make decision.\n" );
#endif  
                                return SUDOKU_RE;
                            }
                        } //end compare
                    }
                    
                    //downright
                    if ( (1 == block->point[right][down][1]) ) {
                        if ( 0 != (block->point[i][j][0] & 
                            block->point[right][down][0]) ){
                                
                            //Eliminate this number
                            block->point[i][j][0] &= 
                                (~block->point[right][down][0]);
                            block->point[i][j][1]--; 
                                
                            if ( 0 == block->point[i][j][0] ){
#if defined DEBUG
                                // Failed to decision,remake decision
                                printf ( "Need re make decision.\n" );
#endif  
                                return SUDOKU_RE;
                            }
                        } //end compare
                    }
                    
                    //Plus OK number when it unique
                    if ( 1 == block->point[i][j][1] ){
                        block->number ++;
                    }
                } //end a unknown point
            }//end for j
        }//end for i
    
    } while ( (temp_number != block->number) && (81 != block->number) );

#if defined DEBUG
    printf ( "blockOKNumber after findsubset is %d.\n",block->number );
    printBlock ( block );
#endif  
    
    if (81 != block->number){
#if defined DEBUG
        printf ( "Need to make decision.\n" );
#endif  
        return SUDOKU_DE;
    }
    
#if defined DEBUG
    printf ("FindSubset() ended.\n");
#endif
    return SUDOKU_OK;
}

//Make decision
List* makeDecision(List* node)
{
#if defined DEBUG
    printf ("Enter makeDecision().\n");
#endif

    int i,j,m;
    int bin                = 0;
    unsigned int deNumber  = 2;
    List* newNode          = NULL;

    if (NULL == node){
        printf ( "List node point is NULL.\n" );
        return NULL;
    }

    //Creat new LIST node
    try {
        newNode = new List;
    } catch ( ... ) {
        printf ("Create new LIST failed,return NULL point.");
        return NULL;
    }

    newNode->prior = node;
    memset( &(newNode->block),0x00,sizeof(SuDoku_block));  
 
    while ( deNumber <=9 ){
        for (i=0;i<9;i++){
            for (j=0;j<9;j++){ 
                
                // Find a subset of unknown point
                if ( deNumber == node->block.point[i][j][1] ){

#if defined DEBUG
                    printf ("[%d][%d] has %d choices.\n",
                    i+1,j+1,deNumber);
#endif
                    //find the first number to be eliminated
                    for (m=0;m<9;m++){
                        bin = 1 << m;
                if ( 0 != (bin & node->block.point[i][j][0])){
                            //set value to new node
                            memcpy(&(newNode->block),
                                &(node->block),sizeof(SuDoku_block));

                            //Eliminate this number for recorder
                            node->block.point[i][j][0] &= (~bin);
                            node->block.point[i][j][1]--;
                            if ( 1 == node->block.point[i][j][1]){
                                node->block.number++;
                            }
                       
                            //Edit the block after decision 
                            newNode->block.point[i][j][0] = bin;
                            newNode->block.point[i][j][1] = 1;
                            newNode->block.number++;
                            goto EXIT;
                        }//end if
                   }
                }//end compare deNumber
            }//end for j
        }//end for i

        deNumber++;
    }//end while

EXIT:    
#if defined DEBUG
    printf ("MakeDecision() ended.\n");
#endif
    return newNode;
}

//Decision regress
List* regress(List* node)
{
#if defined DEBUG
    printf ("Enter regress().\n");
#endif

    List*   prior = NULL;

    if (NULL == node){
        printf ( "List node point is NULL.\n" );
        return NULL;
    }

    if (NULL == node->prior){
        printf ( "Impossible to regress.\n" );
        return NULL;
    }

    prior = node->prior;

    //free
    delete node;
    node = NULL;

#if defined DEBUG
    printf ("Regress() ended.\n");
#endif
    return prior;
}

//Print sudoku block
int printBlock(SuDoku_block* block)
{
#if defined DEBUG
    printf ("Enter printBlock().\n");
#endif

    int i,j;
    
    if (NULL == block){
        printf ( "Block is NULL.\n" );
        return SUDOKU_NG;
    }
    
    printf ("Sudoku result is as follow: \n");
    printf ("########################### \n");
    //Print result
    for (i=0;i<9;i++){
        for (j=0;j<9;j++){
            printf ("%d  ",block->point[i][j][0]);
        }
        printf ("\n"); //end line
    }
    printf ("########################### \n");

#if defined DEBUG
    printf ("PrintBlock() ended.\n");
#endif
    return SUDOKU_OK;
}

//Main function
int main()
{
    timeval newTime;
    int tmp_return         = -1;
    int ret                = 0;
    unsigned long runTime  = 0;
    float second           = 0;
    List* node             = NULL;
    List* result           = NULL;
   
    memset (&newTime,0x00,sizeof(newTime)); 
    //Initialize puzzle
    node = readNumber();
    if (NULL == node){
        printf ("Exit to input.\n");
        return SUDOKU_NG;
    }
    
    //Change form to binary
    tmp_return = doBinChange (&(node->block));
    if ( SUDOKU_NG == tmp_return){
        printf ("Error in binary change.\n");
        return SUDOKU_NG;
    }
    
    //Find the minimum subset
    while (1) {

        if ( 0 == node->block.number ){
            printf ("Error to find result.\n");
            return SUDOKU_NG;
        }
        
        tmp_return = findSubset(&(node->block));
        
        if ( SUDOKU_NG == tmp_return){
            printf ("Error in findSubset change.\n");
            return SUDOKU_NG;
            
        } else if ( SUDOKU_OK == tmp_return ){
            // Finish calculate, break
            break;
            
        } else if ( SUDOKU_DE == tmp_return ){
            //Make decision
            result = makeDecision (node);
            
            if ( NULL == result){
                printf ("Error in decision.\n");
                return SUDOKU_NG;
            }
   
            node = result;
            result = NULL;
            
        } else if ( SUDOKU_RE == tmp_return ){
            //Regress
            result = regress(node);
            
            if ( NULL == result){
                printf ("Error in regress.\n");
                return SUDOKU_NG;
            }

            node = result;
            result = NULL;
            
        } else {
            printf ("Error in findSubset change.\n");
            return SUDOKU_NG;
        }
    }
   
    //change form to decimal 
    doDecChange(&(node->block));

    ret = gettimeofday( &newTime, NULL );
    
    if (-1 == ret){
        printf ("Call gettimeofday failed.\n");
    } else {
/*
#if defined DEBUG
        printf ("Finished!Now sec is %u,Usec is %u.\n",
               newTime.tv_sec,newTime.tv_usec);
#endif
*/
        runTime = (newTime.tv_sec - currentTime.tv_sec)*1000000 +
                   newTime.tv_usec - currentTime.tv_usec;
        second = runTime/1000000.0;
        printf ("Run time is %f second.\n",second);       
    }

    //print result
    printBlock(&(node->block));

    return SUDOKU_OK;
}

