CC = gcc
CFLAGS = -Wall -Wextra -g -I/usr/local/include
LDFLAGS = -L/usr/local/lib -lcurl -lmyhtml -lpthread

SRCS = main.c url_queue.c visited_set.c crawler.c html_parser.c
OBJS = $(SRCS:.c=.o)
TARGET = crawler

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
