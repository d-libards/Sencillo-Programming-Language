#include <stdio.h>

int main()
{
    FILE *fp;
    char c;
    int n = 0;

    fp = fopen("sample.s", "r");
    if (fp == NULL)
    {
        perror("Error in opening file");
        return (-1);
    }
    do
    {
        c = fgetc(fp);
        if (feof(fp))
        {
            break;
        }
        printf("%c", c);
    } while (ch != EOF);

    fclose(fp);
    return (0);
}