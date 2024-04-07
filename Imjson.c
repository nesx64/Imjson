#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int MAX_HEX_LENGTH = 7;

void usage()
{
    printf("Usage: imjson [FILE] [NUMBER OF ELEMENTS BY LINE]\n");
    printf("Where FILE is the file containing all the HEX to convert\n");
}

int processLine(char *line, int x, FILE *out, int eof)
{
    char *token;
    char buffer[x][MAX_HEX_LENGTH + 1];
    int j = 0;
    token = strtok(line, ",");
    while (token != NULL && j < x)
    {
        if (token[0] != '#' || strlen(token) != MAX_HEX_LENGTH)
        {
            printf("[ERROR] \tParsing given file error.\n");
            printf("\t\tCheck the number of elements for one line, or check format\n");
            printf("[INFO] \t\tAs the process failed, the output file was deleted.\n");
            remove("out.json");
            exit(EXIT_FAILURE);
        }
        strcpy(buffer[j], token);
        j++;
        token = strtok(NULL, ",");
    }

    fprintf(out, "\t[\n");
    for (int i = 0; i < j - 1; i++)
    {
        fprintf(out, "\t\t\"%s\",\n", buffer[i]);
    }
    fprintf(out, "\t\t\"%s\"\n\t]%s\n", buffer[j - 1], eof ? "" : ",");
    return 0;
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("ERROR: Incorrect number of arguments.\n");
        usage();
        return 1;
    }

    char *file = argv[1];
    int x = atoi(argv[2]);

    printf("[INFO] \t\tReading %s file...\n", file);
    FILE *in = fopen(file, "r");
    if (in == NULL)
    {
        printf("Error: input file '%s' doesn't exist or couldn't be opened.\n", file);
        usage();
        return 1;
    }

    printf("[WARNING] \tDefault output file is out.json.\n");
    FILE *out = fopen("out.json", "w");
    if (out == NULL)
    {
        printf("Error: output file (res.txt) couldn't be created.\n");
        fclose(in);
        fclose(out);
        return 1;
    }
    char *line = malloc(x * 8 * sizeof(char));
    if (line == NULL)
    {
        printf("Error: memory allocation failed for line buffer.\n");
        fclose(in);
        fclose(out);
        return 1;
    }

    fprintf(out, "[\n");

    char prevLine[x * 8];
    int isFirstLine = 1;

    while (fgets(line, x * 8, in))
    {
        if (line[0] != '\n')
        {
            printf("[INFO] \t\tWill process :  %s\n", line);
            if (!isFirstLine)
            {
                processLine(prevLine, x, out, 0);
            }
            strcpy(prevLine, line);
            isFirstLine = 0;
        }
    }
    processLine(prevLine, x, out, 1);
    fprintf(out, "]");

    free(line);
    printf("\n[SUCCESS] \tClosing files.\n");
    printf("[INFO] \t\tCheck json output before using it.\n");
    fclose(in);
    fclose(out);

    return 0;
}