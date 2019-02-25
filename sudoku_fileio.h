#include <stdio.h>
#include <string.h>

#ifndef ASSIGNMENT_PROJECTS_SUDOKU_FILEIO_H
#define ASSIGNMENT_PROJECTS_SUDOKU_FILEIO_H

#endif //ASSIGNMENT_PROJECTS_SUDOKU_FILEIO_H

/* 将数独转化成的SAT存入.cnf文件 */
void tocnf(FILE *outFile, CLAUSE *clauseHead, int **sudoku) {
    fprintf(outFile, "c sudoku \n");
    fprintf(outFile, "c \n");
    for(int i=0;i<9;i++){
        fprintf(outFile, "c ");
        for(int j=0;j<9;j++){
            if(sudoku[i][j]!=0)
                fprintf(outFile, "%d ", sudoku[i][j]);
            else
                fprintf(outFile, ". ");
        }
        fprintf(outFile, "\n");
    }
    fprintf(outFile, "c \n");
    fprintf(outFile, "p cnf %d %d\n", 729, clauseHead->length);
    CLAUSE *clauseCurr = clauseHead->next;
    while(clauseCurr!=NULL){
        CHARACTER *characterCurr = clauseCurr->right->next;
        while(characterCurr!=NULL){
            fprintf(outFile, "%d ", characterCurr->number);
            characterCurr = characterCurr->next;
        }
        fprintf(outFile, "0\n");
        clauseCurr = clauseCurr->next;
    }
}
