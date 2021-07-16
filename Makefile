NAME = philo

FLAGS = clang++ -Wall -Wextra -Werror

OBJ_DIR = obj/

CFILES = main.c

CFIND = $(CFILES:%=%)

OFILES = $(CFILES:%.cpp=%.o)

OBJ = $(addprefix $(OBJ_DIR), $(OFILES))

all: $(OBJ_DIR) $(NAME)

$(OBJ_DIR):
			@mkdir -p $(OBJ_DIR)

$(NAME): $(OBJ)
				$(FLAGS) -o $(NAME) $(addprefix $(OBJ_DIR), $(OFILES))

$(OBJ): $(CFIND)
				@$(MAKE) $(OFILES)
$(OFILES):
			@echo Compiled: $(@:obj/%=%)
			@$(FLAGS)  -c -o $(OBJ_DIR)$@ $(@:%.o=%.cpp)
clean:
				/bin/rm -rf $(OBJ_DIR)

fclean:			clean
				/bin/rm -f $(NAME)

re: 			fclean all