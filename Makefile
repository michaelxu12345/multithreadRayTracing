CC = clang++

INC = -I includes/
WARNINGS = 

CFLAGS = $(INC) -std=c++11
CFLAGS_DEBUG = $(CFLAGS) -g

# use @ to not show command
all: windows unix
	@echo "making all"


windows:
	@echo "making windows version"
	$(CC) $(CFLAGS) -o render.exe ./src/main.cpp 

unix:
	@echo "making unix version"
	$(CC) $(CFLAGS) -o render.out ./src/main.cpp 

clean:
	@echo "making clean"
	rm render.exe
	rm render.out