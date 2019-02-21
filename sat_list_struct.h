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

CLAUSES *createClause() {
    /* 创建一个空子句 */
    CLAUSES *clausesNew = (CLAUSES *) malloc(sizeof(CLAUSES));
    clausesNew->head = NULL;
    clausesNew->next = NULL;
    return clausesNew;
}

void addClause(CLAUSES *clausesCurr, int index) {
    /* 加入一个子句，指针向后移动到下一个子句 */
    CLAUSES *clausesNew = (CLAUSES *) malloc(sizeof(CLAUSES));
    clausesNew->length = 0;
    clausesNew->initialIndex = index;
    clausesNew->next = NULL;
    clausesNew->head = NULL;
    clausesNew->enable = true;
    clausesCurr->next = clausesNew;
}

void disableClause(CLAUSES *clausesCurr) {
    /* 删除一个子句 */
    clausesCurr->enable = false;
}

void enableClause(CLAUSES *clausesCurr) {
    clausesCurr->enable = true;
}

void destroyClauses(CLAUSES *clausesHead) {
    /* 删除所有子句 */
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

bool isUnitClause(CLAUSES *clausesCurr) {
    return clausesCurr->length == 1 && clausesCurr->enable == true;
}

CHARACTERS *createCharacter() {
    /* 创建一个空子句 */
    CHARACTERS *charactersNew = (CHARACTERS *) malloc(sizeof(CHARACTERS));
    charactersNew->next = NULL;
    return charactersNew;
}

void addCharacter(CHARACTERS *charactersCurr, int number, int index) {
    /* 加入一个子句，指针向后移动到下一个子句 */
    CHARACTERS *charactersNew = (CHARACTERS *) malloc(sizeof(CHARACTERS));
    charactersNew->next = NULL;
    charactersNew->number = number;
    charactersNew->initialIndex = index;
    charactersNew->enable = true;
    charactersCurr->next = charactersNew;
}

void disableCharacter(CHARACTERS *charactersCurr) {
    /* 删除一个子句 */
    charactersCurr->enable = false;
}

void enableCharacter(CHARACTERS *charactersCurr) {
    charactersCurr->enable = true;
}

void linkCharacters(CLAUSES *clausesCurr, CHARACTERS *charactersHead, int length) {
    clausesCurr->head = charactersHead;
    clausesCurr->length = length;
}

typedef struct statusCode {
    int number;
    int status;
    struct statusCode *next;
} STATUSCODE;

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

void addStatusCode(STATUSCODE *statusCodeHead, int number) {
    STATUSCODE *statusCodeNew = (STATUSCODE *) malloc(sizeof(STATUSCODE)), *statusCodeCurr = statusCodeHead;
    statusCodeNew->number = abs(number);
    statusCodeNew->status = number > 0 ? 1 : -1;
    statusCodeNew->next = NULL;
    while (statusCodeCurr->next != NULL) {
        if (statusCodeCurr->number == abs(number)) {
            statusCodeCurr->status = -statusCodeCurr->status;
            break;
        }
        statusCodeCurr = statusCodeCurr->next;
    }
    if (statusCodeCurr->next == NULL && statusCodeCurr->number != abs(number))
        statusCodeCurr->next = statusCodeNew;
}

STATUSCODE *sortStatusCode(STATUSCODE *statusCodeHead) {
    STATUSCODE *statusCodeCurr = statusCodeHead->next;
    int length = 0, num = 0;
    while(statusCodeCurr!= NULL){
        length++;
        statusCodeCurr = statusCodeCurr->next;
    }
    int numberCode[length], statusCode[length], temp = 0;
    statusCodeCurr = statusCodeHead->next;
    while(statusCodeCurr!= NULL){
        numberCode[num] = statusCodeCurr->number;
        statusCode[num] = statusCodeCurr->status;
        statusCodeCurr = statusCodeCurr->next;
        num++;
    }
    for(int i=0;i<length-1;i++){
        for(int j=i+1;j<length;j++){
            if(numberCode[i]>numberCode[j]){
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
    for(int i=0;i<length;i++){
        addStatusCode(newHead, numberCode[i]*statusCode[i]);
    }
    return newHead;
}

void destroyStatusCode(STATUSCODE *statusCodeHead) {
    STATUSCODE *statusCodeCurr = statusCodeHead, *statusCodeTemp;
    while (statusCodeCurr != NULL) {
        statusCodeTemp = statusCodeCurr->next;
        free(statusCodeCurr);
        statusCodeCurr = statusCodeTemp;
    }
}

typedef struct changelog {
    int clauseIndex;
    int characterIndex;
    struct changelog *next;
} CHANGELOG;

CHANGELOG *createChangeLog(void) {
    /* 创建change log */
    CHANGELOG *changelogNew = (CHANGELOG *) malloc(sizeof(CHANGELOG));
    changelogNew->next = NULL;
    return changelogNew;
}

void addChangelog(CHANGELOG *changelogCurr, int clauseIndex, int characterIndex) {
    /* 加入一个change log */
    CHANGELOG *changelogNew = (CHANGELOG *) malloc(sizeof(CHANGELOG));
    changelogNew->next = NULL;
    changelogNew->clauseIndex = clauseIndex;
    changelogNew->characterIndex = characterIndex;
    changelogCurr->next = changelogNew;
}

void destroyChangelog(CHANGELOG *changelogHead) {
    CHANGELOG *changelogCurr = changelogHead, *changelogTemp;
    while (changelogCurr != NULL) {
        changelogTemp = changelogCurr->next;
        free(changelogCurr);
        changelogCurr = changelogTemp;
    }
}
