#ifndef ASSIGNMENT_PROJECTS_SUDOKU_LIST_STRUCT_H
#define ASSIGNMENT_PROJECTS_SUDOKU_LIST_STRUCT_H

#endif //ASSIGNMENT_PROJECTS_SUDOKU_LIST_STRUCT_H

// 子句链表
typedef struct clauses {
    int length;
    struct clauses *next;
    struct characters *right;
} CLAUSE;

// 文字链表
typedef struct characters {
    int number;
    struct characters *next;
} CHARACTER;

CLAUSE *createSudokuClause() {
    /* 创建一个空子句 */
    CLAUSE *clauseNew = (CLAUSE *) malloc(sizeof(CLAUSE));
    clauseNew->right = NULL;
    clauseNew->next = NULL;
    return clauseNew;
}

void addSudokuClause(CLAUSE *clauseCurr) {
    /* 加入一个子句，指针向后移动到下一个子句 */
    CLAUSE *clauseNew = (CLAUSE *) malloc(sizeof(CLAUSE));
    clauseNew->length = 0;
    clauseNew->right = NULL;
    clauseNew->next = NULL;
    clauseCurr->next = clauseNew;
}

void destroySudokuClauses(CLAUSE *clauseHead) {
    /* 删除所有子句 */
    CLAUSE *clauseCurr = clauseHead, *clauseTemp;
    while (clauseCurr != NULL) {
        CHARACTER *characterCurr = clauseCurr->right, *characterTemp;
        while (characterCurr != NULL) {
            characterTemp = characterCurr->next;
            free(characterCurr);
            characterCurr = characterTemp;
        }
        clauseTemp = clauseCurr->next;
        free(clauseCurr);
        clauseCurr = clauseTemp;
    }
}

CHARACTER *createSudokuCharacter() {
    /* 创建一个空子句 */
    CHARACTER *characterNew = (CHARACTER *) malloc(sizeof(CHARACTER));
    characterNew->next = NULL;
    return characterNew;
}

void addSudokuCharacter(CHARACTER *characterCurr, int number) {
    /* 加入一个子句，指针向后移动到下一个子句 */
    CHARACTER *characterNew = (CHARACTER *) malloc(sizeof(CHARACTER));
    characterNew->next = NULL;
    characterNew->number = number;
    characterCurr->next = characterNew;
}

void linkSudokuCharacters(CLAUSE *clauseCurr, CHARACTER *characterHead, int length) {
    clauseCurr->right = characterHead;
    clauseCurr->length = length;
}
