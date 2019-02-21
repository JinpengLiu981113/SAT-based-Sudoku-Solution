#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "sudoku_list_struct.h"

#ifndef ASSIGNMENT_PROJECTS_SUDOKU_METHOD_H
#define ASSIGNMENT_PROJECTS_SUDOKU_METHOD_H

#endif //ASSIGNMENT_PROJECTS_SUDOKU_METHOD_H

int *shuffleSudokuArray() {
    srand((unsigned) clock());
    int *sudokuArrayShuffled = (int *) malloc(9 * sizeof(int));
    int numbers[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    for (int i = 0; i < 9; i++) {
        int num = rand() % (9 - i);
        sudokuArrayShuffled[i] = numbers[num];
        numbers[num] = numbers[9 - i];
    }
    return sudokuArrayShuffled;
}

int **createSudoku(int *sudokuArray) {
    int **newSudoku = (int **) malloc(81 * sizeof(int));
    for (int i = 0; i < 9; i++) {
        int *array = (int *) malloc(9 * sizeof(int));
        newSudoku[i] = array;
    }
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++)
            newSudoku[i][j] = 0;
    }
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            newSudoku[i + 3][j + 3] = sudokuArray[i + j * 3];
        }
    }
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            newSudoku[i + 3][j] = newSudoku[(i + 1) % 3 + 3][j + 3];
            newSudoku[i + 3][j + 6] = newSudoku[(i + 2) % 3 + 3][j + 3];
        }
    }
    for (int j = 0; j < 3; j++) {
        for (int i = 0; i < 3; i++) {
            newSudoku[i][j] = newSudoku[i + 3][(j + 1) % 3];
            newSudoku[i][j + 3] = newSudoku[i + 3][(j + 1) % 3 + 3];
            newSudoku[i][j + 6] = newSudoku[i + 3][(j + 1) % 3 + 6];
            newSudoku[i + 6][j] = newSudoku[i + 3][(j + 2) % 3];
            newSudoku[i + 6][j + 3] = newSudoku[i + 3][(j + 2) % 3 + 3];
            newSudoku[i + 6][j + 6] = newSudoku[i + 3][(j + 2) % 3 + 6];
        }
    }
    return newSudoku;
}

void printSudoku(int **sudoku) {
    for (int i = 0; i < 9; i++)
        printf("+---");
    printf("+\n");
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (sudoku[i][j] != 0)
                printf("| %d ", sudoku[i][j]);
            else
                printf("|   ");
        }
        printf("|\n");
        for (int j = 0; j < 9; j++) {
            printf("+---");
        }
        printf("+\n");
    }
}

void generateSudokuGame(int **sudoku) {
    srand((unsigned) clock());
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (rand() % 30 < 20)
                sudoku[i][j] = 0;
        }
    }
}

int getNumber(int i, int j, int k) {
    return 100 * i + 10 * j + k;
}

void printSudokuScreen(const CLAUSE *clauseHead) {
    CLAUSE *clacurr = clauseHead->next;
    while (clacurr != NULL) {
        printf("%5d", clacurr->length);
        CHARACTER *chacurr = clacurr->right->next;
        while (chacurr != NULL) {
            printf("%5d", chacurr->number);
            chacurr = chacurr->next;
        }
        clacurr = clacurr->next;
        printf("\n");
    }
}

CLAUSE *translateSudokuToSAT(int **sudoku) {
    CLAUSE *clauseHead = createSudokuClause(), *clauseCurr = clauseHead;
    int literal[9][9][9];
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            for (int k = 0; k < 9; k++) {
                literal[i][j][k] = 0;
            }
        }
    }
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (sudoku[i][j] != 0)
                literal[i][j][sudoku[i][j] - 1] = 1;
        }
    }
    /* 数独中已填入的值 */
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            for(int k=0;k<9;k++){
                if(literal[i][j][k] == 1){
                    addSudokuClause(clauseCurr);
                    clauseCurr = clauseCurr->next;
                    CHARACTER *characterHead = createSudokuCharacter(), *characterCurr = characterHead;
                    addSudokuCharacter(characterCurr, getNumber(i + 1, j + 1, k + 1));
                    linkSudokuCharacters(clauseCurr, characterHead, 1);
                }
            }
        }
    }
    /* 数独每空不可能有两个数填入 */
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            for (int k = 0; k < 8; k++) {
                for(int n = k+1;n<9;n++){
                    addSudokuClause(clauseCurr);
                    clauseCurr = clauseCurr->next;
                    CHARACTER *characterHead = createSudokuCharacter(), *characterCurr = characterHead;
                    addSudokuCharacter(characterCurr, -getNumber(i + 1, j + 1, k + 1));
                    characterCurr = characterCurr->next;
                    addSudokuCharacter(characterCurr, -getNumber(i + 1, j + 1, n + 1));
                    linkSudokuCharacters(clauseCurr, characterHead, 2);
                }
            }
        }
    }
    /* 数独每行中两两互不相同 */
    for(int i=0;i<9;i++){
        for(int k=0;k<9;k++){
            for(int j=0;j<8;j++){
                for(int m=j+1;m<9;m++){
                    addSudokuClause(clauseCurr);
                    clauseCurr = clauseCurr->next;
                    CHARACTER *characterHead = createSudokuCharacter(), *characterCurr = characterHead;
                    addSudokuCharacter(characterCurr, -getNumber(i + 1, j + 1, k + 1));
                    characterCurr = characterCurr->next;
                    addSudokuCharacter(characterCurr, -getNumber(i + 1, m + 1, k + 1));
                    linkSudokuCharacters(clauseCurr, characterHead, 2);
                }
            }
        }
    }
    /* 数独每列中两两互不相同 */
    for(int i=0;i<9;i++){
        for(int k=0;k<9;k++){
            for(int j=0;j<8;j++){
                for(int m=j+1;m<9;m++){
                    addSudokuClause(clauseCurr);
                    clauseCurr = clauseCurr->next;
                    CHARACTER *characterHead = createSudokuCharacter(), *characterCurr = characterHead;
                    addSudokuCharacter(characterCurr, -getNumber(j + 1, i + 1, k + 1));
                    characterCurr = characterCurr->next;
                    addSudokuCharacter(characterCurr, -getNumber(m + 1, i + 1, k + 1));
                    linkSudokuCharacters(clauseCurr, characterHead, 2);
                }
            }
        }
    }
    /* 数独每块中两两互不相同 */
    for (int m = 0; m < 3; m++) {
        for (int n = 0; n < 3; n++) {
            for (int k = 0; k < 9; k++) {
                for (int i = 0; i < 8; i++) {
                    for (int j = i + 1; j < 9; j++) {
                        addSudokuClause(clauseCurr);
                        clauseCurr = clauseCurr->next;
                        CHARACTER *characterHead = createSudokuCharacter(), *characterCurr = characterHead;
                        addSudokuCharacter(characterCurr, -getNumber(m * 3 + i/3 + 1, n * 3 + i%3 + 1, k + 1));
                        characterCurr = characterCurr->next;
                        addSudokuCharacter(characterCurr, -getNumber(m * 3 + j/3 + 1, n * 3 + j%3 + 1, k + 1));
                        linkSudokuCharacters(clauseCurr, characterHead, 2);
                    }
                }
            }
        }
    }
    /* 数独每空必须包含1-9中的一个 */
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            int number = 0;
            addSudokuClause(clauseCurr);
            clauseCurr = clauseCurr->next;
            CHARACTER *characterHead = createSudokuCharacter(), *characterCurr = characterHead;
            for (int k = 0; k < 9; k++) {
                number++;
                addSudokuCharacter(characterCurr, getNumber(i + 1, j + 1, k + 1));
                characterCurr = characterCurr->next;
            }
            linkSudokuCharacters(clauseCurr, characterHead, number);
        }
    }
    /* 数独每行必须包含1-9 */
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            int number = 0;
            addSudokuClause(clauseCurr);
            clauseCurr = clauseCurr->next;
            CHARACTER *characterHead = createSudokuCharacter(), *characterCurr = characterHead;
            for (int k = 0; k < 9; k++) {
                number++;
                addSudokuCharacter(characterCurr, getNumber(i + 1, k + 1, j + 1));
                characterCurr = characterCurr->next;
            }
            linkSudokuCharacters(clauseCurr, characterHead, number);
        }
    }
    /* 数独每列必须包含1-9 */
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            int number = 0;
            addSudokuClause(clauseCurr);
            clauseCurr = clauseCurr->next;
            CHARACTER *characterHead = createSudokuCharacter(), *characterCurr = characterHead;
            for (int k = 0; k < 9; k++) {
                number++;
                addSudokuCharacter(characterCurr, getNumber(k + 1, i + 1, j + 1));
                characterCurr = characterCurr->next;
            }
            linkSudokuCharacters(clauseCurr, characterHead, number);
        }
    }
    /* 数独每块必须包含1-9 */
    for (int m = 0; m < 3; m++) {
        for (int n = 0; n < 3; n++) {
            for (int k = 0; k < 3; k++) {
                int number = 0;
                addSudokuClause(clauseCurr);
                clauseCurr = clauseCurr->next;
                CHARACTER *characterHead = createSudokuCharacter(), *characterCurr = characterHead;
                for (int i = 0; i < 9; i++) {
                    number++;
                    addSudokuCharacter(characterCurr, getNumber(m * 3 + i / 3 + 1, n * 3 + i % 3 + 1, k + 1));
                    characterCurr = characterCurr->next;
                }
                linkSudokuCharacters(clauseCurr, characterHead, number);
            }
        }
    }
    int lines = 0;
    clauseCurr = clauseHead->next;
    while (clauseCurr != NULL) {
        lines++;
        clauseCurr = clauseCurr->next;
    }
    clauseHead->length = lines;
    return clauseHead;
}

void solveSudokuWithDPLL(char *outFileName, int **sudoku){
    FILE * infile = fopen(outFileName, "r");

    /* 对.cnf文件进行处理，提取子句与文字个数信息 */
    if (infile == NULL) {
        printf("File error!");
        exit(0);
    }
    int clauses = 0, characters = 0;
    CLAUSES *clausesHead = createClause(), *clausesCurr = clausesHead;
    STATUSCODE *statusCodeHead = createStatusCode(), *statusCodeCurr = statusCodeHead;
    STATUSCODE *newHead = createStatusCode(), *newCurr = newHead->next;
    getMetrics(infile, &clauses, &characters);

    /* 处理文件, 提取其中的文字 */
    createList(infile, clauses, clausesCurr);
    fclose(infile); // 关闭文件

    /* 输出链表 */
    addAssumptionLayer(clausesHead);

    /* DPLL算法 */
    bool satisfiable;
    satisfiable = DPLL(clausesHead, statusCodeCurr);
    printf("%d\n", satisfiable);
    newHead = sortStatusCode(statusCodeHead);
    while(newCurr!=NULL){
        if(newCurr->status>0){
            int row = newCurr->number/100-1;
            int col = (newCurr->number%100)/10-1;
            int num = newCurr->number%10;
            sudoku[row][col] = num;
        }
        newCurr = newCurr->next;
    }

    /* 释放内存 */
    destroyClauses(clausesHead);
    destroyStatusCode(statusCodeHead);
    destroyStatusCode(newHead);
}

void solveSudoku(int **sudoku, int **sudokuCheck){
    int number = 0, row = 0, col = 0, total = 0;
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            if(sudoku[i][j] != 0)
                total++;
        }
    }
    while(total < 81){
        printf("Enter a number of three digits(for example 156, 584), 0 to quit: \n");
        scanf("%d", &number);
        if(number == 0)
            break;
        row = number/100-1;
        col = (number%100)/10-1;
        if(sudokuCheck[row][col] != number%10){
            printf("Incorrect, retry please: \n");
            printSudoku(sudoku);
        }
        else{
            sudoku[row][col] = number%10;
            printSudoku(sudoku);
            total++;
        }
    }
}

/* windows.h */
//void gotoxy(int x, int y)
//{
//    ++x,++y;
//    COORD pos;
//    pos.X = x;
//    pos.Y = y;
//    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
//}
