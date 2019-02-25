# SAT-based-sudoku-solution

----

## What is SAT

[Here](https://en.wikipedia.org/wiki/Boolean_satisfiability_problem) to check for the definition of SAT

----

## How to solve SAT problem

We use the DPLL algorithm to solve SAT problem recursively

```
DPLL(S){
    /* S refers to a set of clauses, if it's satisfiable, return true. Otherwise false. */
    while(unit clause exists in S) {
        pick a unit clause from S;
        simplify S using the unit clause;
        if S = Φ return(true);
        else if (empty clause exists in S) return(false);
    }
    select a literal v from S;
    if DPLL(S∪v) return(TURE);
    return DPLL(S∪¬v);
}
```

----

## Convert sudoku game into SAT problem

We first create a sudoku game like this

![sudoku](https://github.com/JinpengLiu981113/SAT-based-sudoku-solution/blob/master/sudoku.png)

Then we tanslate the sudoku game into SAT in this way

```c
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
```

After we convert the sudoku game into SAT, we are able to solve it using DPLL algorithm
