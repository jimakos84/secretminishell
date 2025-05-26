/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsomacha <tsomacha@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 02:00:03 by tsomacha          #+#    #+#             */
/*   Updated: 2025/05/17 02:58:27 by tsomacha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

int	ft_getpid(void)
{
	int		fd;
	int		pid;
	char	buffer[128];
	ssize_t	bytes;

	pid = 0;
	fd = open("/proc/self/stat", O_RDONLY);
	if (fd < 0)
		return (-1);
	bytes = read(fd, buffer, sizeof(buffer) - 1);
	close(fd);
	if (bytes <= 0)
		return (-1);
	buffer[bytes] = '\0';
	pid = ft_atoi(buffer);
	return (pid);
}

int	ft_rand(void)
{
	int			fd;
	int			rand;
	uint32_t	num;

	rand = 0;
	fd = open("/dev/urandom", O_RDONLY);
	if (fd < 0)
		return (-1);
	if (read(fd, &num, sizeof(num)) != sizeof(num))
	{
		close(fd);
		return (-1);
	}
	close(fd);
	rand = (int)(num & 0x7FFFFFFF);
	return (rand);
}

char	*random_filename(void)
{
	int		pid;
	int		rand;
	char	*file_name;

	file_name = NULL;
	pid = ft_getpid();
	rand = ft_rand();
	if (pid == -1 || rand == -1)
		return (file_name);
	file_name = string_build2(ft_strdup("dts-"), ft_itoa(pid));
	file_name = string_build2(file_name, ft_itoa(rand));
	file_name = string_build2(file_name, ft_strdup(".cache"));
	return (file_name);
}

char	*set_cache_file_name(void)
{
	int		is_exist;
	char	*file_name;

	is_exist = 1;
	file_name = random_filename();
	is_exist = access(file_name, F_OK);
	while (is_exist == 0)
	{
		free(file_name);
		file_name = random_filename();
		is_exist = access(file_name, F_OK);
	}
	return (file_name);
}

void	exit_builtin(t_cmd *cmd, t_shell *mini, int **fd)
{
	int		status;

	status = 0;
	status = check_builtin(cmd, mini);
	free(mini->pids);
	free_fds(fd, mini->num_cmds - 1);
	clear_env(mini->initenv->env);
	free(mini->initenv);
	clear_and_exit(mini);
	exit (status);
}
