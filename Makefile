I_PATH = ./include
O_PATH = ./src

OBJS = $(O_PATH)/main.o	$(O_PATH)/deck_builder.o $(O_PATH)/gameboard.o \
			 $(O_PATH)/player.o

EXEC = L5R

CPPFLAGS = -std=c++11 -Wall -I$(I_PATH)
CC = g++

# The @ character is used to silence make's output

$(EXEC): $(OBJS)
	@$(CC) $(FLAGS) $(OBJS) -o $(EXEC)

.PHONY: clean
.SILENT: $(OBJS) # silence implicit rule output

clean:
	@rm -rf $(EXEC) $(OBJS)

run: $(EXEC)
	@./$(EXEC)
