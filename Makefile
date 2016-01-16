CC = gcc
OBJECTS = src/bask_core.c src/bask_errors.c src/bask_time.c src/bask_config.c src/bask_ui.c src/bask_priority.c src/bask_views.c src/bask_project.c src/bask_filter.c src/bask_task.c src/bask_export.c src/bask_import.c bask.c
LIBS = -lm
CFLAGS = -Wall -O2
BINDIR = $(DESTDIR)/usr/bin
NAME = bask

bask: $(OBJECTS)
	$(CC) -o $(NAME) $(OBJECTS) $(LIBS)

%.o: %.c
	$(CC) -c $(CFLAGS) $<

