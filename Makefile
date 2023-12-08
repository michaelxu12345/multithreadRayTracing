CC = g++

INC = -I includes/
WARNINGS = -Wall -Wextra -Werror
EXEC_WINDOWS = windows
EXEC_UNIX = unix

CFLAGS = $(WARNINGS) $(INC) -std=c++20
CFLAGS_DEBUG = $(CFLAGS) -g

# use @ to not show command
all: windows unix
	@echo "making all"


windows:
	@echo "making windows version"
	$(CC) $(CFLAGS) -o render.exe ./src/test1.cpp 

unix:
	@echo "making unix version"
	$(CC) $(CFLAGS) -o render.out ./src/test1.cpp 

clean:
	@echo "making clean"
	rm render.exe
	rm render.out