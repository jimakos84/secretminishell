# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dvlachos <dvlachos@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/25 11:46:44 by dvlachos          #+#    #+#              #
#    Updated: 2025/03/28 12:49:57 by dvlachos         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

TARGET = minishell

LIBFT_DIR = ./libft
LIBFT_LIB = $(LIBFT_DIR)/libft.a

CMD = cc

CFLAGS = -Wall -Werror -Wextra -g

LDFLAGS = -lreadline

SRCS = srcs/main.c

OBJS = $(SRCS:.c=.o)

all : $(TARGET) 

$(TARGET) : $(OBJS) $(LIBFT_LIB)
	$(CMD) $(CFLAGS) $^ -o $@ $(LDFLAGS) -L$(LIBFT_DIR) -lft

$(LIBFT_LIB):
	$(MAKE) -C $(LIBFT_DIR)

clean :
	rm -f $(OBJS)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean : clean
	rm -f $(TARGET)
	rm -f $(LIBFT_LIB)

re : fclean all

.PHONY : all clean fclean re
