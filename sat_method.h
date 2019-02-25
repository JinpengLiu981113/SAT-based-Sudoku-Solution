#include <stdlib.h>
#include "sat_list_struct.h"

#ifndef ASSIGNMENT_PROJECTS_FUNCTION_DEFINITION_H
#define ASSIGNMENT_PROJECTS_FUNCTION_DEFINITION_H

#endif //ASSIGNMENT_PROJECTS_FUNCTION_DEFINITION_H

/* 将.cnf文件中的子句和文字用链表的形式存储 */
void createList(FILE *infile, int clauses, CLAUSES *clausesCurr) {
    int number = 0, notes = 0, characterIndex = 0, clauseIndex = 0;
    for (int num = 0; num < clauses; num++) {
        addClause(clausesCurr, clauseIndex + 1);
        clausesCurr = clausesCurr->next;
        CHARACTERS *charactersHead = createCharacter(), *charactersCurr = charactersHead;
        fscanf(infile, "%d", &number);
        while (number != 0) {
            addCharacter(charactersCurr, number, characterIndex + 1);
            charactersCurr = charactersCurr->next;
            characterIndex++;
            notes++;
            fscanf(infile, "%d", &number);
        }
        clauseIndex++;
        characterIndex = 0;
        linkCharacters(clausesCurr, charactersHead, notes);
        notes = 0;
    }
}

/* 在屏幕上输出这个链表 */
void printScreen(const CLAUSES *clausesHead) {
    CLAUSES *clacurr = clausesHead->next;
    while (clacurr != NULL) {
        printf("%5d", clacurr->length);
        CHARACTERS *chacurr = clacurr->head->next;
        while (chacurr != NULL) {
            printf("%5d", chacurr->number);
            chacurr = chacurr->next;
        }
        clacurr = clacurr->next;
        printf("\n");
    }
}

/* 加入假设层 */
void addAssumptionLayer(CLAUSES *clausesHead) {
    CLAUSES *clausesCurr = clausesHead, *clausesTemp = clausesHead->next;
    addClause(clausesCurr, 0);
    clausesCurr = clausesCurr->next;
    clausesCurr->enable = false;
    CHARACTERS *charactersHead = createCharacter();
    linkCharacters(clausesCurr, charactersHead, 0);
    clausesCurr->next = clausesTemp;
}

/* 删除假设层的文字 */
void deleteTheWord(CLAUSES *clausesHead) {
    CHARACTERS *charactersCurr = clausesHead->next->head;
    free(charactersCurr->next);
    charactersCurr->next = NULL;
}

/* 选择一个文字，以让其成为单子句 */
int getLiteral(CLAUSES *clausesHead) {
    CLAUSES *clausesCurr = clausesHead->next->next;
    int number = 0;
    while (clausesCurr != NULL) {
        if (clausesCurr->enable == true) {
            CHARACTERS *charactersCurr = clausesCurr->head->next;
            while (charactersCurr != NULL) {
                if (charactersCurr->enable == true) {
                    number = charactersCurr->number;
                    break;
                }
                charactersCurr = charactersCurr->next;
            }
        }
        if(number != 0)
            break;
        clausesCurr = clausesCurr->next;
    }
    return number;
}

/* 把已选择的文字变成一个单子句 */
void chooseAWord(CLAUSES *clausesHead, int keyword) {
    CLAUSES *clacurr = clausesHead->next;
    CHARACTERS *charactersCurr = clacurr->head;
    addCharacter(charactersCurr, keyword, 1);
    clacurr->enable = true;
    clacurr->length = 1;
}

/* 把刚才选择的单子句取反 */
void reverseTheWord(CLAUSES *clausesHead, int keyword) {
    CLAUSES *clacurr = clausesHead->next;
    CHARACTERS *charactersCurr = clacurr->head->next;
//    charactersCurr->initialIndex = 1;
    charactersCurr->number = -keyword;
    clacurr->enable = true;
}

/* 恢复对链表的改变 */
void recoverChanges(CLAUSES *clausesHead, CHANGELOG *changelogHead) {
    CHANGELOG *changelogCurr = changelogHead->next;
    while (changelogCurr != NULL) {
        CLAUSES *clausesCurr = clausesHead->next;
        while (clausesCurr->initialIndex != changelogCurr->clauseIndex)
            clausesCurr = clausesCurr->next;
        enableClause(clausesCurr);
        CHARACTERS *charactersCurr = clausesCurr->head->next;
        while (charactersCurr->initialIndex != changelogCurr->characterIndex)
            charactersCurr = charactersCurr->next;
        enableCharacter(charactersCurr);
        if(clausesCurr == clausesHead->next){
            clausesCurr->length = 1;
        }
        else{
            clausesCurr->length++;
        }
        changelogCurr = changelogCurr->next;
    }
}

/* 判断是否所有的句子都被去除了，(S为空集) */
bool allDisabled(CLAUSES *clausesHead) {
    CLAUSES *clausesCurr = clausesHead->next;
    while (clausesCurr != NULL) {
        if (clausesCurr->enable)
            return false;
        clausesCurr = clausesCurr->next;
    }
    return true;
}

/* 链表中有单子句 */
bool hasSingleClause(const CLAUSES *clausesHead) {
    CLAUSES *clacurr = clausesHead->next;
    bool hasSingleClause = false;
    while (clacurr != NULL) {
        if (isUnitClause(clacurr))
            hasSingleClause = true;
        clacurr = clacurr->next;
    }
    return hasSingleClause;
}

/* 找到单子句对应的文字 */
int getSingleLiteral(CLAUSES *clausesHead) {
    CLAUSES *clacurr = clausesHead->next;
    int number = 0;
    while (clacurr != NULL) {
        if (isUnitClause(clacurr)) {
            CHARACTERS *charactersCurr = clacurr->head->next;
            while (charactersCurr != NULL) {
                if (charactersCurr->enable == true)
                    number = charactersCurr->number;
                charactersCurr = charactersCurr->next;
            }
            break;
        }
        clacurr = clacurr->next;
    }
    return number;
}

/* DPLL算法 */
bool DPLL(CLAUSES *clausesHead, STATUSCODE *statusCodeHead) {
    CHANGELOG *changelogHead = createChangeLog(), *changelogCurr = changelogHead;
    while (hasSingleClause(clausesHead)) {
        int singleWord = 0;
        singleWord = getSingleLiteral(clausesHead);
//        printf("%d\n", singleWord);
//        printScreen(clausesHead);
        addStatusCode(statusCodeHead, singleWord);
        CLAUSES *clacurr = clausesHead->next;
        while (clacurr != NULL) {
            if (clacurr->enable == true) {
                CHARACTERS *chacurr = clacurr->head->next;
                while (chacurr != NULL) {
                    if (chacurr->enable == true) {
                        if (chacurr->number == singleWord) {
                            disableClause(clacurr);
                            chacurr = clacurr->head->next;
                            while (chacurr != NULL) {
                                if (chacurr->enable == true) {
                                    disableCharacter(chacurr);
                                    addChangelog(changelogCurr, clacurr->initialIndex, chacurr->initialIndex);
                                    changelogCurr = changelogCurr->next;
                                    clacurr->length--;
                                }
                                chacurr = chacurr->next;
                            }
                        } else if (chacurr->number == -singleWord) {
                            disableCharacter(chacurr);
                            addChangelog(changelogCurr, clacurr->initialIndex, chacurr->initialIndex);
                            changelogCurr = changelogCurr->next;
                            clacurr->length--;
                            chacurr = chacurr->next;
                        } else {
                            chacurr = chacurr->next;
                        }
                    } else {
                        chacurr = chacurr->next;
                    }
                }
            }
            clacurr = clacurr->next;
        }
        if (allDisabled(clausesHead)) {
            /**/
            recoverChanges(clausesHead, changelogHead);
            /**/
            destroyChangelog(changelogHead);
            return true;
        }
        clacurr = clausesHead->next;
        while (clacurr != NULL) {
            if (clacurr->length == 0 && clacurr->enable == true) {
                recoverChanges(clausesHead, changelogHead);
                destroyChangelog(changelogHead);
                return false;
            }
            clacurr = clacurr->next;
        }
    }
    int literal = getLiteral(clausesHead);
    if (clausesHead->next->head->next != NULL)
        deleteTheWord(clausesHead);
    chooseAWord(clausesHead, literal);
    bool satisfiable;
    satisfiable = DPLL(clausesHead, statusCodeHead);
    if (satisfiable) {
        /**/
        recoverChanges(clausesHead, changelogHead);
        /**/
        destroyChangelog(changelogHead);
        return true;
    }
    reverseTheWord(clausesHead, literal);
    satisfiable = DPLL(clausesHead, statusCodeHead);
    if (satisfiable) {
        /**/
        recoverChanges(clausesHead, changelogHead);
        /**/
        destroyChangelog(changelogHead);
        return true;
    } else {
        recoverChanges(clausesHead, changelogHead);
        destroyChangelog(changelogHead);
        return false;
    }
}
