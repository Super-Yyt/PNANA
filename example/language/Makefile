# Makefile 示例
# 展示 Makefile 的语法特性

# 变量定义
CC = gcc
CXX = g++
CFLAGS = -Wall -O2 -Iinclude
CXXFLAGS = $(CFLAGS) -std=c++17
LDFLAGS = -lm

# 目标文件
TARGET = myprogram
SRCS = main.c utils.c math.c
OBJS = $(SRCS:.c=.o)
DEPS = $(SRCS:.c=.d)

# 默认目标
.PHONY: all
all: $(TARGET)

# 主要目标
$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

# 模式规则
%.o: %.c
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

# 清理目标
.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJS) $(DEPS)

# 非常清理
.PHONY: distclean
distclean: clean
	rm -f *~

# 安装目标
.PHONY: install
install: $(TARGET)
	install -d $(DESTDIR)/usr/bin
	install $(TARGET) $(DESTDIR)/usr/bin/

# 卸载目标
.PHONY: uninstall
uninstall:
	rm -f $(DESTDIR)/usr/bin/$(TARGET)

# 测试目标
.PHONY: test
test: $(TARGET)
	./$(TARGET) --test

# 帮助目标
.PHONY: help
help:
	@echo "Available targets:"
	@echo "  all       - Build the program (default)"
	@echo "  clean     - Remove object files and executable"
	@echo "  distclean - Remove all generated files"
	@echo "  install   - Install the program"
	@echo "  uninstall - Uninstall the program"
	@echo "  test      - Run tests"
	@echo "  help      - Show this help message"

# 包含依赖文件
-include $(DEPS)

# 特殊目标
.SECONDARY: $(OBJS)
.DELETE_ON_ERROR:

# 条件编译示例
ifdef DEBUG
CFLAGS += -g -DDEBUG
endif

ifdef OPTIMIZE
CFLAGS += -O3 -march=native
endif

# 函数使用
sources = $(wildcard *.c)
objects = $(patsubst %.c,%.o,$(sources))

# 多行变量
define compile_c
@echo "Compiling $< -> $@"
$(CC) $(CFLAGS) -c $< -o $@
endef

# 自动变量使用
$(OBJS): %.o: %.c
	$(compile_c)
