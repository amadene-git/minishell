#include "../includes/minishell.h"

void	shlvl_plusplus(t_dlist *shlvl)
{
	long int nb;

	nb = (unsigned)ft_atoi(shlvl->data->value);
	nb = (nb >= 0) ? nb + 1 : 0;
	free(shlvl->data->value);
	shlvl->data->value = ft_itoa(nb);
}

void	clean_spaces(t_dlist *shlvl)
{
	int 	i;
	char	*copy;
	
	i = 0;
	while (*shlvl->data->value == ' ')
		shlvl->data->value++;
	copy = ft_strdup(shlvl->data->value);
	//free(shlvl->data->value);
	shlvl->data->value = copy;
} 

t_dlist *init_env(const char **env)
{
    t_dlist	*envlist;
    t_dlist	*elem;
    char 	*buff;
	char 	*tmp;

    envlist = get_envlist_from_env(env);
    if (!(elem = dlist_chr(envlist, "PWD")))
    {
		buff = ft_calloc(PATHSIZE + 1, sizeof(char));
        if(!getcwd(buff, PATHSIZE))
        {
            dprintf(2, "minishell: : getcwd: %s\n", strerror(errno));
            return (NULL);
        }
        else
		{
            tmp = ft_strjoindoublefree(ft_strdup("PWD="), ft_strdup(buff));
			elem = insert_var(envlist, create_var((const char*)tmp));
			free(tmp);
		}
		free(buff);
	}
    if (!(elem = dlist_chr(envlist, "SHLVL")))
    {
            tmp = ft_strjoindoublefree(ft_strdup("SHLVL="), ft_strdup("1"));
			elem = insert_var(envlist, create_var((const char*)tmp));
			free(tmp);
    }
    else
	{
		shlvl_plusplus(elem);
		clean_spaces(elem);
	}
	if (!(elem = dlist_chr(envlist, "_")))
    {
            tmp = ft_strjoindoublefree(ft_strdup("_="), ft_strdup("?"));
			elem = insert_var(envlist, create_var((const char*)tmp));
			free(tmp);
    }

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
	if (var->value)
		value = ft_strdup(var->value);
	else
		value = NULL;
	i = -1;
	while (value && value[++i])
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
	{
		if (elem->data->value)
			env = get_env_from_envlist(begin, elem->next, lvl + 1);
		else
			env = get_env_from_envlist(begin, elem->next, lvl);
	}
	else
	{
		env = (char**)malloc(sizeof(char*) * (lvl + 2));
		env[lvl + 1] = NULL;
		env[lvl] = get_var_str(elem->data);
		return (env);
	}
	if (elem->data->value)
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
	int	status;

	i = 0;
	status = 0;
	while (++i < cmd->ac)
	{
		if (!is_valid_name(cmd->av[i]))
		{
			dprintf(2, "minishell: unset: `%s': not a valid identifier\n", cmd->av[i]);
			status = 1;
		}
		else if (!dlist_chr(cmd->envlist, cmd->av[i]))
			status = 1;
		else
			free_elem(cmd->envlist, cmd->av[i]);
		
	}
	return (status);

}

int		built_in_env(t_cmd *cmd)
{
	if (cmd->ac != 1)
		return (-1);
	envlist_print((const char**)cmd->env, cmd->fdout);
	return (0);
}



