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
void showRec(Record *rec); 

void MergeSort(Record *rec, int left, int right);
void Merge(Record *rec, int left, int mid, int right);

void QuickSort(Record *rec, int left, int right);

int main()
{
    FILE *temps;
    temps = fopen("test.txt", "r");

    if (temps == NULL)
    {
        printf("File couln't be opened");
        return 1;
    }
    Record tRecs[MAX_ENTRIES]; // temperature records
    appendInArr(tRecs, temps);

    printf("Unsorted temperature records:\n");
    showRec(tRecs);

    MergeSort(tRecs, 0, MAX_ENTRIES - 1); // sort temperature records

    printf("Sorted temperature records:\n");
    showRec(tRecs);

    FILE *hum;
    hum = fopen("hum.txt", "r");

    if (hum == NULL)
    {
        printf("File couln't be opened");
        return 1;
    }
    Record hRecs[MAX_ENTRIES]; // humidity records
    appendInArr(hRecs, hum);
    MergeSort(hRecs, 0, MAX_ENTRIES - 1); // sort humidity records


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
        current_pos++; // skip brace

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

            // next pair
            current_pos = closingQ + 1;
            recIndex++;

            if (recIndex >= MAX_ENTRIES)
                break;
        }
    }

    fclose(f);
}

void MergeSort(Record *rec, int left, int right)
{

    if (left >= right)
        return;

    int mid = (left + right) / 2;

    MergeSort(rec, left, mid);      // first half
    MergeSort(rec, mid + 1, right); // second half

    Merge(rec, left, mid, right);
}

void Merge(Record *rec, int left, int mid, int right)
{

    Record *temp = malloc((right - left + 1) * sizeof(Record));

    if (!temp)
    {
        printf(" allocation failed\n");
        exit(1);
    }

    int i = left;    // left subarray index
    int j = mid + 1; // right subarray index
    int k = 0;       // merged array index

    while (i <= mid && j <= right)
    {
        if (rec[i].value < rec[j].value)
        {
            temp[k++] = rec[i++];
        }
        else
        {
            temp[k++] = rec[j++];
        }
    }

    while (i <= mid) // copy remaining elements from left subarray
    {
        temp[k++] = rec[i++];
    }
    while (j <= right) // copy remaining elements from right subarray
    {
        temp[k++] = rec[j++];
    }

    for (i = left; i <= right; i++)
    {
        rec[i] = temp[i - left];
    }

    free(temp);
}

void showRec(Record *rec)
{
    
    for (int i = 0; i < MAX_ENTRIES; i++)
    {   
        if (!rec[i].value)
            break; 
        printf("Record %d: %s = %f\n", i, rec[i].timestamp, rec[i].value);
    }
}