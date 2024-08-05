CC=gcc
CFLAGS=-Wall -Iinclude
LDFLAGS=-lssl -lcrypto

BUILD_DIR=build
TEST_DIR=test
OUT_DIR=output
SRC_DIR=src

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

TARGET=$(BUILD_DIR)/compare

all: $(TARGET)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

run:
	./$(TARGET) $(ARGS)

test:
	./test.sh
 
clean:
	rm -rf $(BUILD_DIR) $(TEST_DIR) $(OUT_DIR)/*