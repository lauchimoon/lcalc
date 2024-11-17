#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#define MAX_STACK_SIZE 1024
#define MAX_VALUE_SIZE 30
#define MAX_WORD_SIZE  30

enum {
    UOP_SIN = 0,
    UOP_COS,
    UOP_EXP,
    UOP_LN
};

struct Stack {
    double p[MAX_STACK_SIZE];
    size_t size;
};

void init(struct Stack *);
void push(struct Stack *, double);
double pop(struct Stack *);

void parse_number(char [], char);

int isuop(char);
int isbinop(char);
int is_specialword(char *);
char mathfunction(char *);
void binop(struct Stack *, char);
void uop(struct Stack *, char);
void constant(struct Stack *, char *);

int main()
{
    struct Stack s;
    init(&s);
    char c;

    while ((c = getchar()) != EOF) {
        if (isspace(c)) {
            c = getchar();
            while (isspace(c))
                c = getchar();
        } else if (isdigit(c)) {
            char value[MAX_VALUE_SIZE] = {};
            parse_number(value, c);
            push(&s, atof(value));
        } else if (isalpha(c) && !isuop(c)) { /* process words like "sin", "cos" or "exp" */
            char word[MAX_WORD_SIZE] = {};
            int j = 0;
            word[j++] = c;
            c = getchar();
            while (isalpha(c)) {
                word[j++] = c;
                c = getchar();
            }
            word[j] = '\0';
            if (!is_specialword(word)) {
                printf("error: unknown math function '%s'\n", word);
                return 1;
            }
            if (strcmp(word, "pi") == 0 || strcmp(word, "e") == 0)
                constant(&s, word);
            uop(&s, mathfunction(word));
        } else if (isbinop(c)) {
            binop(&s, c);
            c = getchar();
        } else if (isuop(c)) {
            uop(&s, c);
            c = getchar();
        }
    }
    return 0;
}

void init(struct Stack *s)
{
    s->size = 0;
}

void push(struct Stack *s, double elem)
{
    if (s->size + 1 >= MAX_STACK_SIZE) {
        printf("error: stack full\n");
        return;
    }
    s->p[s->size++] = elem;
}

double pop(struct Stack *s)
{
    if (s->size <= 0) {
        printf("error: stack empty\n");
        return 0.0;
    }
    return s->p[--s->size];
}

void parse_number(char value[], char c)
{
    int j = 0;
    value[j++] = c;
    c = getchar();
    while (isdigit(c)) {
        value[j++] = c;
        c = getchar();
    }
    if (c == '.') {
        value[j++] = c;
        c = getchar();
        if (isdigit(c)) {
            while (isdigit(c)) {
                value[j++] = c;
                c = getchar();
            }
        }
    }
}

int isuop(char c)
{
    return c == '.' || c == 'n';
}

int isbinop(char c)
{
    return c == '+' || c == '-' || c == '*' || c == '/' ||
        c == '%' || c == '^';
}

int is_specialword(char *s)
{
    return strcmp(s, "sin") == 0 || 
            strcmp(s, "cos") == 0 || 
            strcmp(s, "exp") == 0 ||
            strcmp(s, "ln") == 0 ||
            strcmp(s, "pi") == 0 ||
            strcmp(s, "e") == 0; 
}

char mathfunction(char *s)
{
    if (strcmp(s, "sin") == 0)
        return UOP_SIN;
    else if (strcmp(s, "cos") == 0)
        return UOP_COS;
    else if (strcmp(s, "exp") == 0)
        return UOP_EXP;
    else if (strcmp(s, "ln") == 0)
        return UOP_LN;
    else
        return -1;
}

void binop(struct Stack *s, char op)
{
    switch (op) {
        case '+':
            push(s, pop(s) + pop(s));
            break;
        case '-': {
            double op2 = pop(s);
            push(s, pop(s) - op2);
            break;
        }
        case '*':
            push(s, pop(s)*pop(s));
            break;
        case '/': {
            double op2 = pop(s);
            push(s, pop(s) / op2);
            break;
        }
        case '%': {
            double op2 = pop(s);
            push(s, fmod(pop(s), op2));
            break;
        }
        case '^': {
            double op2 = pop(s);
            push(s, pow(pop(s), op2));
            break;
        }
        default:
            break;
        }
}

void uop(struct Stack *s, char op)
{
    switch (op) {
        case '.': /* print and pop */
            printf("%g\n", pop(s));
            break;
        case 'n': { /* print and don't pop */
            double x = pop(s);
            printf("%g\n", x);
            push(s, x);
            break;
        }
        case UOP_SIN: {
            double x = pop(s);
            push(s, sin(x));
            break;
        }
        case UOP_COS: {
            double x = pop(s);
            push(s, cos(x));
            break;
        }
        case UOP_EXP: {
            double x = pop(s);
            push(s, exp(x));
            break;
        }
        case UOP_LN: {
            double x = pop(s);
            push(s, log(x));
            break;
        }
        default:
            break;
    }
}

void constant(struct Stack *s, char *ct)
{
    if (strcmp(ct, "pi") == 0)
        push(s, M_PI);
    else if (strcmp(ct, "e") == 0)
        push(s, M_E);
}
