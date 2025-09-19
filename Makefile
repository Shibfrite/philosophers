NAME		:= philosophers

CC			:= gcc
CFLAGS		:= -Wall -Wextra -Werror -g
RM			:= rm -rf

INC_DIR 	:= includes
SRC_DIR 	:= sources
OBJ_DIR		:= objects

SRC_FILES   := main.c routine.c log.c threads.c
SRC_PREFIX  := $(SRC_DIR)/
SRCS        := $(addprefix $(SRC_PREFIX),$(SRC_FILES))
OBJS		:= $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
INC			:= -I$(INC_DIR)
DIRS		:= $(INC_DIR) $(SRC_DIR) $(OBJ_DIR)

all:		$(NAME)

$(NAME):	$(OBJS)
	echo "Linking $(NAME)..."
	$(CC) $(CFLAGS) $(OBJS) -o $@
	echo "$(NAME) created successfully!"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(DIRS)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(DIRS):
	mkdir -p $@
	echo "Created directory: $@"

clean:
	echo "Cleaning object files..."
	$(RM) $(OBJ_DIR)
	echo "Object files cleaned."

fclean: clean
	echo "Cleaning executable and library..."
	$(RM) $(NAME)
	echo "Executable and library cleaned."

re: fclean all

.SILENT:
.PHONY: all clean fclean re
