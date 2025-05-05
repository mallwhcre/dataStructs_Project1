// C program for the implementation of merge sort
#include <stdio.h>
#include <stdlib.h>

// Merges two subarrays of arr[].
// First subarray is arr[left..mid]
// Second subarray is arr[mid+1..right]

typedef struct Record
{
    char timestamp[24];
    float value;
} Record;

void Merge(Record *rec, int left, int mid, int right);

void MergeSort(Record *rec, int left, int right)
{
    // printf("before ret");
    if (left >= right)
    {
        // printf("left >= right");
        return;
    }

    // printf("after ret");

    // int mid = (left + right) / 2;
    int mid = left + (right - left) / 2;
    // int mid = left + (right - left) / 2;

    MergeSort(rec, left, mid);      // first half
    MergeSort(rec, mid + 1, right); // second half

    Merge(rec, left, mid, right);
}

void Merge(Record *rec, int left, int mid, int right)
{
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Create temporary arrays
    // int leftArr[n1], rightArr[n2];

    Record *leftArr = malloc(n1 * sizeof(Record));
    Record *rightArr = malloc(n2 * sizeof(Record));

    // Copy data to temporary arrays
    for (i = 0; i < n1; i++)
        leftArr[i] = rec[left + i];
    for (j = 0; j < n2; j++)
        rightArr[j] = rec[mid + 1 + j];

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
        if (!rec[i].value)
            break; 
        printf("Record %d: %s = %f\n", i, rec[i].timestamp, rec[i].value);
    }
}

int main()
{
    // int arr[] = {12, 11, 13, 5, 6, 7};
    // int n = sizeof(arr) / sizeof(arr[0]);

    Record rec[7];
    rec[0].value = 12;
    rec[1].value = 11;
    rec[2].value = 12;
    rec[3].value = 5;
    rec[4].value = 6;
    rec[5].value = 7;
    rec[6].value = 12;

    showRec(rec, 7);

    // Sorting arr using mergesort
    MergeSort(rec, 0, 7);

    showRec(rec, 7);


    // for (int i = 0; i < n; i++)
    //     printf("%d ", arr[i]);
    return 0;
}