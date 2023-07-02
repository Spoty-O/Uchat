NAME = uchat

SERVER_NAME = uchat_server
SERVER_DIR = server

CLIENT_DIR = client

LIBMX_DIR = frameworks/libmx
LIBMX = $(LIBMX_DIR)/libmx.a
LIBCJSON_DIR = frameworks/cjson
LIBSQLITE_DIR = frameworks/sqlite3

MAKE_M = make -sf Makefile -C
MKDIR_M = mkdir -p
RM = /bin/rm -rf

all: del_font font $(LIBMX)
	@$(MAKE_M) $(CLIENT_DIR) $@
	@$(MAKE_M) $(SERVER_DIR) $@

$(LIBMX):
	@make -sC $(LIBMX_DIR)

$(NAME):
	@$(MAKE_M) $(CLIENT_DIR)

$(SERVER_NAME):
	@$(MAKE_M) $(SERVER_DIR)

del_font:
	@rm -rf ${HOME}/Library/Fonts
	@rm -rf ${HOME}/Library/Fonts/Baloo-Regular.ttf
	@rm -rf ${HOME}/Library/Fonts/Nunito-Regular.ttf
	@rm -rf ${HOME}/Library/Fonts/Alata-Regular.ttf

font:
	@mkdir ${HOME}/Library/Fonts
	@cp client/resources/fonts/Baloo/* ${HOME}/Library/Fonts/
	@cp client/resources/fonts/Nunito/* ${HOME}/Library/Fonts/
	@cp client/resources/fonts/Alata/* ${HOME}/Library/Fonts/


clean:
	@$(MAKE_M) $(CLIENT_DIR) $@
	@$(MAKE_M) $(SERVER_DIR) $@
	@$(MAKE_M) $(LIBMX_DIR) $@

uninstall: del_font
	@$(MAKE_M) $(CLIENT_DIR) $@
	@$(MAKE_M) $(SERVER_DIR) $@
	@$(MAKE_M) $(LIBMX_DIR) $@

reinstall: uninstall all

.PHONY: all clean uninstall reinstall $(NAME) $(SERVER_NAME)