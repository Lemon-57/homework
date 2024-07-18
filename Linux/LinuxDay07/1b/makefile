SRC:=$(wildcard *.c)
OUT:=$(patsubst %.c,%,$(SRC))
CC:=gcc
$(OUT):$(SRC)
	$(CC) $^ -o $(OUT)
.PHONY:clean rebuild
rebuild:clean $(OUT)
clean:
	$(RM) $(OUT)
