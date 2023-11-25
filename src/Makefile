CC = g++
OUTPUT = test1.exe

ifeq ($(OS), WINDOWS_NT)
	@echo "hi"
	$(eval OUTPUT=test1.exe)
endif

INC = -I ../includes/
WARNINGS = -Wall -Wextra -Werror

CFLAGS = $(WARNINGS) $(INC) -std=c++20 -o $(OUTPUT)
CFLAGS_DEBUG = $(CFLAGS) -g

# use @ to not show command
all:
	@echo "making all"
	$(CC) $(CFLAGS) main.cpp

clean:
	@echo "making clean"
	rm $(OUTPUT)