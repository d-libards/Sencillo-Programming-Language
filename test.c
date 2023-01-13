void fileToString(char *str)
{
    FILE *fp;
    char file[] = "Input.cb";
    char *ext;
    ext = strrchr(file, '.');
    if (isEqual(ext, ".cb"))
    {
        fp = fopen(file, "r");
        if (fp == NULL)
        {
            printf("Error! Can't find file.\n");
            exit(0);
        }
    }
    else
        printf("Error! Incorrect file extension.\n");
    char ch;
    int i = 0;
    int comments = 0, stringLiteral = 0;
    do
    {
        ch = fgetc(fp);
        if (ch == '#' && (isDelimiter(str[i - 1]) || i == 0))
            comments++;
        if (ch == '\n')
        {
            if (comments > 0)
            {
                comments = comments - 1;
                str[i] = '\\';
                i++;
                str[i] = 'n';
                i++;
                str[i] = ' ';
                i++;
                continue;
            }
            else
            {
                str[i] = ' ';
                i++;
                continue;
            }
        }
        if (ch == '\"')
            stringLiteral++;
        if (ch == '\"' && (stringLiteral == 1))
        {
            str[i] = ' ';
            i++;
        }
        if (ch == '\"' && (stringLiteral == 2))
        {
            stringLiteral = 0;
        }
        str[i] = ch;
        i++;
    } while (ch != EOF);
    str[i - 1] = '\0';
    fclose(fp);
}