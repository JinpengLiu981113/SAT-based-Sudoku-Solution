#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "sat_method.h"
#include "sat_fileio.h"
#include "sudoku_method.h"
#include "sudoku_fileio.h"

void solveForSAT() {
    /* .cnf与.res文件名处理 */
    char *fileName = (char *) malloc(200 * sizeof(char));
    char *outFileName = (char *) malloc(200 * sizeof(char));
    dealWithFileName(fileName, outFileName);

    /* 记录时间 */
    clock_t t0 = clock();

    /* 对.cnf文件进行处理，提取子句与文字个数信息 */
    FILE *infile = fopen(fileName, "r");
    if (infile == NULL) {
        printf("File error!");
        exit(0);
    }
    int clauses = 0, characters = 0;
    CLAUSES *clausesHead = createClause(), *clausesCurr = clausesHead;
    STATUSCODE *statusCodeHead = createStatusCode();
    STATUSCODE *newHead;
    getMetrics(infile, &clauses, &characters);

    /* 处理文件, 提取其中的文字 */
    createList(infile, clauses, clausesCurr);
    fclose(infile);
    free(fileName);

    /* 输出链表 */
    printScreen(clausesHead);
    addAssumptionLayer(clausesHead);

    /* DPLL算法 */
    bool satisfiable;
    satisfiable = DPLL(clausesHead, statusCodeHead);
    printf("%d\n", satisfiable);
    newHead = sortStatusCode(statusCodeHead);

    /* 释放内存 */
    destroyClauses(clausesHead);
    destroyStatusCode(statusCodeHead);

    /* 记录时间，计算所用时间，将sat结果写入.res文件 */
    FILE *outfile = fopen(outFileName, "w");
    clock_t t1 = clock();
    satOutFile(outfile, newHead, satisfiable, t1 - t0);
    fclose(outfile);
    free(outFileName);
    destroyStatusCode(newHead);

    /* 记录时间，计算所用时间 */
    clock_t t2 = clock();
    printf("Time used this turn: %5ld\n", t2 - t0);
}

void sudokuGame() {
    /* 记录时间 */
    clock_t t0 = clock();

    /* 生成数独 */
    int *sudokuArray;
    int **sudoku;
    sudokuArray = shuffleSudokuArray();
    sudoku = createSudoku(sudokuArray);
    free(sudokuArray);
    generateSudokuGame(sudoku);
    printSudoku(sudoku);

    /* 将数独转换成sat问题 */
    CLAUSE *clauseHead;
    clauseHead = translateSudokuToSAT(sudoku);

    /* 生成.cnf文件 */
    char outSudokuFileName[100] = "c:\\Users\\liujinpeng\\Desktop\\sudoku.cnf";
    FILE *outFile = fopen(outSudokuFileName, "w");
    tocnf(outFile, clauseHead, sudoku);
    fclose(outFile);
    destroySudokuClauses(clauseHead);

    /* 读入sudoku.cnf文件，生成链表 */
    FILE *infile = fopen(outSudokuFileName, "r");
    CLAUSES *clausesHead = createClause(), *clausesCurr = clausesHead;
    int clauses = 0, characters = 0;
    getMetrics(infile, &clauses, &characters);
    createList(infile, clauses, clausesCurr);
    addAssumptionLayer(clausesHead);
    fclose(infile);

    /* 选择游戏模式 */
    int choice = 0;
    printf("Play or auto_solve?(1 to play, 2 to auto_solve, 3 to quit)\n");
    scanf("%d", &choice);
    getchar();
    if(choice == 1){
        /* 用户自行求解 */
        solveSudoku(sudoku, clausesHead);
    }
    else if(choice == 2){
        /* 用DPLL求解数独 */
        solveSudokuWithDPLL(sudoku, clausesHead);
        printSudoku(sudoku);
    }
    destroyClauses(clausesHead);
    free(sudoku);

    /* 记录时间，计算所用时间 */
    clock_t t1 = clock();
    printf("Time used this turn: %5ld\n", t1 - t0);
}
