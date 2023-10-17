CC = g++

INC = -I../includes/
WARNINGS = -Wall -Wextra -Werror

CFLAGS = $(WARNINGS) $(INC) -std=c++20 -o test1.out
CFLAGS_DEBUG = $(CFLAGS) -g

# use @ to not show command
all: test1.cpp
	@echo "making all"
	$(CC) $(CFLAGS) test1.cpp

clean:
	@echo "making clean"
	rm test1.out