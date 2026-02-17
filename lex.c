#include <stdio.h>
#include <ctype.h>
#include <string.h>
#define MAX 100

// List of keywords
char *keywords[] = {"int", "return", "include", "printf"};
int num_keywords = 4;

// Check if a string is a keyword
int isKeyword(char str[])
{
    for (int i = 0; i < num_keywords; i++)
    {
        if (strcmp(str, keywords[i]) == 0)
            return 1;
    }
    return 0;
}

// Main Lexical Analyzer
void lexicalAnalyzer(FILE *fp)
{
    char c;
    char token[MAX];
    int k = 0;

    while ((c = fgetc(fp)) != EOF)
    {
        // Ignore whitespace
        if (isspace(c)) continue;

        //comment detection
        else if(c=='/')
        {
            k=0;
            token[k++]=c;
            c=fgetc(fp);

            if(c=='/')
            {
                //single line
                token[k++]=c;
                c=fgetc(fp);
                while(c!='\n' && k<MAX-1)
                {
                    token[k++]=c;
                    c=fgetc(fp);
                }
                token[k]='\0';

                printf("Token: %-20s \tType: COMMENT\n", token);


            }
            else if (c == '*')
            {
                //multi line
                token[k++] = c;
                c=fgetc(fp);
                while(c != EOF && k < MAX-3)
                {
                    token[k++]=c;
                    c=fgetc(fp);
                    if (c == '*' && k < MAX - 2)
                    {
                        token[k++]=c;
                        c=fgetc(fp);
                        if (c == '/' && k < MAX -1)
                        {
                            token[k++]=c;
                            c=fgetc(fp);
                            break;
                        }
                    }
                }
                token[k]='\0';
                printf("Token: %-60s \tType: MULTI COMMENT\n", token);

            }


            else
            {
                //operator
                token[k++]='\0';
                printf("Token: %-20s \tType: OPERATOR\n", token);
                ungetc(c,fp);

            }




        }
        // Identifiers or keywords
        else if (isalpha(c) || c == '_')
        {
            k = 0;
            token[k++] = c;
            while ((isalnum(c = fgetc(fp)) || c == '_') && k < MAX - 1)
            {
                token[k++] = c;
            }
            token[k] = '\0';
            if (c != EOF) ungetc(c, fp);

            if (isKeyword(token))
                printf("Token: %-20s \tType: KEYWORD\n", token);
            else
                printf("Token: %-20s \tType: IDENTIFIER\n", token);
        }

        // Numbers
        else if (isdigit(c))
        {
            k = 0;
            token[k++] = c;
            while (isdigit(c = fgetc(fp)) && k < MAX - 1)
            {
                token[k++] = c;
            }
            token[k] = '\0';
            if (c != EOF) ungetc(c, fp);

            printf("Token: %-20s \tType: NUMBER\n", token);
        }

        // String literals
        else if (c == '"')
        {
            k = 0;
            token[k++] = '"'; // store starting quote
            while ((c = fgetc(fp)) != '"' && c != EOF && k < MAX - 1)
            {
                token[k++] = c;
            }
            token[k++] = '"'; // store ending quote
            token[k] = '\0';

            printf("Token: %-20s \tType: STRING_LITERAL\n", token);
        }

        // Operators and punctuation
        else
        {
            token[0] = c;
            token[1] = '\0';
            printf("Token: %-20s \tType: SPECIAL_SYMBOL\n", token);
        }
    }
}

int main()
{
    FILE *fp = fopen("hello.c", "r");
    if (!fp)
    {
        printf("Cannot open file\n");
        return 1;
    }

    printf("Lexical Analysis Output:\n\n");
    lexicalAnalyzer(fp);

    fclose(fp);
    return 0;
}
