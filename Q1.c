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


void appendInArr(Record *rec, FILE *f); // appends recs and values in an array

void MergeSort(Record *rec, int left, int right);
void Merge(Record *rec, int left, int mid, int right);
void QuickSort(Record *rec, int left, int right);


int main()
{   
    FILE *temps;
    temps = fopen("tempm.txt", "r");

    if (temps == NULL)
    {
        printf("File couln't be opened");
        return 1;
    }
    Record tRecs[MAX_ENTRIES];            //temperature records
    appendInArr(tRecs, temps);

    FILE *hum;
    hum = fopen("hum.txt", "r");

    if (hum == NULL)
    {
        printf("File couln't be opened");
        return 1;
    }
    Record hRecs[MAX_ENTRIES];            // humidity records
    appendInArr(hRecs, hum);
}

void appendInArr(Record *rec, FILE *f)
{
    char line[MAX_LINELEN];
    int recIndex = 0; // record index


    while (fgets(line, sizeof(line), f) != NULL && recIndex < MAX_ENTRIES)
    {
        char *current_pos = line; // pos in the line

        // find opening brace
        while (*current_pos && *current_pos != '{')
            current_pos++;
        current_pos++; //skip brace

        while (*current_pos && *current_pos != '}')
        {
            if (*current_pos == '\0' || *current_pos == '}')
                break;

            // timestamp
            char *openingQ = strchr(current_pos, '"');
            if (!openingQ)
                break;

            char *closingQ = strchr(openingQ + 1, '"');
            if (!closingQ)
                break;

            int sizeOf = closingQ - openingQ - 1;

            strncpy(rec[recIndex].timestamp, openingQ + 1, sizeOf);

            rec[recIndex].timestamp[sizeOf] = '\0'; // null terminate the timestamp

            // value
            char *colon = strchr(closingQ, ':');
            if (!colon)
                break;

            // Find value
            openingQ = strchr(colon, '"');
            if (!openingQ)
                break;

            closingQ = strchr(openingQ + 1, '"');
            if (!closingQ)
                break;

            sizeOf = closingQ - openingQ - 1;

            char temp[5]; // holds the value
            strncpy(temp, openingQ + 1, sizeOf);
            temp[sizeOf] = '\0';

            rec[recIndex].value = atof(temp);

            // printf("Record %d: %s = %f\n", recIndex, rec[recIndex].timestamp, rec[recIndex].value);

            // Move to next pair
            current_pos = closingQ + 1;
            recIndex++;

            if (recIndex >= MAX_ENTRIES)
                break;
        }
    }

    fclose(f);
}
