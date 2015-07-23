CC = g++
CFLAGS = -Wall -c
LFLAGS = -Wall
LIBS = -lldns -lmysqlpp

OBJS = logger.o querier.o conf.o

dns_querier: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o dns_querier $(LIBS)

logger.o: logger.cpp logger.hpp querier.hpp
	$(CC) $(CFLAGS) logger.cpp
	
querier.o: querier.cpp querier.hpp logger.hpp conf.hpp
	$(CC) $(CFLAGS)  querier.cpp
	
conf.o: conf.cpp conf.hpp
	$(CC) $(CFLAGS) conf.cpp
	
clean:
	\rm -f *.o
	
clean_dist: clean
	\rm dns_querier
