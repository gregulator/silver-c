CFLAGS := --std=c99 -pedantic -Wall -Werror
DEBUG_FLAGS := $(CFLAGS) -g
RELEASE_FLAGS := $(CFLAGS) -O3

INCLUDE_FLAGS := -Iinclude -Iunder_construction

SOURCE_FILES = test_bloom.c

LIB_FLAGS = -I../../include -L../.. -lred -lm

debug:
	make -C ../..
	gcc $(INCLUDE_FLAGS) $(SOURCE_FILES) $(LIB_FLAGS) $(DEBUG_FLAGS) -o test_bloom

release:
	make -C ../.. release
	gcc $(INCLUDE_FLAGS) $(SOURCE_FILES) $(LIB_FLAGS) $(RELEASE_FLAGS) -o test_bloom

run run_debug: debug
	LD_LIBRARY_PATH=../.. ./test_bloom

run_release: release
	LD_LIBRARY_PATH=../.. ./test_bloom
clean:
	rm test_bloom

