CC = g++
CFLAGS = -Wall -Wextra -O3
LDFLAGS =

TARGET = rush_hour_game
SRC_DIR = CSS_Files
INC_DIR = Headers_Files
DATA_DIR = Data
INTERFACE_DIR = Interfaces

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
HEADERS = $(wildcard $(INC_DIR)/*.h)
DATA = $(wildcard $(DATA_DIR)/*.txt)
INTERFACES = $(wildcard $(INTERFACE_DIR)/*.txt)

all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS) $(DATA) $(INTERFACES)
	$(CC) $(CFLAGS) -I$(INC_DIR) $(SOURCES) -o $(TARGET) $(LDFLAGS)
	./$(TARGET)
	rm -f $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean
