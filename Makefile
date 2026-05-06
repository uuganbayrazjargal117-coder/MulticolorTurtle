CC      = gcc
CFLAGS  = -Wall -Wextra -std=c99 -g -I$(SRCDIR) -D_GNU_SOURCE
LDFLAGS = -lGL -lGLU -lglut -lm

SRCDIR  = src
OBJDIR  = build
TARGET  = multicolor_turtle

SRCS = $(SRCDIR)/main.c \
       $(SRCDIR)/game.c \
       $(SRCDIR)/renderer.c \
       $(SRCDIR)/shagai.c \
       $(SRCDIR)/board.c \
       $(SRCDIR)/input.c \
       $(SRCDIR)/ui.c

OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))

.PHONY: all clean

all: $(OBJDIR) $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR) $(TARGET)
