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
void appendInArr(Record *rec, FILE *f); // seperates the timestamps & values

int main()
{
    FILE *rcs;
    rcs = fopen("test.txt", "r");

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

    int recIndex = 0; // index of record in the array
    int linePos = sizeof(line);
    // while (fgets(line, sizeof(line), f) != NULL && recIndex < MAX_ENTRIES)
    //{
    while(line[linePos]!='}')
    {

        char *content = line + 1; // Skip opening brace

        // pair processing only first line
        strtok(content, ","); // seperates pair

        char *openingQ = strchr(content, '"');
        char *closingQ = strchr(openingQ + 1, '"');

        int sizeOf = closingQ - openingQ - 1;

        strncpy(rec[recIndex].timestamp, openingQ + 1, sizeOf); // timestamp added

        char *colon = strchr(closingQ, ':');

        openingQ = strchr(colon, '"');
        closingQ = strchr(openingQ + 1, '"');

        sizeOf = closingQ - openingQ - 1;

        char temp[5]; // holds the value
        strncpy(temp, openingQ + 1, sizeOf);

        rec[recIndex].value = atof(temp); // value is added

        printf("%s\n", rec[recIndex].timestamp);
        printf("%f\n", rec[recIndex].value);

        linePos=*closingQ+1;

        // }
    } 

    recIndex++; // move to the next line
}
