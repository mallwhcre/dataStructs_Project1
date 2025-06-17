#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "math.h"

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

void binaryInterpolationSearch(Record *rec, int left, int right, int x);

int main()
{
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

    MergeSort(tRecs, 0, rec_index_temp - 1); // sort temperature records
    // quickSort(tRecs, 0, rec_index_temp - 1);

    printf("Sorted temperature records:\n");
    showRec(tRecs, rec_index_temp);

    Record rects_test[] = {{"hello1", 1}, {"hello2", 3}, {"hello53", 5}, {"hello53", 10}, {"hello53", 15}};

    binaryInterpolationSearch(tRecs, 0, rec_index_temp - 1, 11);

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

int next(Record *rec, int x, int left, int right)
{
    int size = right - left + 1;
    int next_val = (size * (x - rec[left].value) / (rec[right].value - rec[left].value)) + 1;

    return next_val;
}

int min(int a, int b)
{
    return (a < b) ? a : b;
}

int max(int a, int b)
{
    return (a > b) ? a : b;
}

void binaryInterpolationSearch(Record *rec, int left, int right, int key)
{
    int size = right - left + 1;
    int next_val =  (size * (key - rec[left].value)/ (rec[right].value - rec[left].value));

    printf("next va is %d\n", next_val);

    while (key != rec[next_val].value)
    {
        int i = 0;
        if (size <= 3)
        {
            int j=0;
            for (j; j<size; j++)
            {
                if (key == rec[j].value)
                {
                    printf("Record Found: %s = %f\n", rec[left + j].timestamp, rec[left + j].value);
                    return;
                }
            }
        }

        if (key >= rec[next_val].value)
        {
            while (key > rec[next_val + i * (int)sqrt(size) - 1].value)
            {
                i = i + 1;
            }

            right = next_val + i * (int)sqrt(size);
            left = next_val + (i - 1) * (int)sqrt(size);

        printf("left is %d\n", right);
        printf("right is %d\n", right);

        }

        else if (key < rec[next_val].value)
        {
            while (key < rec[next_val - i * (int)sqrt(size)].value)
            {
                i = i + 1;
            }
            right = next_val - (i - 1) * (int)sqrt(size);
            left = next_val - i * (int)sqrt(size);

            printf("left is %d\n", right);
            printf("right is %d\n", right);
        }

        next_val = left + ((right - left + 1) * (key - rec[left].value) / (rec[right].value - rec[left].value));

        printf("next_val is %d\n", next_val);
    }

    if (key == rec[next_val].value)
    {
        while (key == rec[next_val].value)
        {
            next_val--;

        }

        next_val++;

        while (key == rec[next_val].value)
        {
            printf("SUCit Record: %s = %f\n", rec[next_val].timestamp, rec[next_val].value);
            next_val++;

        }

        // printf("SUCit Record: %s = %f\n", rec[next_val].timestamp, rec[next_val].value);
    }

    else
    {
        printf("drunk it");
    }
}

void quickSort(Record *rec, int left, int right)
{

    if (left < right)
    {

        // int pivot = rec[left].value;

        // int i = left - 1;
        // for (int j = left; j <= right - 1; j++)
        // {
        //     if (rec[j].value < pivot)
        //     {
        //         i++;
        //         swap(&rec[i], &rec[j]);
        //     }
        // }

        // swap(&rec[i + 1], &rec[right]);

        // quickSort(rec, left, i);
        // quickSort(rec, i + 2, right);

        int pivot = rec[left].value;
        int i = left - 1;
        int k = right + 1;

        while (i <= k)
        {
            do
            {
                i++;
            } while (rec[i].value < pivot);

            do
            {
                k--;
            } while (rec[k].value > pivot);

            if (k > i)
            {
                swap(&rec[k], &rec[i]);
            }
        }

        quickSort(rec, left, k);
        quickSort(rec, k + 1, right);
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