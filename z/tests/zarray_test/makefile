CFLAGS += -g -I../..
TARGET = ./zarray_test

all: $(TARGET)

$(TARGET) : zarray_test.c ../../zarray.h ../../ztest.h
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm $(TARGET)

run: $(TARGET)
	@$(TARGET)
