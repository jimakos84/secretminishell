/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsomacha <tsomacha@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 06:41:52 by tsomacha          #+#    #+#             */
/*   Updated: 2025/05/14 06:43:14 by tsomacha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

int	validation_for_onechar(char *input);
int	validation_for_twochar(char *input);
int	validation_for_threechar(char *input);

int	validation_for_onechar(char *input)
{
	char	c;

	c = input[0];
	if (c == '\0' || c == '<' || c == '>' || c == '|')
		return (1);
	return (0);
}

int	validation_for_twochar(char *input)
{
	if (input[0] == '>')
	{
		if (input[1] == '<' || input[1] == '|')
			return (1);
	}
	if (input[0] == '<')
	{
		if (input[1] == '>' || input[1] == '|')
			return (1);
	}
	return (0);
}

int	validation_for_threechar(char *input)
{
	char	c1;
	char	c2;
	char	c3;

	c1 = input[0];
	c2 = input[1];
	c3 = input[2];
	if (c1 == '<' && (c2 == '|' || c2 == '>'))
		return (1);
	if (c1 == '<' && c2 == '<' && (c3 == '>' || c3 == '|' || c3 == '<'))
		return (1);
	if (c1 == '>' && (c2 == '|' || c2 == '<'))
		return (1);
	if (c1 == '>' && c2 == '>' && (c3 == '<' || c3 == '|' || c3 == '>'))
		return (1);
	return (0);
}
