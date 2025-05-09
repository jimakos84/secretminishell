TARGET = minishell

CMD = cc

CFLAGS = -Werror -Wall -Wextra

LDFLAGS = -lreadline

LIBFT_DIR = ./ft_libft

TARGET_LIBFT = $(LIBFT_DIR)/libft.a

LIBFT_LINK = -L$(LIBFT_DIR) -lft

LIBFT_INC = $(LIBFT_DIR)/libft.h $(LIBFT_DIR)/ft_printf.h $(LIBFT_DIR)/get_next_line.h

SRCS = \
	srcs/exit.c\
	srcs/echo.c\
	srcs/export.c\
	srcs/cleaner.c\
	srcs/cleaner2.c\
	srcs/error.c\
	srcs/execute.c\
	srcs/help.c\
	srcs/help2.c\
	srcs/helper.c\
	srcs/heredoc.c\
	srcs/parser.c\
	srcs/parser2.c\
	srcs/parser3.c\
	srcs/parser4.c\
	srcs/signal.c\
	srcs/token.c\
	srcs/token2.c\
	srcs/validate.c\
	srcs/validate2.c\
	srcs/built_in.c\
	srcs/envrn.c\
	srcs/utils.c\
	srcs/utils2.c\
	srcs/built_in2.c\
	srcs/redirect.c\
	srcs/redirect2.c\
	srcs/quote.c\
	main.c\


OBJS = $(SRCS:.c=.o)

all : $(TARGET)

$(TARGET) : $(TARGET_LIBFT) $(OBJS)
	$(CMD) $(CFLAGS) $(OBJS) $(LIBFT_LINK) -o $(TARGET) $(LDFLAGS)
%.o : %.c
	$(CMD) $(CFLAGS) -c $< -o $@

$(TARGET_LIBFT) :
	@make -C $(LIBFT_DIR)

clean :
	rm -f $(OBJS)
	@make -C $(LIBFT_DIR) clean

fclean : clean
	rm -f $(TARGET)
	@make -C $(LIBFT_DIR) fclean

re : fclean all

.PHONY : all clean fclean re
