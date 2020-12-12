#include "sortings.h"

static void swap(char** a, char** b)
{
    char* tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

void bubble(strings_array_t strings, array_size_t stringAmount, comparator_func_t cmp)
{
    for (int i = 0 ; i < (int)stringAmount; i++)
        for (int j = 0 ; j < (int)stringAmount - 1 ; j++)
            if (cmp(strings[j], strings[j+1]) > 0)
                swap(&strings[j], &strings[j+1]);
}

void insertion(strings_array_t strings, array_size_t stringAmount, comparator_func_t cmp)
{
    for (size_t i = 0; i < stringAmount; i++)
    {
        for (size_t j = i; j > 0; j--)
        {
            if (cmp(strings[j - 1], strings[j]) <= 0)
                swap(&strings[j-1], &strings[j]);
        }
    }
}

void merge(strings_array_t strings, array_size_t stringAmount, comparator_func_t cmp)
{
    array_size_t step = 1;
    strings_array_t temp = malloc(sizeof(char*) * stringAmount);
    if (!temp)
    {
        printf("[ERROR] Unable to allocate sufficient memory.\n");
        return;
    }
    while (step < stringAmount)
    {
        array_size_t index = 0;
        array_size_t left = 0;
        array_size_t mid = left + step;
        array_size_t right = left + step * 2;
        do
        {
            mid = mid < stringAmount ? mid : stringAmount;
            right = right < stringAmount ? right : stringAmount;
            array_size_t i1 = left, i2 = mid;
            while (i1 < mid && i2 < right)
            {
                if (cmp(strings[i1], strings[i2]) < 0)
                    temp[index++] = strings[i1++];
                else
                    temp[index++] = strings[i2++];
            }
            while (i1 < mid)
                temp[index++] = strings[i1++];

            while (i2 < right)
                temp[index++] = strings[i2++];

            left += step * 2;
            mid += step * 2;
            right += step * 2;
        } while (left < stringAmount);
        for (array_size_t i = 0; i < stringAmount; i++)
            strings[i] = temp[i];
        step *= 2;
    }
    free(temp);
}

static int partition(strings_array_t strings, size_t left, size_t right, comparator_func_t cmp)
{
    char* middle_string = strings[(left + right) / 2];
    while (left <= right)
    {
        while(cmp(strings[left], middle_string) > 0)
            left++;
        while(cmp(middle_string, strings[right]) > 0)
            right--;
        if (left >= right)
            break;
        swap(&strings[left], &strings[right]);
        left++;
        right--;
    }
    return right;
}

static void quickCore(strings_array_t strings, size_t left, size_t right, comparator_func_t cmp)
{
    if (left < right)
    {
        size_t mid = partition(strings, left, right, cmp);
        quickCore(strings, left, mid, cmp);
        quickCore(strings, mid + 1, right, cmp);
    }
}

void quick(strings_array_t strings, array_size_t stringAmount, comparator_func_t cmp)
{
    quickCore(strings, 0, stringAmount - 1, cmp);
}

void radixCore(char **strings, array_size_t stringAmount, int max_radix, comparator_func_t cmp)
{
    for (int i = max_radix - 1; i >= 0; i--)
    {
        char *temp_array[stringAmount];
        int count[128] = { 0 };

        for (int j = 0; j < (int) stringAmount; j++)
            count[(int) strings[j][i]]++;

        for (int j = 1; j < 128; j++)
            count[j] += count[j - 1];

        for (int j = (int)stringAmount - 1; j >= 0; j--)
        {
            temp_array[count[(int) strings[j][i]] - 1] = strings[j];
            count[(int) strings[j][i]]--;
        }
        for (int j = (int)stringAmount - 1; j >= 0; j--)
        {
            if (i > 0 || cmp("A", "B") < 0)
                strings[j] = temp_array[j];
            else
                strings[stringAmount - 1 - j] = temp_array[j];
        }
    }
}

void radix(strings_array_t strings, array_size_t stringAmount, comparator_func_t cmp)
{
    int max_radix = 0;
    for (int i = 0 ; i < (int)stringAmount; i++)
    {
        if (max_radix < (int)strlen(strings[i]))
            max_radix = (int)strlen(strings[i]);
    }

    radixCore(strings, stringAmount, max_radix, cmp);
}