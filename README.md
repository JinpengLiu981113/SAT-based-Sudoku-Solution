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

After we convert the sudoku game into SAT, we are able to solve it using DPLL algorithm
