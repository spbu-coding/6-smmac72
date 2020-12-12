#include "main.h"
enum ERROR LastError = STATUS_OK;

int checkError()
{
    switch (LastError)
    {
        case STATUS_OK:
            return 0;

        case ERR_OPEN_FILE:
            printf("[ERROR] Can't open input file!");
            return -1;

        case ERR_ALLOC:
            printf("[ERROR] Can't allocate sufficient memory.");
            return -1;

        case ERR_PARAM_AMOUNT:
            printf("[ERROR] Only five parameters are accepted.");
            return -1;

        case ERR_PRINTING:
            printf("[ERROR] Can't print to a file.");
            return -1;

        case ERR_READING:
            printf("[ERROR] Can't read from a file.");
            return -1;

        case ERR_STRING_AMOUNT:
            printf("[ERROR] File string amount is less than a parametered one.");
            return -1;

        case ERR_UNSUPPORTED_SORTING:
            printf("[ERROR] Unsupported sorting method!");
            return -1;

        case ERR_UNSUPPORTED_COMPARATOR:
            printf("[ERROR] Unsupported comparator settings! Try 'asc' or 'des'");
            return -1;

        case ERR_SECOND_PARAM:
            printf("[ERROR] String amount should be a positive number.");
            return -1;

        default:
            printf("[ERROR] Unspecific error has broken everything. sorry :(");
            return -1;
    }
}
int compareASC(const char* string1, const char* string2)
{
    return strcmp(string2, string1);
}

int compareDES(const char* string1, const char* string2)
{
    return strcmp(string1, string2);
}

int parseParams(int argc, char** argv, array_size_t* stringAmount, comparator_func_t* cmp)
{
    if (argc != 6)
    {
        LastError = ERR_PARAM_AMOUNT;
        return checkError();
    }

    char* endPointer;
    *stringAmount = strtoul(argv[1], &endPointer, 10);
    if (*endPointer != '\0')
    {
        LastError = ERR_SECOND_PARAM;
        return checkError();
    }

    if (strcmp(asc, argv[5]) == 0)
        *cmp = compareASC;
    else if(strcmp(des, argv[5]) == 0)
        *cmp = compareDES;
    else
    {
        LastError = ERR_UNSUPPORTED_COMPARATOR;
        return checkError();
    }
    return 0;
}

int readFile(FILE* file, array_size_t stringAmount, strings_array_t strings)
{
    size_t i;
    for (i = 0; i < stringAmount && !feof(file); i++)
    {
        if (!fgets(strings[i], MAX_INPUT_STRING_SIZE, file))
        {
            LastError = ERR_READING;
            return checkError();
        }
    }
    if (i < stringAmount)
    {
        LastError = ERR_STRING_AMOUNT;
        return checkError();
    }
    return 0;
}

int writeFile(FILE* file, array_size_t stringAmount, strings_array_t strings)
{
    for (size_t i = 0; i < stringAmount; i++)
    {
        if (fputs(strings[i], file) == EOF)
        {
            LastError = ERR_PRINTING;
            return checkError();
        }
    }
    if (!stringAmount)
        fputs("\n", file);
    return 0;
}

int stringSort(char** argv, array_size_t stringAmount, strings_array_t strings, comparator_func_t cmp)
{
    if (strcmp(word_bubble, argv[4]) == 0)
        bubble(strings, stringAmount, cmp);
    else if (strcmp(word_merge, argv[4]) == 0)
        merge(strings, stringAmount, cmp);
    else if (strcmp(word_quick, argv[4]) == 0)
        quick(strings, stringAmount, cmp);
    else if (strcmp(word_insertion, argv[4]) == 0)
        insertion(strings, stringAmount, cmp);
    else if (strcmp(word_radix, argv[4]) == 0)
        radix(strings, stringAmount, cmp);
    else
    {
        LastError = ERR_UNSUPPORTED_SORTING;
        return checkError();
    }
    return 0;
}

int main(int argc, char** argv)
{
    array_size_t stringAmount;
    comparator_func_t cmp = NULL;
    FILE* inputFile;
    FILE* outputFile;

    if (parseParams(argc, argv, &stringAmount, &cmp) == -1)
        return -1;

    inputFile = fopen(argv[2], "r");
    if (!inputFile)
    {
        fclose(inputFile);
        LastError = ERR_OPEN_FILE;
        return checkError();
    }

    strings_array_t strings = malloc(sizeof(char*) * stringAmount);
    if (!strings)
    {
        fclose(inputFile);
        LastError = ERR_ALLOC;
        return checkError();
    }
    for (size_t i = 0; i < stringAmount; i++)
    {
        strings[i] = (char*)malloc(sizeof(char) * (MAX_INPUT_STRING_SIZE + 1));
        if (!strings[i])
        {
            for (size_t j = 0; j < i; j++)
                free(strings[j]);
            free(strings);

            fclose(inputFile);
            LastError = ERR_ALLOC;
            return checkError();
        }
    }

    if (readFile(inputFile, stringAmount, strings) == -1)
    {
        for (size_t i = 0; i < stringAmount; i++)
            free(strings[i]);
        free(strings);
        fclose(inputFile);
        return -1;
    }
    fclose(inputFile);
    if (stringSort(argv, stringAmount, strings, cmp) == -1)
        return -1;

    outputFile = fopen(argv[3], "w");
    if (!outputFile)
    {
        fclose(outputFile);
        LastError = ERR_OPEN_FILE;
        return checkError();
    }

    if (writeFile(outputFile, stringAmount, strings) == -1)
    {
        for (size_t i = 0; i < stringAmount; i++)
            free(strings[i]);
        free(strings);
        fclose(outputFile);
        return -1;
    }

    for (size_t i = 0; i < stringAmount; i++)
        free(strings[i]);
    free(strings);
    fclose(outputFile);
    return 0;
}