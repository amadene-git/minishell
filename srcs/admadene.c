#include <stdio.h>
#define INTEGER 1
#define PLUS 2
#define MINUS 3
#define MULT 4
#define DIV 5
#define RPAREN 6
#define LPAREN 7
#define END 0

typedef struct s_token
{
    int type;
    int value;
} t_token;

typedef struct s_struct
{
    t_token current_tok;
    t_token token;
    int term;
    int factor;
    int pos;
    char *str;
    int result;
} t_struct;

int expr(t_struct *s);

int get_next_token(t_struct *s)
{
    while (s->str[s->pos] && s->str[s->pos] == ' ')
        s->pos++;
    char current_char = s->str[s->pos];
    if (!current_char)
    {
        s->current_tok.value = 0;
        s->current_tok.type = END;
        return (1);
    }
    if (current_char >= '0' && current_char <= '9')
    {
        s->current_tok.value = ft_atoi(s->str + s->pos);
        s->current_tok.type = INTEGER;
        while (s->str[s->pos] >= '0' && s->str[s->pos] <= '9')
            s->pos++;
        return (1);
    }
    if (current_char == '+')
    {
        s->current_tok.value = current_char;
        s->current_tok.type = PLUS;
        s->pos++;
        return (1);
    }
    if (current_char == '-')
    {
        s->current_tok.value = current_char;
        s->current_tok.type = MINUS;
        s->pos++;
        return (1);
    }
    if (current_char == '*')
    {
        s->current_tok.value = current_char;
        s->current_tok.type = MULT;
        s->pos++;
        return (1);
    }
    if (current_char == '/')
    {
        s->current_tok.value = current_char;
        s->current_tok.type = DIV;
        s->pos++;
        return (1);
    }
    if (current_char == '(')
    {
        s->current_tok.value = current_char;
        s->current_tok.type = RPAREN;
        s->pos++;
        return (1);
    }
    if (current_char == ')')
    {
        s->current_tok.value = current_char;
        s->current_tok.type = LPAREN;
        s->pos++;
        return (1);
    }
    return (0);
}

int eat(t_struct *s, int token_type)
{
    if (s->current_tok.type == token_type)
    {
        if (!get_next_token(s))
            return (0);
        return (1);
    }
    return (0);
}

int factor(t_struct *s)
{
    int res;
    if (s->current_tok.type == INTEGER)
    {
        res = s->current_tok.value;
        if (eat(s, INTEGER))
            return (res);
        return (0);
    }
    else if (s->current_tok.type == RPAREN)
    {
        if (!eat(s, RPAREN))
            return (0);
        res = expr(s);
        if (!eat(s, LPAREN))
            return (0);
        return (res);
    }
}

int term(t_struct *s)
{
    int res;
    res = factor(s);
    while (s->current_tok.type == MULT || s->current_tok.type == DIV)
        if (eat(s, MULT))
            res *= factor(s);
        else if (eat(s, DIV))
            res /= factor(s);
    return (res);
}

int expr(t_struct *s)
{
    int res;
    res = term(s);
    while (s->current_tok.type == MINUS || s->current_tok.type == PLUS)
        if (eat(s, PLUS))
            res += term(s);
        else if (eat(s, MINUS))
            res -= term(s);
    return (res);
}

int main(void)
{
    t_struct *s = malloc(sizeof(t_struct));
    write(1, "calc>", 5);
    s->pos = 0;
    s->result = 0;
    get_next_line(0, &s->str);
    printf("expr = |%s|\n", s->str);
    if (!get_next_token(s))
        return (0);
    if (!(s->result = expr(s)))
    {
        printf("0\n");
        free(s);
        return (0);
    }
    printf("result :%d\n", s->result);
    free(s->str);
    free(s);
    return (0);
}