/*

    We consider that all n integers entered by the user can be placed in the
    file (n is also entered), also file's maximum size does not exceed the
    maximum value of the long type, the file contains only numbers of type int

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void prepare_data(const char *);
void invert_data(const char *);
void print_data(const char *);


// Writing data from input stream to file
void prepare_data(const char *name) {
    int n, number;
    FILE *f = fopen(name, "w");

    if (f == NULL) {
        printf("Error opening file");
        exit(1);
    }

    printf("Enter the amount of numbers: ");
    scanf("%d", &n);
    printf("Enter %d number(s): ", n);
    for(int i = 0; i < n; ++i) {
        scanf("%d", &number);
        fwrite(&number, sizeof(int), 1, f);
    }

    fclose(f);
}

// Inverting data in file
void invert_data(const char *name) {
    long pos, n;
    int num1, num2, i = 0;
    FILE* f = fopen(name, "r+");

    if (f == NULL) {
        printf("Error opening file");
        exit(1);
    }

    fseek(f, 0L, SEEK_END);
    pos = ftell(f);
    n = pos / sizeof(int);

    pos = 0;
    while (i < n / 2) {
        fseek(f, pos, SEEK_SET);
        fread(&num1, sizeof(int), 1, f);
        fseek(f, -pos - sizeof(int), SEEK_END);
        fread(&num2, sizeof(int), 1, f);
        fseek(f, pos, SEEK_SET);
        fwrite(&num2, sizeof(int), 1, f);
        fseek(f, -pos - sizeof(int), SEEK_END);
        fwrite(&num1, sizeof(int), 1, f);
        pos += sizeof(int);
        ++i;
    }

    fclose(f);
}


// File output
void print_data(const char *name) {
    int number;
    long pos, n;
    FILE *f = fopen(name, "r");

    if (f == NULL) {
        printf("Error opening file");
        exit(1);
    }

    fseek(f, 0L, SEEK_END);
    pos = ftell(f);
    n = pos / sizeof(int);
    fseek(f, 0L, SEEK_SET);

    printf("Your numbers after inverting: ");
    for(int i = 0; i < n; ++i) {
        fread(&number, sizeof(int), 1, f);
        printf("%d%c", number, ' ');
    }
    printf("\n");

    fclose(f);
}


int main(void) {

    const char *file_name = "data.bin";

    // Preparing data
    prepare_data(file_name);

    // Inverting data in file
    invert_data(file_name);

    // Data output
    print_data(file_name);
    remove(file_name);

    return 0;
}