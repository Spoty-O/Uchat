NAME = uchat
SRCS = $(wildcard $(SRC_DIR)/*.c)
HDRS = $(wildcard $(INC_DIR)/*.h)

SRC_DIR = src
OBJ_DIR = obj
INC_DIR = inc

CC = clang

CFLAGS = -std=c11 $(addprefix -W, all extra error pedantic) -g \

LIBMX_DIR = ../frameworks/libmx

OBJ_FILES = $(addprefix $(OBJ_DIR)/, $(notdir $(SRCS:%.c=%.o)))

JSON_C = ../frameworks/json-c/0.15/lib/libjson-c.a

GTK_LINK_FLAGS = `pkg-config --cflags gtk4`
GTK_LINK_LIB = `pkg-config --libs gtk4`

MKDIR = mkdir -p
RM = rm -rf

all: clean uchat

uchat: $(OBJ_FILES)
	@$(CC) $(CFLAGS) $(SRCS) $(GTK_LINK_LIB) $(GTK_LINK_FLAGS) $(JSON_C) -L$(LIBMX_DIR) -lmx -o $@
	@printf "\r\33[2K$@\t \033[32;1mcreated\033[0m\n"
	@mv $@ ../

$(OBJ_FILES): | $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HDRS)
	@$(CC) $(CFLAGS) -c $< -o $@ -I $(INC_DIR) $(GTK_LINK_FLAGS) -I $(LIB_DIR)/$(INC_DIR)
	@printf "\r\33[2K$(NAME)\033[33;1m\t compile \033[0m$(<:$(SRC_DIR)/%.c=%)\r"

$(OBJ_DIR):
	@$(MKDIR) $@

$(CJSON_FILES):
	@make -sC $(CJSON_DIR)

clean:
	@$(RM) $(OBJ_DIR)
	@$(RM) uchat.dSYM
	@printf "$(OBJ_DIR) in $(NAME)\t \033[31;1mdeleted\033[0m\n"

uninstall:
	@$(RM) $(OBJ_DIR)
	@$(RM) uchat.dSYM
	@$(RM) ../$(NAME)
	@@printf "$(NAME)\t \033[31;1muninstalled\033[0m\n"

reinstall: uninstall uchat

.PHONY: all uchat clean uninstall reinstall