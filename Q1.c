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

void appendInArr(Record *rec, FILE *f, int *recIndex); // appends recs and values in an array
void showRec(Record *rec, int max_array);

void MergeSort(Record *rec, int left, int right);
void Merge(Record *rec, int left, int mid, int right);

void quickSort(Record *rec, int left, int right);
void swap(Record *rec_a, Record *rec_b);

int main()
{

    // Record rec_test[2] = {{"hello1", 5}, {"hello2", 4}};

    // showRec(rec_test, 2);
    // swap(&rec_test[0], &rec_test[1]);

    // showRec(rec_test, 2);

    FILE *temps;
    temps = fopen("test.txt", "r");
    int rec_index_temp = 0;
    int rec_index_hum = 0;

    if (temps == NULL)
    {
        printf("File couln't be opened");
        return 1;
    }

    Record *tRecs = malloc(MAX_ENTRIES * sizeof(Record));
    appendInArr(tRecs, temps, &rec_index_temp);

    printf("Unsorted temperature records:\n");
    showRec(tRecs, rec_index_temp);

    printf("Index is:%d\n", rec_index_temp);

    // MergeSort(tRecs, 0, rec_index_temp - 1); // sort temperature records
    quickSort(tRecs, 0, rec_index_temp - 1);

    printf("Sorted temperature records:\n");
    showRec(tRecs, rec_index_temp);

    printf("Index is:%d\n", rec_index_temp);

    FILE *hum;
    hum = fopen("hum.txt", "r");

    if (hum == NULL)
    {
        printf("File couln't be opened");
        return 1;
    }
    Record hRecs[MAX_ENTRIES]; // humidity records
    appendInArr(hRecs, hum, &rec_index_hum);
    MergeSort(hRecs, 0, rec_index_hum - 1); // sort humidity records

    free(tRecs);
    return 0;
}

void quickSort(Record *rec, int left, int right)
{

    if (left < right)
    {

        int pivot = rec[left].value;

        int i = left - 1;
        for (int j = left; j <= right - 1; j++)
        {
            if (rec[j].value < pivot)
            {
                i++;
                swap(&rec[i], &rec[j]);
            }
        }

        swap(&rec[i + 1], &rec[right]);

        quickSort(rec, left, i);
        quickSort(rec, i + 2, right);
    }
}

void swap(Record *rec_a, Record *rec_b)
{
    Record temp = *rec_a;
    *rec_a = *rec_b;
    *rec_b = temp;
}

void appendInArr(Record *rec, FILE *f, int *recIndex)
{
    char line[MAX_LINELEN];

    while (fgets(line, sizeof(line), f) != NULL && *recIndex < MAX_ENTRIES)
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

            strncpy(rec[*recIndex].timestamp, openingQ + 1, sizeOf);

            rec[*recIndex].timestamp[sizeOf] = '\0'; // null terminate the timestamp

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

            rec[*recIndex].value = atof(temp);

            // next pair
            current_pos = closingQ + 1;
            *recIndex += 1;

            if (*recIndex >= MAX_ENTRIES)
                break;
        }
    }

    fclose(f);
}

void MergeSort(Record *rec, int left, int right)
{
    if (left >= right)
    {
        return;
    }

    int mid = left + (right - left) / 2;

    MergeSort(rec, left, mid);      // first half
    MergeSort(rec, mid + 1, right); // second half

    Merge(rec, left, mid, right);
}

void Merge(Record *rec, int left, int mid, int right)
{
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;

    Record *leftArr = malloc(n1 * sizeof(Record));
    Record *rightArr = malloc(n2 * sizeof(Record));

    // Copy data to temporary arrays
    for (i = 0; i < n1; i++)
    {
        leftArr[i] = rec[left + i];
    }
    for (j = 0; j < n2; j++)
    {
        rightArr[j] = rec[mid + 1 + j];
    }

    // Merge the temporary arrays back into arr[left..right]
    i = 0;
    j = 0;
    k = left;
    while (i < n1 && j < n2)
    {
        if (leftArr[i].value <= rightArr[j].value)
        {
            rec[k] = leftArr[i];
            i++;
        }
        else
        {
            rec[k] = rightArr[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of leftArr[], if any
    while (i < n1)
    {
        rec[k] = leftArr[i];
        i++;
        k++;
    }

    // Copy the remaining elements of rightArr[], if any
    while (j < n2)
    {
        rec[k] = rightArr[j];
        j++;
        k++;
    }

    free(leftArr);
    free(rightArr);
}

void showRec(Record *rec, int max_array)
{

    for (int i = 0; i < max_array; i++)
    {
        printf("Record %d: %s = %f\n", i, rec[i].timestamp, rec[i].value);
    }
}