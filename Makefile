#** ************************************************************************ ***
#                                                                              #
#         __ __         __    __  ___                                          #
#        / // /__ ____ / /   /  |/  /__  __ __                                 #
#       / _  / _ `/ -_) /   / /|_/ / _ \/ // /                                 #
#      /_//_/\_,_/\__/_/   /_/  /_/\___/\_,_/                                  #
#                                                                              #
#      | [ Makefile file ]                                                       #
#      | By: hael-mou <hamzaelmoudden2@gmail.com>                              #
#      | Created: 2024-05-16                                                   #
#                                                                              #
#** ************************************************************************* **

NAME		:=	webserv

#=== Directories : ===========================================================
SRC_DIR		:=	src
OBJ_DIR		:=	.build
INC_DIR		:=	$(SRC_DIR)/utils\
				$(SRC_DIR)/core/configParser\
				$(SRC_DIR)/core/multiplexer\
				$(SRC_DIR)/core/serverCore\
				$(SRC_DIR)/core/reactor\
				$(SRC_DIR)/http/factory\
				$(SRC_DIR)/http/module\
				$(SRC_DIR)/http/handler\
				interface

#=== Files : =================================================================
SRC_FILES	:=	$(SRC_DIR)/utils/Utils.cpp\
				\
				$(SRC_DIR)/core/configParser/Dictionary.cpp\
				$(SRC_DIR)/core/configParser/Directive.cpp\
				$(SRC_DIR)/core/configParser/ConfigParser.cpp\
				\
				$(SRC_DIR)/core/multiplexer/SelectMultiplexer.cpp\
				$(SRC_DIR)/core/reactor/Reactor.cpp\
				\
				$(SRC_DIR)/core/serverCore/ServerCore.cpp\
				$(SRC_DIR)/core/serverCore/main.cpp\
				\
				$(SRC_DIR)/http/factory/HttpFactory.cpp\
				$(SRC_DIR)/http/module/HttpException.cpp\
				$(SRC_DIR)/http/module/HttpCluster.cpp\
				$(SRC_DIR)/http/module/HttpServer.cpp\
				$(SRC_DIR)/http/module/HttpClient.cpp\
				$(SRC_DIR)/http/module/HttpRequest.cpp\
				$(SRC_DIR)/http/module/HttpErrorPage.cpp\
				$(SRC_DIR)/http/module/HttpLocation.cpp\
				$(SRC_DIR)/http/module/HttpAResponse.cpp\
				$(SRC_DIR)/http/module/HttpRawResponse.cpp\
				$(SRC_DIR)/http/module/HttpFileResponse.cpp\
				$(SRC_DIR)/http/module/HttpCgiResponse.cpp\
				$(SRC_DIR)/http/module/HttpBufferReader.cpp\
				$(SRC_DIR)/http/module/HttpChunkReader.cpp\
				\
				$(SRC_DIR)/http/handler/HttpAcceptHandler.cpp\
				$(SRC_DIR)/http/handler/HttpRecvHandler.cpp\
				$(SRC_DIR)/http/handler/HttpSendHandler.cpp\
				$(SRC_DIR)/http/handler/HttpGetHandler.cpp\
				$(SRC_DIR)/http/handler/HttpCgiHandler.cpp

INC_FILES	:=	$(foreach dir, $(INC_DIR), $(wildcard $(dir)/*.hpp))

OBJ_FILES	:=	$(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

#=== Commands : ==============================================================
CPP			:=	c++
RM			:=	rm -rf
CFLAGS		:=	-Wall -Wextra -Werror -std=c++98 #-g -fsanitize=address
INCLUDE		:=	$(addprefix -I , $(INC_DIR))

#=== Variables : =============================================================
CONFIG_FILE :=	default.conf

#=== Colors : =================================================================
DEF			:=	\033[3;39m
GRAY		:=	\033[3;90m
PINK		:=	\033[3;38;5;199m
RED			:=	\033[3;91m
GREEN		:=	\033[3;32m
CYAN		:=	\033[3;96m
PURPLE		:=	\033[3;35m
YELLOW		:=	\033[3;93m

#=== Pattern rules : =========================================================
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(INC_FILES)
	@mkdir -p $(dir $@)
	@$(CPP) $(CFLAGS) $(INCLUDE) -c $< -o $@
	@printf "$(GREEN) [OK]	$(PURPLE)Compiling ==> $(DEF)%5s\n" $<

#=== Rules : ==================================================================
all: $(NAME)

$(NAME): $(OBJ_FILES)
	@$(CPP) $(CFLAGS) $(INCLUDE) $(OBJ_FILES) -o $@
	@printf "$(GREEN) [OK]	$(YELLOW)$@ is created !! $(DEF)\n"

run:
	@if [ ! -f $(NAME) ]; then\
		printf "$(PURPLE) Warning: $(YELLOW) $(NAME) not found !!$(DEF)\n";\
	else\
		make stop > /dev/null;
		if [ -f $(NAME).pid ]; then\
			$(RM) $(NAME).pid;\
		fi;\
		./$(NAME) $(CONFIG_FILE) & printf $$! > $(NAME).pid;\
		printf "$(PURPLE) Notis: $(GREEN) $(NAME) is started!$(DEF) \n";\
	fi

stop:
	@if [ -f $(NAME).pid ]; then\
		kill $(shell cat $(NAME).pid 2> /dev/null) &> /dev/null;\
		$(RM) $(NAME).pid  &> /dev/null;\
		printf "$(PURPLE) Notis: $(RED) $(NAME) is stopped!$(DEF)\n";\
	else\
		printf "$(PURPLE) Warning: $(YELLOW) $(NAME) is not running !!$(DEF)\n";\
	fi

clean:
	@if [ -d $(OBJ_DIR) ]; then\
		$(RM) $(OBJ_DIR);\
		echo "$(GREEN) [OK]  $(RED) Object Files are Cleaned!$(DEF)";\
	fi

fclean: clean
	@if [ -f $(NAME) ]; then\
		$(RM) $(NAME);\
		echo "$(GREEN) [OK]  $(RED) $(NAME) Cleaned!$(DEF)";\
	fi

re:	fclean all

.PHONY: all clean fclean re run stop
