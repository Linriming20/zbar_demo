CC := gcc 
CFLAGS := -I./include -pthread -Wall
LDFLAGS :=

# 控制程序里的DEBUG打印开关
ifeq ($(DEBUG), 1)
CFLAGS += -DENABLE_DEBUG
endif

src := main.c
static_libs := lib/libzbar.a
target := zbar_demo


all : $(target)


$(target) : $(src)
	$(CC) $^ $(static_libs) $(CFLAGS) $(LDFLAGS) -o $@

clean: 
	rm -rf $(target)
.PHONY := clean
