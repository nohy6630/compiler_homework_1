#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

enum
{
    NUMBER,
    PLUS,
    MINUS,
    STAR,
    DIV,
    LP,
    RP,
    END
} token;

typedef enum
{
    INT,
    FLT
} kind;

typedef struct
{
    kind t;
    int i;
    float f;
} RET;

RET calculate(RET a, int op, RET b);
RET expression();
RET term();
RET factor();
void skip_whitespace();
void get_token();
void warning();
void error();

RET num;
char ch=' ';

int main()
{
    RET result;
    get_token();
    result = expression();
    if (token != END)
        error();
    else
    {
        if (result.t == INT)
            printf("%d\n", result.i);
        else
            printf("%f\n", result.f);
    }
    return 0;
}

RET calculate(RET a, int op, RET b)
{
    RET ret;
    if (a.t != b.t)
        warning();
    float x = (float)(a.t == INT ? a.i : a.f);
    float y = (float)(b.t == INT ? b.i : b.f);
    if (a.t == FLT || b.t == FLT)
    {
        ret.t = FLT;
        switch (op)
        {
        case PLUS:
            ret.f = x + y;
            break;
        case MINUS:
            ret.f = x - y;
            break;
        case STAR:
            ret.f = x * y;
            break;
        case DIV:
            if (y == 0)
                error();
            ret.f = x / y;
        }
    }
    else
    {
        ret.t = INT;
        switch (op)
        {
        case PLUS:
            ret.i = (int)x + (int)y;
            break;
        case MINUS:
            ret.i = (int)x - (int)y;
            break;
        case STAR:
            ret.i = (int)x * (int)y;
            break;
        case DIV:
            if (y == 0)
                error();
            ret.i = (int)x / (int)y;
        }
    }
    return ret;
}

RET expression()
{
    RET result;
    result = term();
    while (token == PLUS || token == MINUS)
    {
        int op = token;
        get_token();
        result = calculate(result, op, term());
    }
    return result;
}

RET term()
{
    RET result;
    result = factor();
    while (token == STAR || token == DIV)
    {
        int op = token;
        get_token();
        result = calculate(result, op, factor());
    }
    return result;
}

RET factor()
{
    RET result;
    if (token == NUMBER)
    {
        result = num;
        get_token();
    }
    else if (token == LP)
    {
        get_token();
        result = expression();
        if (token == RP)
            get_token();
        else
            error();
    }
    else
        error();
    return result;
}

void skip_whitespace()
{
    while (ch == ' ' || ch == '\t')
        ch = getchar();
}

void get_token()
{
    skip_whitespace();
    if (isdigit(ch))
    {
        num.t = INT;
        num.i = 0;
        do
        {
            num.i = num.i * 10 + (ch - '0');
            ch = getchar();
            skip_whitespace();
        } while (isdigit(ch));
        if (ch == '.')
        {
            ch = getchar();
            skip_whitespace();
            if (isdigit(ch))
            {
                num.t = FLT;
                num.f = (float)num.i;
                float offset = 0.1;
                do
                {
                    num.f = num.f + offset * (ch - '0');
                    offset *= 0.1f;
                    ch = getchar();
                    skip_whitespace();
                } while (isdigit(ch));
            }
            else
                error();
        }
        token = NUMBER;
    }
    else if (ch == '+')
    {
        ch = getchar();
        token = PLUS;
    }
    else if (ch == '-')
    {
        ch = getchar();
        token = MINUS;
    }
    else if (ch == '*')
    {
        ch = getchar();
        token = STAR;
    }
    else if (ch == '/')
    {
        ch = getchar();
        token = DIV;
    }
    else if (ch == '(')
    {
        ch = getchar();
        token = LP;
    }
    else if (ch == ')')
    {
        ch = getchar();
        token = RP;
    }
    else if (ch == '\n')
        token = END;
    else
        error();
}

void warning()
{
    printf("warning\n");
}

void error()
{
    printf("error\n");
    exit(1);
}