#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ENTRIES 50000
#define MAX_LINELEN 5000

typedef struct Record
{
    char timestamp[24];
    float value;
} Record;

Record records[MAX_ENTRIES];            // array that will hold the records
void appendInArr(Record *rec, FILE *f); // seperates the timestamps & vals

int main()
{
    FILE *rcs;
    rcs = fopen("tempm.txt", "r");

    if (rcs == NULL)
    {
        printf("File couln't be opened");
        return 1;
    }
    appendInArr(records, rcs);

}

void appendInArr(Record *rec, FILE *f)
{
    char line[MAX_LINELEN];
    fgets(line, sizeof(line), f);

    int len=strlen(line);

    //trim newlines and blank spaces
    while ((line[len-1] == '\n'|| line[len-1] == ' ' || line[len-1] == '\t')) {
        line[--len] = '\0';
    }
    
    if (line[len - 1] == '}')
    {
        line[--len] = '\0'; // Remove closing brace
    }
    char *content = line + 1; // Skip opening brace  


    //printf("%d",strlen(line));
    printf("%s \n", content);
    
}