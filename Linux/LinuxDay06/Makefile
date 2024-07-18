SRCS:=$(wildcard *.c)
OBJS:=$(patsubst %.c, %.o, $(SRCS))
OUT1:=main1
OUT2:=main2

.PHONY:
ALL:$(OUT1) $(OUT2) 
ALL1:=add.o sub.o
ALL2:=$(main1) $(main2)
${OUT1}: ${ALL1} ${OUT1}.o
	gcc $^ -o $@ -lpthread
${OUT2}: ${ALL1} ${OUT2}.o
	gcc $^ -o $@ -lpthread
%.o: %.c
	$(CC) -c $^ -lpthread
.PHONY: clean rebuild
clean:
	$(RM) ${OBJS}
rebuild: clean $(ALL2)
