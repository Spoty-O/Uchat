NAME = uchat_server

SRC_DIR = src
OBJ_DIR = obj
INC_DIR = inc

LIBMX_DIR = ../frameworks/libmx

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(addprefix $(OBJ_DIR)/, $(notdir $(SRC_FILES:%.c=%.o)))
INC_FILES = $(wildcard $(INC_DIR)/*.h)

JSON_C = ../frameworks/json-c/0.15/lib/libjson-c.a

# SSL_FILES_1 = ../frameworks/openssl@3/3.0.1/lib
# SSL_INC_FILES:= ../frameworks/openssl@3/3.0.1/include/openssl/

SSL_FILES_1 = $(HOME)/.brew/Cellar/openssl@3/3.0.1/lib
SSL_INC_FILES:= $(HOME)/.brew/Cellar/openssl@3/3.0.1/include/openssl/

SQLITE = -l sqlite3

LIBS_FLAGS = -lmx -lssl -lcrypto\
			 -L$(SSL_FILES_1) -I$(SSL_INC_FILES)
			#  -L$(SSL_FILES) -I$(SSL_INC_FILES) \
			#  -L$(SSL_DIR) -I$(SSL_INC_FILES) \

CC = clang

CFLAGS = -std=c11 $(addprefix -W, all extra error pedantic) -g \

MKDIR = mkdir -p
RM = rm -rf

all: clean $(NAME)

$(NAME): $(OBJ_FILES)
	@$(CC) $(CFLAGS) $(OBJ_FILES) $(JSON_C) $(LIBS_FLAGS) $(SQLITE) -L$(LIBMX_DIR) -lmx -o $@
	@printf "\r\33[2K$@\t \033[32;1mcreated\033[0m\n"
	@mv $@ ../

$(OBJ_FILES): | $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INC_FILES)
	@$(CC) $(CFLAGS) -c $< -o $@ -I $(INC_DIR) -I $(LIBMX_DIR)/$(INC_DIR)
	@printf "\r\33[2K$(NAME)\033[33;1m\t compile \033[0m$(<:$(SRC_DIR)/%.c=%)\r"

$(OBJ_DIR):
	@$(MKDIR) $@

clean:
	@$(RM) $(OBJ_DIR)
	@printf "$(OBJ_DIR) in $(NAME)\t \033[31;1mdeleted\033[0m\n"

uninstall:
	@$(RM) $(OBJ_DIR)
	@$(RM) ../$(NAME) $(NAME)
	@@printf "$(NAME)\t \033[31;1muninstalled\033[0m\n"

reinstall: uninstall all

.PHONY: all uninstall clean reinstall

