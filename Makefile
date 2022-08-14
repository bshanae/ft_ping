LOG=${info [ft_ping] $1}
SILENT=@

SRC_DIR=src
SRC=$(wildcard $(SRC_DIR)/*.c)

INCL=include

OBJ_DIR_V4=obj_v4
OBJ_DIR_V6=obj_v6
OBJ_V4=$(patsubst %.c,%.o,$(addprefix $(OBJ_DIR_V4)/,$(notdir $(SRC))))
OBJ_V6=$(patsubst %.c,%.o,$(addprefix $(OBJ_DIR_V6)/,$(notdir $(SRC))))

EXE_V4=ft_ping
EXE_V6=ft_ping6

CC=gcc
CFLAGS_V4=$(addprefix -I , $(INCL))
CFLAGS_V6=$(addprefix -I , $(INCL)) -D IPv6
LDFLAGS=

all: $(EXE_V4) $(EXE_V6)

$(EXE_V4): $(OBJ_DIR_V4) $(OBJ_V4)
	${call LOG,Linking executable $@}
	$(SILENT) $(CC) $(OBJ_V4) -o $@ $(LDFLAGS)

$(EXE_V6): $(OBJ_DIR_V6) $(OBJ_V6)
	${call LOG,Linking executable $@}
	$(SILENT) $(CC) $(OBJ_V6) -o $@ $(LDFLAGS)

$(OBJ_DIR_V4)/%.o : $(SRC_DIR)/%.c
	${call LOG,Compiling C object $<}
	$(SILENT) $(CC) $(CFLAGS_V4) -c $< -o $@

$(OBJ_DIR_V6)/%.o : $(SRC_DIR)/%.c
	${call LOG,Compiling C object $<}
	$(SILENT) $(CC) $(CFLAGS_V6) -c $< -o $@

$(OBJ_DIR_V4):
	${call LOG,Creating folder $@}
	$(SILENT) mkdir $@

$(OBJ_DIR_V6):
	${call LOG,Creating folder $@}
	$(SILENT) mkdir $@

clean:
	${call LOG,Deleting objects}
	$(SILENT) rm -rf $(OBJ_DIR)
	${call LOG,Cleaning libft}
	$(SILENT) make -C $(LIBFT_DIR) clean

fclean:
	${call LOG,Deleting objects}
	$(SILENT) rm -rf $(OBJ_DIR)
	${call LOG,Deleting executable}
	$(SILENT) rm -f $(EXE_V4)

re: fclean all
