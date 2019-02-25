#include <stdio.h>
#include <string.h>

#ifndef ASSIGNMENT_PROJECTS_FILEIO_H
#define ASSIGNMENT_PROJECTS_FILEIO_H

#endif //ASSIGNMENT_PROJECTS_FILEIO_H

/* .cnf与.res文件名处理 */
void dealWithFileName(char *fileName, char *outFileName) {
    printf("Please enter a file: \n");
    fgets(fileName, 100, stdin);
    char suffix[4] = ".res";
    int filelen = strlen(fileName);
    fileName[filelen - 1] = '\0';
    for (int i = 0; i < filelen; i++)
        outFileName[i] = fileName[i];
    for (int i = filelen - 5; i < filelen - 1; i++)
        outFileName[i] = suffix[i-filelen+5];
}

void getMetrics(FILE *infile, int *clauses, int *characters) {
    char linech[100];
    int i = 6;
    /* 除去文件前几行的注释等行 */
    do {
        fgets(linech, 95, infile);
    } while (linech[0] != 'p');
    /* 处理文件, 获得文字个数信息及语句个数信息 */
    while (linech[i] != ' ') {
        (*characters) = (*characters) * 10 + (linech[i] - '0');
        i++;
    }
    i++;
    while (linech[i] != '\n') {
        (*clauses) = (*clauses) * 10 + (linech[i] - '0');
        i++;
    }
}

void satOutFile(FILE *outfile, STATUSCODE *statusCodeHead, bool satisfiable, clock_t time) {
    /* 输出信息到.cnf文件 */
    if (satisfiable) {
        STATUSCODE *statusCodeCurr = statusCodeHead->next;
        fprintf(outfile, "s %d\n", satisfiable);
        fprintf(outfile, "v ");
        while (statusCodeCurr != NULL) {
            fprintf(outfile, "%5d", statusCodeCurr->number * statusCodeCurr->status);
            statusCodeCurr = statusCodeCurr->next;
        }
        fprintf(outfile, "\n");
        fprintf(outfile, "t %5ld ms\n", time);
    } else {
        fprintf(outfile, "s %d\n", satisfiable);
        fprintf(outfile, "t %5ld ms\n", time);
    }
}
