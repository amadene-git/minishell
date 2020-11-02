#define INTEGER 0
#define PLUS 1
#define ENDOF 2


typedef struct	s_token
{
	int		type;
	char	value;
}				t_token;

typedef struct	s_interpreter
{
	char	*text;
	int		pos;
	char	current_char __attribute__(()),(=text[pos]);
	t_token	current_token;
}				t_interpreter;


int	main()
{
	printf("Hello World !\n");
	return (0);
}
