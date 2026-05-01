CC = gcc
CFLAGS = -std=c99 -Isrc
SRC_DIR = src
SRC = $(SRC_DIR)/client.c $(SRC_DIR)/helper.c $(SRC_DIR)/requests.c $(SRC_DIR)/parson.c
OBJ = $(SRC:.c=.o)
DEPS = $(SRC_DIR)/client.h $(SRC_DIR)/helper.h $(SRC_DIR)/requests.h $(SRC_DIR)/parson.h
EXEC = client

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(SRC_DIR)/*.o $(EXEC)
