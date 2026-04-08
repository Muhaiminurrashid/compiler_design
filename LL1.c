#include <stdio.h>
#include <string.h>

#define MAXSTACK 200
#define MAXTOK 200
#define MAXSYM 50

/* Grammar:
S  → id = E ;
E  → T E'
E' → , T E' | epsilon
T  → F T'
T' → < F > T' | epsilon
F  → id | < E >
*/

char *NT[] = {"S", "E", "E'", "T", "T'", "F"};
#define NNT 6

char *TERMINALS[] = {"id", "=", ";", ",", "<", ">", "$"};
#define NTER 7

char *RHS[] =
{
    "id = E ;",    // 1: S -> id = E ;
    "T E'",        // 2: E -> T E'
    ", T E'",      // 3: E' -> , T E'
    "",            // 4: E' -> epsilon
    "F T'",        // 5: T -> F T'
    "< F > T'",    // 6: T' -> < F > T'
    "",            // 7: T' -> epsilon
    "id",          // 8: F -> id
    "< E >"        // 9: F -> < E >
};

// LL(1) Table Mapping
int TABLE[NNT][NTER] =
{
//   id  =   ;   ,   <   >   $
    {1,  0,  0,  0,  0,  0,  0}, // S
    {2,  0,  0,  0,  2,  0,  0}, // E
    {0,  0,  4,  3,  0,  4,  0}, // E'
    {5,  0,  0,  0,  5,  0,  0}, // T
    {0,  0,  7,  7,  6,  7,  0}, // T'
    {8,  0,  0,  0,  9,  0,  0}  // F
};

// ----------------------
// Stack Implementation
// ----------------------
char stack[MAXSTACK][MAXSYM];
int top = -1;

void push(char *s) { 
    if (top < MAXSTACK - 1) strcpy(stack[++top], s); 
}

char* pop() { 
    return (top >= 0) ? stack[top--] : NULL; 
}

void print_stack() {
    printf("Stack: [");
    for(int i = top; i >= 0; i--) {
        printf("%s", stack[i]);
        if(i > 0) printf(", ");
    }
    printf("]\n");
}

// ----------------------
// Helpers
// ----------------------
int find_nt(char *x) { 
    for(int i = 0; i < NNT; i++) if(strcmp(NT[i], x) == 0) return i; 
    return -1; 
}

int find_t(char *x) { 
    for(int i = 0; i < NTER; i++) if(strcmp(TERMINALS[i], x) == 0) return i; 
    return -1; 
}

int tokenize(char *line, char tokens[][MAXSYM]) {
    int n = 0;
    char *p = strtok(line, " \t\n");
    while(p) { 
        strcpy(tokens[n++], p); 
        p = strtok(NULL, " \t\n"); 
    }
    if(n == 0 || strcmp(tokens[n-1], "$") != 0) strcpy(tokens[n++], "$");
    return n;
}

// ----------------------
// Main Parser
// ----------------------
int main() {
    char line[500];
    printf("Enter input string (e.g., 'id = < id > , id ;'):\n");
    fgets(line, sizeof(line), stdin);

    char input[MAXTOK][MAXSYM];
    int n = tokenize(line, input);

    printf("\nTokens detected: ");
    for(int i = 0; i < n; i++) printf("%s ", input[i]);
    printf("\n\n");

    push("$");
    push(NT[0]); // Start symbol 'S'
    int ip = 0;

    printf("%-20s %-12s %-12s %-20s\n", "Stack State", "Lookahead", "Top", "Production Applied");
    printf("---------------------------------------------------------------------------\n");

    while(top >= 0) {
        char X[MAXSYM];
        strcpy(X, pop());
        char *a = input[ip];

        printf("%-20s %-12s %-12s ", " ", a, X);

        int tindex = find_t(X);

        if(tindex != -1) { 
            if(strcmp(X, a) == 0) {
                printf("%-20s\n", "match");
                ip++;
            } else {
                printf("REJECTED (Mismatch: expected %s but found %s)\n", X, a); 
                return 0; 
            }
            print_stack();
            continue;
        }

        int ntindex = find_nt(X);
        int aindex = find_t(a);

        if(ntindex == -1 || aindex == -1) { 
            printf("REJECTED (Invalid symbol)\n"); 
            return 0; 
        }

        int prod = TABLE[ntindex][aindex];

        if(prod == 0) { 
            printf("REJECTED (No Table Entry for [%s, %s])\n", X, a); 
            return 0; 
        }

        if(strlen(RHS[prod-1]) == 0) {
            printf("%-20s\n", "epsilon");
        } else {
            printf("%-20s\n", RHS[prod-1]);
            
            char temp[200]; 
            strcpy(temp, RHS[prod-1]);
            char *p = strtok(temp, " ");
            char symbols[10][MAXSYM];
            int k = 0;
            
            while(p) { 
                strcpy(symbols[k++], p);
                p = strtok(NULL, " "); 
            }
            for(int i = k - 1; i >= 0; i--) {
                push(symbols[i]);
            }
        }
        print_stack();
    }

    if(ip == n) 
        printf("\nRESULT: ACCEPTED\n");
    else 
        printf("\nRESULT: REJECTED (Input remaining)\n");

    return 0;
}
