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
    STATUSCODE *statusCodeHead = createStatusCode(), *statusCodeCurr = statusCodeHead;
    STATUSCODE *newHead = createStatusCode();
    getMetrics(infile, &clauses, &characters);

    /* 处理文件, 提取其中的文字 */
    createList(infile, clauses, clausesCurr);
    fclose(infile); // 关闭文件
    free(fileName);

    /* 输出链表 */
    printScreen(clausesHead);
    addAssumptionLayer(clausesHead);

    /* DPLL算法 */
    bool satisfiable;
    satisfiable = DPLL(clausesHead, statusCodeCurr);
    printf("%d\n", satisfiable);
    newHead = sortStatusCode(statusCodeHead);

    /* 释放内存 */
    destroyClauses(clausesHead);
    destroyStatusCode(statusCodeHead);

    /* 记录时间，计算所用时间，将sat结果写入.res文件 */
    FILE *outfile = fopen(outFileName, "w");
    clock_t t1 = clock();
    satOutFile(outfile, newHead, satisfiable, t1 - t0, characters);
    free(outFileName);
    destroyStatusCode(newHead);

    /* 记录时间，计算所用时间 */
    clock_t t2 = clock();
    printf("Time used this turn: %5ld\n", t2 - t0);
}

void sudokuGame() {
    /* 记录时间 */
//    clock_t t0 = clock();

    /* 生成数独 */
    int *sudokuArray = (int *) malloc(9 * sizeof(int));
    int **sudoku;
    int **sudokuCheck = (int **)malloc(81*sizeof(int));
    int *sudokuCheckArray[9];
    for(int i=0;i<9;i++){
        sudokuCheckArray[i] = (int *)malloc(9*sizeof(int));
    }
    sudokuArray = shuffleSudokuArray();
    sudoku = createSudoku(sudokuArray);
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            sudokuCheckArray[i][j] = sudoku[i][j];
        }
    }
    sudokuCheck = sudokuCheckArray;
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

    int choice = 0;
    printf("Play or auto_solve?(1 to play, 2 to auto_solve)\n");
    scanf("%d", &choice);
    getchar();
    if(choice == 1){
        /* 用户自行求解 */
        solveSudoku(sudoku, sudokuCheck);
    }
    else if(choice == 2){
        /* 用DPLL求解数独 */
        solveSudokuWithDPLL(outSudokuFileName, sudoku);
        printSudoku(sudoku);
    }
    free(sudoku);
    destroySudokuClauses(clauseHead);

    /* 记录时间，计算所用时间 */
//    clock_t t1 = clock();
//    printf("\n%5ld", t1 - t0);
}
