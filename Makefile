CC = gcc
OBJECTS = src/bask_ui.c src/bask_views.c src/bask_project.c src/bask_task.c bask.c
LIBS = -lm
CFLAGS = -Wall -O2
BINDIR = $(DESTDIR)/usr/bin
NAME = bask

bask: $(OBJECTS)
	$(CC) -o $(NAME) $(OBJECTS) $(LIBS)

%.o: %.c
	$(CC) -c $(CFLAGS) $<
