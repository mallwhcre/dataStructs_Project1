#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define MAX_ENTRIES 50000
#define MAX_LINELEN 5000

typedef struct Record
{
    char timestamp[24];
    float value;
} Record;

typedef enum
{
    bis,
    bisstar
} search_alg_t;

void appendInArr(Record *rec, FILE *f, int *recIndex); // appends recs and values in an array
void showRec(Record *rec, int max_array);

void quickSort(Record *rec, int left, int right);
void swap(Record *rec_a, Record *rec_b);

void binaryInterpolationSearch(Record *rec, int left, int right, int x);
void BISStar(Record *rec, int left, int right, int key);
int binarySearch(Record *rec, int left, int right, int key);

void search_temp(Record *temp_rec, int length, int temp_val, search_alg_t search_alg);
void search_hum(Record *hum_rec, int length, float hum_val, search_alg_t search_alg);

int main()
{
    char input[100];

    while (1)
    {
        printf("Enter command or 'quit' or 'h' for help: ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0; // Remove newline

        if (strcmp(input, "quit") == 0)
        {
            break;
        }

        char type[20] = "";
        float value = -1;
        char algorithm[20] = "";
        int has_value = 0;
        int has_algorithm = 0;

        char *token = strtok(input, " ");
        if (token == NULL)
            continue;

        if (strcmp(token, "temp") == 0 || strcmp(token, "hum") == 0)
        {
            strcpy(type, token);
        }
        else
        {
            printf("Example usage: temp -v 20 -a bis*\n");
            printf("               hum -v 60 -a bis\n");
            continue;
        }

        while ((token = strtok(NULL, " ")) != NULL)
        {
            if (strcmp(token, "-v") == 0)
            {
                token = strtok(NULL, " ");
                if (token)
                {
                    value = atoi(token);
                    has_value = 1;
                }
            }
            else if (strcmp(token, "-a") == 0)
            {
                token = strtok(NULL, " ");
                if (token)
                {
                    strcpy(algorithm, token);
                    has_algorithm = 1;
                }
            }
        }

        if (has_value && has_algorithm)
        {
            if (strcmp(type, "hum") == 0)
            {
                FILE *hum_file = fopen("hum.txt", "r");
                if (!hum_file)
                {
                    printf("Error opening file");
                }

                Record hum_recs[MAX_ENTRIES];
                int hum_rec_index = 0;
                appendInArr(hum_recs, hum_file, &hum_rec_index);

                fclose(hum_file);

                quickSort(hum_recs, 0, hum_rec_index - 1);

                search_hum(hum_recs, hum_rec_index, value, (strcmp(algorithm, "bis") == 0) ? bis : bisstar);
            }

            else if (strcmp(type, "temp") == 0)
            {
                FILE *temp_file = fopen("tempm.txt", "r");
                if (!temp_file)
                {
                    printf("Error opening file");
                }

                Record t_recs[MAX_ENTRIES];
                int t_rec_index = 0;
                appendInArr(t_recs, temp_file, &t_rec_index);

                fclose(temp_file);

                quickSort(t_recs, 0, t_rec_index - 1);

                search_temp(t_recs, t_rec_index, value, (strcmp(algorithm, "bis") == 0) ? bis : bisstar);
            }
        }
    }

    printf("Exiting application.\n");

    return 0;
}

void search_temp(Record *temp_rec, int length, int temp_val, search_alg_t search_alg)
{
    clock_t t;
    switch (search_alg)
    {
    case bis:
        t = clock();
        binaryInterpolationSearch(temp_rec, 0, length - 1, temp_val);
        t = clock() - t;
        break;

    case bisstar:
        t = clock();
        BISStar(temp_rec, 0, length - 1, temp_val);
        t = clock() - t;
        break;
    }

    double time_taken = ((double)t) / CLOCKS_PER_SEC;
    printf("Searching took %f seconds\n", time_taken);
}

void search_hum(Record *hum_rec, int length, float hum_val, search_alg_t search_alg)
{
    clock_t t;
    switch (search_alg)
    {
    case bis:
        t = clock();
        binaryInterpolationSearch(hum_rec, 0, length - 1, hum_val);
        t = clock() - t;
        break;

    case bisstar:
        t = clock();
        BISStar(hum_rec, 0, length - 1, hum_val);
        t = clock() - t;
        break;
    }
    double time_taken = ((double)t) / CLOCKS_PER_SEC;
    printf("Searching took %f seconds\n", time_taken);
}

void BISStar(Record *rec, int left, int right, int key)
{
    int size = right - left + 1;

    int next_val = 0;

    if (rec[right].value == rec[left].value)
    {
        next_val = left;
    }

    else
    {
        next_val = left + (size * (key - rec[left].value)) / (rec[right].value - rec[left].value);
    }

    // check bounds
    if (next_val < left)
    {
        next_val = left;
    }
    if (next_val > right)
    {
        next_val = right;
    }

    // if next_val is already greater than the key
    if (rec[next_val].value >= key)
    {

        int result = binarySearch(rec, left, next_val, key);

        if (result != -1)
        {

            while (result > left && rec[result - 1].value == key)
            {
                result--;
            }

            while (result <= right && rec[result].value == key)
            {
                printf("Record found: %s = %f\n", rec[result].timestamp, rec[result].value);
                result++;
            }
        }

        else
        {
            printf("Not found\n");
        }
    }

    else
    {
        int i = 1;
        int step = next_val;
        int step_size = (int)sqrt(size);

        // we do steps of i * sqrt(size), where i = 2^x

        while ((step + i * step_size <= right) && rec[step + i * step_size].value < key)
        {
            i *= 2;
        }

        // if we move past the key we set the left and right boundaries

        left = step + (i / 2) * step_size;
        right = (step + i * step_size <= right) ? step + i * step_size : right;

        // printf("Binary search in [%d, %d]\n", left, right);

        // we do binary search

        int result = binarySearch(rec, left, right, key);

        if (result != -1)
        {

            while (result > left && rec[result - 1].value == key)
            {
                result--;
            }

            while (result <= right && rec[result].value == key)
            {
                printf("Record found: %s = %f\n", rec[result].timestamp, rec[result].value);
                result++;
            }
        }

        else
        {
            printf("Not found\n");
        }
    }
}

int binarySearch(Record *rec, int left, int right, int key)
{
    int size = right - left + 1;
    while (left <= right)
    {
        int mid = left + (right - left) / 2;

        if (rec[mid].value == key)
        {
            return mid;
        }

        if (rec[mid].value < key)
        {
            left = mid + 1;
        }

        else
        {
            right = mid - 1;
        }
    }

    return -1;
}

void binaryInterpolationSearch(Record *rec, int left, int right, int key)
{
    int size = right - left + 1;

    int next_val = 0;

    if (rec[right].value == rec[left].value)
    {
        next_val = left;
    }

    else
    {
        next_val = left + (size * (key - rec[left].value)) / (rec[right].value - rec[left].value);
    }

    // check if it gets out of bounds
    if (next_val < left)
    {
        next_val = left;
    }
    if (next_val > right)
    {
        next_val = right;
    }

    while (left <= right && rec[next_val].value != key)
    {
        size = right - left + 1;

        if (size <= 3) // do linear search for small array
        {
            for (int i = 0; i < right; i++)
            {
                if (rec[i].value == key)
                {
                    printf("found");
                }
            }

            return;
        }

        int step = (int)sqrt(size);
        int i = 1;

        if (key > rec[next_val].value)
        {
            // step forward while values < key
            while (next_val + i * step <= right && rec[next_val + i * step].value < key) // calc how many steps of sqrt we need to jump
            {
                i++;
            }

            // udate the area where the key should be
            left = next_val + (i - 1) * step;

            if (right >= (next_val + i * step))
            {
                right = next_val + i * step;
            }
        }
        else
        {
            // step backwards while values > key
            while (next_val - i * step >= left && rec[next_val - i * step].value > key) // calc how many steps of sqrt we need to jump
            {
                i++;
            }

            // udate the area where the key should be
            right = next_val - (i - 1) * step;

            if (left <= (next_val - i * step))
            {
                left = next_val - i * step;
            }
        }

        if (left > right || rec[right].value == rec[left].value)
        {
            break;
        }

        next_val = left + ((right - left + 1) * (key - rec[left].value)) / (rec[right].value - rec[left].value);

        // check again if it is getting out of bounds
        if (next_val < left)
        {
            next_val = left;
        }

        if (next_val > right)
        {
            next_val = right;
        }
    }

    if (rec[next_val].value == key)
    {
        while (next_val > left && rec[next_val - 1].value == key)
        {
            next_val--;
        }

        while (next_val <= right && rec[next_val].value == key)
        {
            printf("Record found: %s = %f\n", rec[next_val].timestamp, rec[next_val].value);
            next_val++;
        }
    }

    else
    {
        printf("Not found\n");
    }
}

void quickSort(Record *rec, int left, int right)
{

    if (left < right)
    {

        int pivot = rec[left].value;
        int i = left - 1;
        int k = right + 1;

        while (1)
        {
            do
            {
                i++;
            } while (rec[i].value < pivot);

            do
            {
                k--;
            } while (rec[k].value > pivot);

            if (i >= k)
            {
                break;
            }

            swap(&rec[k], &rec[i]);
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

void showRec(Record *rec, int max_array)
{

    for (int i = 0; i < max_array; i++)
    {
        printf("Record %d: %s = %f\n", i, rec[i].timestamp, rec[i].value);
    }
}