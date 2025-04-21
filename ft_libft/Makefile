# Target library
NAME = libft.a

# Compiler and Flags
CC = cc
CFLAGS = -Wall -Wextra -Werror -I. -g

# Archive tool
AR = ar rcs
RM = rm -rf

# Directories
SRC_DIR = srcs
PRINTF_DIR = ft_printf
GNL_DIR = ft_gnl

# Source files
SRC_FILES = $(wildcard $(SRC_DIR)/*.c) $(wildcard $(PRINTF_DIR)/*.c) $(wildcard $(GNL_DIR)/*.c)

# Object files
OBJ_FILES = $(SRC_FILES:.c=.o)

# Headers
HEADER = libft.h ft_printf.h get_next_line.h

# Targets
all: $(NAME)

$(NAME): $(OBJ_FILES)
	$(AR) $(NAME) $(OBJ_FILES)

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ_FILES)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
