#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "sudoku_list_struct.h"

#ifndef ASSIGNMENT_PROJECTS_SUDOKU_METHOD_H
#define ASSIGNMENT_PROJECTS_SUDOKU_METHOD_H

#endif //ASSIGNMENT_PROJECTS_SUDOKU_METHOD_H

/* 随机打乱1-9的数组 */
int *shuffleSudokuArray() {
    srand((unsigned) clock());
    int *sudokuArrayShuffled = (int *) malloc(9 * sizeof(int));
    int numbers[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    for (int i = 0; i < 9; i++) {
        int num = rand() % (9 - i);
        sudokuArrayShuffled[i] = numbers[num];
        numbers[num] = numbers[8 - i];
    }
    return sudokuArrayShuffled;
}

/* 通过对中间的九宫格进行行列变换并填充到其他九宫格中生成完整数独 */
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

/* 按2/3的概率逐空挖，最后数独剩余数字个数多于17个的概率在95%以上 */
void generateSudokuGame(int **sudoku) {
    int level = 0;
    int number = 30, dig = 0;
    printf("Pick a level(1 for easy, 2 for intermediate, 3 for hard): \n");
    scanf("%d", &level);
    if (level == 1)
        dig = 10;
    else if (level == 2)
        dig = 15;
    else
        dig = 20;
    srand((unsigned) clock());
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (rand() % number < dig)
                sudoku[i][j] = 0;
        }
    }
}

/* 打印出数独格局，(Clion中美观) */
void printSudoku(int **sudoku) {
    for (int i = 0; i < 9; i++)
        printf("+-------");
    printf("+\n");
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (sudoku[i][j] != 0)
                printf("|   %d   ", sudoku[i][j]);
            else
                printf("|       ");
        }
        printf("|\n");
        for (int j = 0; j < 9; j++) {
            printf("+-------");
        }
        printf("+\n");
    }
}

/* 打印出数独格局，(dev-c++中美观) */
//void printSudoku(int **sudoku) {
//    for (int i = 0; i < 9; i++)
//        printf("+---");
//    printf("+\n");
//    for (int i = 0; i < 9; i++) {
//        for (int j = 0; j < 9; j++) {
//            if (sudoku[i][j] != 0)
//                printf("| %d ", sudoku[i][j]);
//            else
//                printf("|   ");
//        }
//        printf("|\n");
//        for (int j = 0; j < 9; j++) {
//            printf("+---");
//        }
//        printf("+\n");
//    }
//}

/* 打印出数独转化成SAT后生成的链表 */
//void printSudokuScreen(const CLAUSE *clauseHead) {
//    CLAUSE *clacurr = clauseHead->next;
//    while (clacurr != NULL) {
//        printf("%5d", clacurr->length);
//        CHARACTER *chacurr = clacurr->right->next;
//        while (chacurr != NULL) {
//            printf("%5d", chacurr->number);
//            chacurr = chacurr->next;
//        }
//        clacurr = clacurr->next;
//        printf("\n");
//    }
//}

/* 将三个数字组成三位数 */
int getNumber(int i, int j, int k) {
    return 100 * i + 10 * j + k;
}

/* 将生成的数独转化成SAT问题 */
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
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            for (int k = 0; k < 9; k++) {
                if (literal[i][j][k] == 1) {
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
                for (int n = k + 1; n < 9; n++) {
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
    for (int i = 0; i < 9; i++) {
        for (int k = 0; k < 9; k++) {
            for (int j = 0; j < 8; j++) {
                for (int m = j + 1; m < 9; m++) {
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
    for (int i = 0; i < 9; i++) {
        for (int k = 0; k < 9; k++) {
            for (int j = 0; j < 8; j++) {
                for (int m = j + 1; m < 9; m++) {
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
                        addSudokuCharacter(characterCurr, -getNumber(m * 3 + i / 3 + 1, n * 3 + i % 3 + 1, k + 1));
                        characterCurr = characterCurr->next;
                        addSudokuCharacter(characterCurr, -getNumber(m * 3 + j / 3 + 1, n * 3 + j % 3 + 1, k + 1));
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
            for (int k = 0; k < 9; k++) {
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

/* 用DPLL自动求解数独 */
void solveSudokuWithDPLL(int **sudoku, CLAUSES *clausesHead) {
    STATUSCODE *statusCodeHead = createStatusCode();
    STATUSCODE *newHead;
    /* DPLL算法 */
    DPLL(clausesHead, statusCodeHead);
    newHead = sortStatusCode(statusCodeHead);
    STATUSCODE *newCurr = newHead->next;
    while (newCurr != NULL) {
        if (newCurr->status > 0) {
            int row = newCurr->number / 100 - 1;
            int col = (newCurr->number % 100) / 10 - 1;
            int num = newCurr->number % 10;
            sudoku[row][col] = num;
        }
        newCurr = newCurr->next;
    }
    /* 释放内存 */
    destroyStatusCode(statusCodeHead);
    destroyStatusCode(newHead);
}

/* 玩家自己求解数独 */
void solveSudoku(int **sudoku, CLAUSES *clausesHead) {
    STATUSCODE *statusCodeHead = createStatusCode();
    int number = 0, row = 0, col = 0, total = 0;
    /* 记录目前数独中已填好的数的个数 */
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (sudoku[i][j] != 0)
                total++;
        }
    }
    /* 共填81个 */
    while (total < 81) {
        printf("Enter a number of three digits(for example 156, 584), 0 to quit: \n");
        scanf("%d", &number);
        if (number == 0)
            break;
        row = number / 100 - 1;
        col = (number % 100) / 10 - 1;
        if (sudoku[row][col] != 0) {
            printSudoku(sudoku);
            printf("Number already exists, retry please: \n");
        } else {
            CLAUSES *clausesCurr = clausesHead->next, *temp;
            int lineIndex = 0;
            while (clausesCurr->next != NULL) {
                lineIndex++;
                clausesCurr = clausesCurr->next;
            }
            clausesHead->next->enable = false;
            addClause(clausesCurr, lineIndex + 1);
            clausesCurr = clausesCurr->next;
            CHARACTERS *charactersHead = createCharacter();
            CHARACTERS *newCharacter = charactersHead;
            addCharacter(newCharacter, number, 1);
            linkCharacters(clausesCurr, charactersHead, 1);
            if (DPLL(clausesHead, statusCodeHead)) {
                sudoku[row][col] = number % 10;
                printSudoku(sudoku);
                total++;
            } else {
                clausesCurr = clausesHead->next;
                temp = clausesHead;
                while (clausesCurr->next != NULL) {
                    clausesCurr = clausesCurr->next;
                    temp = temp->next;
                }
                temp->next = NULL;
                free(clausesCurr->head->next);
                free(clausesCurr->head);
                free(clausesCurr);
                printSudoku(sudoku);
                printf("Incorrect, retry please: \n");
            }
        }
    }
}
