#include "../includes/minishell.h"

char    *str_plusplus(char *nbr)
{
    int     i;
    int     a;
    char    *str = ft_strdup(nbr);

    i = -1;
    a = 0;
    free(nbr);
    while (str[++i])
        a += (str[i] == '9') ? 0 : 1;
    if (a == 0)
    {
        free(str);
        i += 2;
        str = (char*)malloc(sizeof(char) * (i));
        str[i--] = 0;
        while (--i)
            str[i] = '0';
        str[i] = '1';
        return (str);
    }
    i--;
    while (i >= 0)
    {
        if (str[i] >= '0' && str[i] <= '8')
        {
            str[i] += 1;
            return (str);
        }
        if (str[i] < '0' || str[i] > '9')
            return(NULL);
        i--;
    }
    return (str);
}

t_dlist *init_env(const char **env)
{
    t_dlist	*envlist;
    t_dlist	*elem;
    char	**tab;
    char 	buf[PATHMAX + 1];
	char 	*tmp;

    tab = (char**)malloc(sizeof(char*) * 2);
    tab[0] = ft_strdup("export");
    envlist = get_envlist_from_env(env);
    if (!(elem = dlist_chr(envlist, "PWD")) && getcwd(&buf[0], PATHMAX))
    {
        if(!getcwd(&buf[0], PATHMAX))
        {
            dprintf(2, "minishell: : getcwd: %s\n", strerror(errno));
            return (NULL);
        }
        else
        {
            tab[1] = ft_strjoindoublefree(ft_strdup("PWD="), ft_strdup(&buf[0]));
            built_in_export(2, tab, envlist, 1);
            free(tab[1]);
        }
    }
    if (!(elem = dlist_chr(envlist, "OLDPWD")))
    {
        tab[1] = ft_strdup("OLDPWD");
        built_in_export(2, tab, envlist, 1);
        free(tab[1]);

    }
    if (!(elem = dlist_chr(envlist, "SHLVL")))
    {
        tab[1] = ft_strjoindoublefree(ft_strdup("SHLVL="), ft_strdup("1"));
        built_in_export(2, tab, envlist, 1);
        free(tab[1]);

    }
    else
    	dlist_chr(envlist, "SHLVL")->data->value = str_plusplus(dlist_chr(envlist, "SHLVL")->data->value);
    free(tab[0]);
    free(tab);
    return (envlist);
}

t_dlist	*get_envlist_from_env(const char **env)
{
	int		i;
	t_dlist *begin;
	t_dlist *elem1;
	t_dlist	*elem2;

	if (!env[0] || !env)
		return (NULL);
	begin = dlist_create_elem(create_var(env[0]));
	elem1 = begin;
	i = 1;
	while (env[i])
	{
		if (!(elem2 = dlist_create_elem(create_var(env[i++]))))
			return (NULL);
		elem2->prev = elem1;
		elem1->next = elem2;
		if (env[i])
		{
			if (!(elem1 = dlist_create_elem(create_var(env[i++]))))
				return (NULL);
			elem1->prev = elem2;
			elem2->next = elem1;
		}
	}
	if (!(i % 2))
	{
		begin->prev = elem2;
		elem2->next = begin;
	}
	else
	{
		begin->prev = elem1;
		elem1->next = begin;
	}
	return (begin);
}

char	*get_var_str(t_var *var)
{
	char	*str;
	char	*value;
	int		i;

	str = ft_strjoindoublefree(ft_strdup(var->name), ft_strdup("="));
	value = ft_strdup(var->value);
	i = -1;
	while (value[++i])
	{
		if (value[i] == '"' || value[i] == '\\' || value[i] == '$')
		{
			insert_string(value, ft_strdup("\\"), i, i);
			i++;
		}
	}
	str = ft_strjoindoublefree(str, value);
	return (str);
}

char	**get_env_from_envlist(t_dlist *begin, t_dlist *elem, int lvl)
{
	char 	**env;
	
	if (elem->next != begin)
		env = get_env_from_envlist(begin, elem->next, lvl + 1);
	else
	{
		env = (char**)malloc(sizeof(char*) * (lvl + 2));
		env[lvl + 1] = NULL;
		env[lvl] = get_var_str(elem->data);
		return (env);
	}
	env[lvl] = get_var_str(elem->data);
	return (env);
}

void	envlist_print(const char **env, int fd)
{
	int	i;

	i = -1;
	while (env[++i])
		ft_putendl_fd(env[i], fd);

}

int		built_in_unset(t_cmd *cmd)
{
	int i;

	i = 0;
	while (++i < cmd->ac)
		free_elem(cmd->envlist, cmd->av[i]);
	return (0);

}

int		built_in_env(t_cmd *cmd)
{
	if (cmd->ac != 1)
		return (-1);
	envlist_print((const char**)cmd->env, cmd->fdout);
	return (0);
}



