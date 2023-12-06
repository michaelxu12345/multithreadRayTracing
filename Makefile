CC = g++

INC = -I includes/
WARNINGS = -Wall -Wextra -Werror

CFLAGS = $(WARNINGS) $(INC) -std=c++20
CFLAGS_DEBUG = $(CFLAGS) -g

# use @ to not show command
all:
	@echo "making all"
	$(CC) $(CFLAGS) ./src/test1.cpp

clean:
	@echo "making clean"
	rm test1.out