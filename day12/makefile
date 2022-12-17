
CLEANUP = rm -f

C_COMPILER=gcc
ifeq ($(shell uname -s), Darwin)
C_COMPILER=clang
endif

UNITY_ROOT=unity

TEST=test.out

MAIN=a.out

SRC_FILES=$(UNITY_ROOT)/src/unity.c $(UNITY_ROOT)/extras/fixture/src/unity_fixture.c  $(UNITY_ROOT)/extras/memory/src/unity_memory.c test/hill_climbing_test.c test/test_runner.c test/all_tests.c src/hill_climbing.c

INC_DIRS=-Isrc -I$(UNITY_ROOT)/src -I$(UNITY_ROOT)/extras/fixture/src -I$(UNITY_ROOT)/extras/memory/src

all: clean test main

default: $(SRC_FILES)
	$(C_COMPILER) $(CFLAGS) $(INC_DIRS) $(SYMBOLS) $(SRC_FILES) -o $(TARGET)
	- ./$(TARGET)

test: $(SRC_FILES)
	$(C_COMPILER) $(CFLAGS) $(INC_DIRS) $(SYMBOLS) $(SRC_FILES) -o $(TEST)

main: $(SRC_FILES)
	$(C_COMPILER) $(CFLAGS) $(INC_DIRS) $(SYMBOLS) src/main.c src/hill_climbing.c -o $(MAIN)

clean:
	$(CLEANUP) $(TARGET)



