#include <stdlib.h>
#include <stdbool.h>

#ifndef ASSIGNMENT_PROJECTS_LIST_STRUCT_H
#define ASSIGNMENT_PROJECTS_LIST_STRUCT_H

#endif //ASSIGNMENT_PROJECTS_LIST_STRUCT_H

// 子句链表
typedef struct clausesList {
    int length;
    int initialIndex;
    bool enable;
    struct clausesList *next;
    struct charactersNode *head;
} CLAUSES;

// 文字链表
typedef struct charactersNode {
    int number;
    int initialIndex;
    bool enable;
    struct charactersNode *next;
} CHARACTERS;

/* 创建一个空子句 */
CLAUSES *createClause() {
    CLAUSES *clausesNew = (CLAUSES *) malloc(sizeof(CLAUSES));
    clausesNew->head = NULL;
    clausesNew->next = NULL;
    return clausesNew;
}

/* 加入一个子句 */
void addClause(CLAUSES *clausesCurr, int index) {
    CLAUSES *clausesNew = (CLAUSES *) malloc(sizeof(CLAUSES));
    clausesNew->length = 0;
    clausesNew->initialIndex = index;
    clausesNew->next = NULL;
    clausesNew->head = NULL;
    clausesNew->enable = true;
    clausesCurr->next = clausesNew;
}

/* 关闭对子句的访问 */
void disableClause(CLAUSES *clausesCurr) {
    clausesCurr->enable = false;
}

/* 开启对子句的访问 */
void enableClause(CLAUSES *clausesCurr) {
    clausesCurr->enable = true;
}

/* 删除所有子句 */
void destroyClauses(CLAUSES *clausesHead) {
    CLAUSES *clausesCurr = clausesHead, *clausesTemp;
    while (clausesCurr != NULL) {
        CHARACTERS *charactersCurr = clausesCurr->head, *charactersTemp;
        while (charactersCurr != NULL) {
            charactersTemp = charactersCurr->next;
            free(charactersCurr);
            charactersCurr = charactersTemp;
        }
        clausesTemp = clausesCurr->next;
        free(clausesCurr);
        clausesCurr = clausesTemp;
    }
}

/* 判断是否为单子句 */
bool isUnitClause(CLAUSES *clausesCurr) {
    return clausesCurr->length == 1 && clausesCurr->enable == true;
}

/* 创建一个空文字 */
CHARACTERS *createCharacter() {
    CHARACTERS *charactersNew = (CHARACTERS *) malloc(sizeof(CHARACTERS));
    charactersNew->next = NULL;
    return charactersNew;
}

/* 加入一个文字 */
void addCharacter(CHARACTERS *charactersCurr, int number, int index) {
    CHARACTERS *charactersNew = (CHARACTERS *) malloc(sizeof(CHARACTERS));
    charactersNew->next = NULL;
    charactersNew->number = number;
    charactersNew->initialIndex = index;
    charactersNew->enable = true;
    charactersCurr->next = charactersNew;
}

/* 关闭对文字的访问 */
void disableCharacter(CHARACTERS *charactersCurr) {
    charactersCurr->enable = false;
}

/* 开启对文字的访问 */
void enableCharacter(CHARACTERS *charactersCurr) {
    charactersCurr->enable = true;
}

/* 把一个子句中的所有文字连到子句上 */
void linkCharacters(CLAUSES *clausesCurr, CHARACTERS *charactersHead, int length) {
    clausesCurr->head = charactersHead;
    clausesCurr->length = length;
}

// 真值表
typedef struct statusCode {
    int number;
    int status;
    struct statusCode *next;
} STATUSCODE;

/* 创建真值表 */
STATUSCODE *createStatusCode() {
    STATUSCODE *statusCodeHead = (STATUSCODE *) malloc(sizeof(STATUSCODE));
    statusCodeHead->next = NULL;
    return statusCodeHead;
}

//void changeStatusCode(STATUSCODE *statusCodeHead, int number) {
//    int absNumber = abs(number);
//    STATUSCODE *statusCodeCurr = statusCodeHead->next;
//    while (statusCodeCurr->number != absNumber)
//        statusCodeCurr = statusCodeCurr->next;
//    statusCodeCurr->status = -statusCodeCurr->status;
//}

/* 添加(改变)真值 */
void addStatusCode(STATUSCODE *statusCodeHead, int number) {
    STATUSCODE *statusCodeNew = (STATUSCODE *) malloc(sizeof(STATUSCODE));
    STATUSCODE *statusCodeCurr = statusCodeHead->next, *temp = statusCodeHead;
    statusCodeNew->number = abs(number);
    statusCodeNew->status = number > 0 ? 1 : -1;
    statusCodeNew->next = NULL;
    while (statusCodeCurr != NULL) {
        if (statusCodeCurr->number == abs(number)) {
            statusCodeCurr->status = number > 0 ? 1 : -1;
            free(statusCodeNew);
            break;
        }
        temp = temp->next;
        statusCodeCurr = statusCodeCurr->next;
    }
    if (statusCodeCurr == NULL)
        temp->next = statusCodeNew;
}

/* 真值排序 */
STATUSCODE *sortStatusCode(STATUSCODE *statusCodeHead) {
    STATUSCODE *statusCodeCurr = statusCodeHead->next;
    int length = 0, num = 0;
    while (statusCodeCurr != NULL) {
        length++;
        statusCodeCurr = statusCodeCurr->next;
    }
    int numberCode[length], statusCode[length], temp = 0;
    statusCodeCurr = statusCodeHead->next;
    while (statusCodeCurr != NULL) {
        numberCode[num] = statusCodeCurr->number;
        statusCode[num] = statusCodeCurr->status;
        statusCodeCurr = statusCodeCurr->next;
        num++;
    }
    for (int i = 0; i < length - 1; i++) {
        for (int j = i + 1; j < length; j++) {
            if (numberCode[i] > numberCode[j]) {
                temp = numberCode[i];
                numberCode[i] = numberCode[j];
                numberCode[j] = temp;
                temp = statusCode[i];
                statusCode[i] = statusCode[j];
                statusCode[j] = temp;
            }
        }
    }
    STATUSCODE *newHead = createStatusCode();
    for (int i = 0; i < length; i++) {
        addStatusCode(newHead, numberCode[i] * statusCode[i]);
    }
    return newHead;
}

/* 回收空间 */
void destroyStatusCode(STATUSCODE *statusCodeHead) {
    STATUSCODE *statusCodeCurr = statusCodeHead, *statusCodeTemp;
    while (statusCodeCurr != NULL) {
        statusCodeTemp = statusCodeCurr->next;
        free(statusCodeCurr);
        statusCodeCurr = statusCodeTemp;
    }
}

// change log 便于还原链表
typedef struct changelog {
    int clauseIndex;
    int characterIndex;
    struct changelog *next;
} CHANGELOG;

/* 创建change log */
CHANGELOG *createChangeLog(void) {
    CHANGELOG *changelogNew = (CHANGELOG *) malloc(sizeof(CHANGELOG));
    changelogNew->next = NULL;
    return changelogNew;
}

/* 加入一个change log */
void addChangelog(CHANGELOG *changelogCurr, int clauseIndex, int characterIndex) {
    CHANGELOG *changelogNew = (CHANGELOG *) malloc(sizeof(CHANGELOG));
    changelogNew->next = NULL;
    changelogNew->clauseIndex = clauseIndex;
    changelogNew->characterIndex = characterIndex;
    changelogCurr->next = changelogNew;
}

/* 回收空间 */
void destroyChangelog(CHANGELOG *changelogHead) {
    CHANGELOG *changelogCurr = changelogHead, *changelogTemp;
    while (changelogCurr != NULL) {
        changelogTemp = changelogCurr->next;
        free(changelogCurr);
        changelogCurr = changelogTemp;
    }
}
