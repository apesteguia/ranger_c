CC = gcc
CFLAGS = -Wall -Wextra -pedantic -lncurses -lmenu -g

SRCDIR = src
OBJDIR = obj
BINDIR = bin

EXEC = $(BINDIR)/program

SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(SRCDIR)/window.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/window.o: $(SRCDIR)/window.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -f $(OBJS) $(EXEC)
