CC = gcc
CFlags = -Wall -g
OBJS = scanner-driver.o getToken.o


scanner: ${OBJS}
	${CC} ${CFlags} -o scanner ${OBJS}

scanner-driver.o: scanner-driver.c scanner.h
	${CC} ${CFlags} -c scanner-driver.c

getToken.o: getToken.c scanner.h
	${CC} ${CFlags} -c getToken.c

clean:
	rm -f *.o scanner
