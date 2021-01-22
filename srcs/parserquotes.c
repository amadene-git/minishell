#define	END		0
#define	COMMAND	1
#define	SEP		2

typedef struct	s_tok
{
	char	*value;
	int		type;
}				t_tok;

int		is_command(char c)
{
	if ((c => 'a' && c <= 'z') ||\
		(c => 'A' && c <= 'Z') ||\
		(c == '''))
}

t_tok	*get_tok(char *str)
{
	while (*str && c == ' ')
		str++;
	if (!*str)
		return (NULL);
	if (c => 'a' && c <= 'z')
	
}

int main(int ac, char **av)
(
	const char *str = "salut'ca'va";
)
