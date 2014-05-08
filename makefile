CFLAGS := --std=c99 -pedantic -Wall -Werror
DEBUG_FLAGS := $(CFLAGS) -g
RELEASE_FLAGS := $(CFLAGS) -O3

INCLUDE_FLAGS := -Iinclude -Iunder_construction

SOURCE_FILES = src/red_hash.c src/red_test.c src/red_bloom.c src/red_json.c src/red_string.c

debug:
	gcc -fPIC -rdynamic -shared $(INCLUDE_FLAGS) $(SOURCE_FILES) $(DEBUG_FLAGS) -o libred.so

release:
	gcc -fPIC -rdynamic -shared $(INCLUDE_FLAGS) $(SOURCE_FILES) $(RELEASE_FLAGS) -o libred.so

clean:
	rm libred.so
