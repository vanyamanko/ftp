CC = gcc
CFLAGS_COMMON = -std=c11 -pedantic -W -Wall -Wextra
CFLAGS_DEBUG = $(CFLAGS_COMMON) -g -ggdb
CFLAGS_RELEASE = $(CFLAGS_COMMON) -Werror

OBJDIR = ../target

SRCS = server.c ../common/common.c server_func.c
OBJS = $(addprefix $(OBJDIR)/, $(notdir $(SRCS:.c=.o)))
EXEC = server

.PHONY: all clean

all: $(EXEC)

$(EXEC): $(OBJS)
ifeq ($(MODE),debug)
	$(CC) $(CFLAGS_DEBUG) $(OBJS) -o $(EXEC)
else
	$(CC) $(CFLAGS_RELEASE) $(OBJS) -o $(EXEC)
endif

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) $(CFLAGS_COMMON) -c $< -o $@

$(OBJDIR)/common.o: ../common/common.c | $(OBJDIR)
	$(CC) $(CFLAGS_COMMON) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR) $(EXEC)