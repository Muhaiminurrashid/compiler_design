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
    for(int i=0; i<num_keywords; i++)
    {
        if (strcmp(keywords[i],str) == 1) return 1;
    }
    return 0;
}
// operator check
int isoperator(char C)
{
    if (C == '+')return 1;
    if (C == '-')return 1;
    if (C == '*')return 1;
    if (C == '&')return 1;
    if (C == '^')return 1;
    if (C == '|')return 1;

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
        if (isspace(c))continue;
        //comment detection
        if (c =='/')
        {
            k = 0;
            token[k++] = c;
            c = fgetc(fp);
            if (c == '/')
            {
                token[k++] = c;
                c = fgetc(fp);
                while(c != '\n' && k < MAX-1)
                {
                    token[k++]= c;
                    c = fgetc(fp);
                }
                token[k] = '\0';

                printf("Token: %-20s \tType: COMMENT\n", token);
            }
            else if (c == '*')
            {
                //multi line
                token[k++] = c;
                c = fgetc(fp);
                while(c != EOF && k< MAX-3)
                {
                    token[k++] = c;
                    c = fgetc(fp);
                    if (c == '*' && k < MAX -2)
                    {
                        token[k++] = c;
                        c = fgetc(fp);
                        if (c == '/' && k < MAX-1)
                        {
                            token[k++] = c;
                            c = fgetc(fp);
                            break;
                        }
                    }
                }
                token[k] = '\0';

                printf("Token: %-90s \tType: MULTI COMMENT\n", token);
            }
            else
            {
                //operator
                token[k++] = '\0';
                printf("Token: %-20s \tType: OPERATOR\n", token);
                ungetc(c,fp);
            }
        }
        // Identifiers or keywords
        else if (isalpha(c) || c == '_')
        {
            k = 0;
            token[k++] = c;
            c = fgetc(fp);
            while((isalnum(c) || c == '_') && k< MAX-1)
            {
                token[k++] = c;
                c = fgetc(fp);
            }
            token[k] = '\0';
            if (c != EOF)ungetc(c,fp);

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
            c = fgetc(fp);
            while(isdigit(c) && k < MAX-1)
            {
                token[k++] = c;
                c = fgetc(fp);
            }
            token[k] = '\0';
            if (c != EOF)ungetc(c,fp);
            printf("Token: %-20s \tType: NUMBER\n", token);

        }

        // String literals
        else if (c == '"')
        {
            k = 0;
            token[k++] = c;
            c = fgetc(fp);
            while(c != '"' && k < MAX-1 && c!=EOF)
            {
                token[k++] = c;
                c = fgetc(fp);
            }
            token[k++] = '"';
            token[k] = '\0';
            printf("Token: %-20s \tType: STRING_LITERAL\n", token);

        }
       else if (isoperator(c))
            // operators
        {
            token[0] = c;
            token[1] = '\0';

            printf("Token: %-20s \tType: OPERATOR \n", token);
        }
        // punctuation
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
    FILE *fp = fopen("hello.c","r");
    if (!fp)
    {
        printf("Can't find file\n");
    }
    lexicalAnalyzer(fp);
    fclose(fp);

    return 0;
}
