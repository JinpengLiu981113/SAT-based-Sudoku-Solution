#include "modules.c"

int main(void) {
    /* 主程序 */
    int mode = 0;
    printf("Program starting\n");
    do {
        printf("Sudoku or SAT?(1 for Sudoku, 2 for SAT, 3 to quit)\n");
        scanf("%d", &mode);
        if (mode != 1 && mode != 2 && mode != 3)
            continue;
        else if (mode == 1)
            sudokuGame();
        else if (mode == 2)
            solveForSAT();
    } while (mode != 3);
    printf("Program Quiting");

    /* 测试sat */
//    solveForSAT();

    /* 测试数独 */
//    sudokuGame();

    return 0;
}