#定义变量 
#add.c sub.c main.c mult.c div.c
src=$(wildcard ./*.c)
objs = $(patsubst %.c,%.o,$(src))
target=app

$(target):$(objs)
	$(CC)  $^ -o $(target)

%.o:%.c
	$(CC) -c $< -o $@

.PHONY:clean
clean:
	rm $(objs) -f
