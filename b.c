#include <stdio.h>
#define BUF_SIZE 4096

enum
{
    NUMBER,
    PLUS,
    STAR,
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

int num, idx = 0;
char input[BUF_SIZE];

void main()
{
    int result;
    scanf("%s", &input);
    get_token();
    result = expression();
    if (token != END)
        error(3);
    else
        printf("%d\n", result);
}

int expression()
{
    int result;
    result = term();
    while (token == PLUS)
    {
        get_token();
        result = result + term();
    }
    return result;
}

int term()
{
    int result;
    result = factor();
    while (token == STAR)
    {
        get_token();
        result = result * factor();
    }
    return result;
}

int factor()
{
    int result;
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
            error(2);
    }
    else
        error(1);
    return result;
}

void get_token()//실수도 읽을수있도록 수정해야함
{
    while (input[idx] == ' ')
        idx++;
    if (input[idx] >= '0' && input[idx] <= '9')
    {
        token = NUMBER;
        while (input[idx] >= '0' && input[idx] <= '9')
            idx++;
    }
    else
    {
        if (input[idx] == '+')
            token = PLUS;
        else if (input[idx] == '*')
            token = STAR;
        else if (input[idx] == '(')
            token = LP;
        else if (input[idx] == ')')
            token = RP;
        else if (input[idx] == 0)
            token = END;
        else
            error(4);
        idx++;
    }
}

void error(int i)
{
    printf("error\n");
    exit(1);
}