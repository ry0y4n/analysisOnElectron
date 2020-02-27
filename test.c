#include<stdio.h>

int main() {
    FILE *fp;
    char file_name[] = "test.csv";

    fp = fopen(file_name, "w");

    fprintf(fp, "1,2,3");

    fclose(fp);

    return 0;
}