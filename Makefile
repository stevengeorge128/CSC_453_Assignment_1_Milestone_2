CC = gcc
CFlags = -Wall -g
OBJS = driver.o parse.o getToken.o




compile: ${OBJS}
	${CC} ${CFlags} -o compile ${OBJS}

driver.o: driver.c scanner.h
	${CC} ${CFlags} -c driver.c

parse.o: parse.c scanner.h
	${CC} ${CFlags} -c parse.c

getToken.o: getToken.c scanner.h
	${CC} ${CFlags} -c getToken.c

clean:
	rm -f *.o compile
