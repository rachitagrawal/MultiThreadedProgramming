CC = g++
CFLAGS = -pthread -O3
CODE=LockFree
LockFree: Main.o
	$(CC) $(CFLAGS) ./$(.TARGET)/TestMain.cpp

WaitFree:
	$(CC) -pthread -O3 ./WaitFree/TestMain.cpp

OptimizedLockFree:
	$(CC) -pthread -O3 ./OptimizedLockFree/TestMain.cpp

gOptiWait:
	$(CC) -pthread -O3 ./OptimizedWaitFree/TestMain.cpp

run: LockFree
	./a.out

clean:
	rm a.out

TestMain.o: TestMain.cpp Main.o
	$(CC) -c $(CFLAGS) ./$(CODE)/Testmain.cpp

Main.o: LockFree.o Node.o
	$(CC) -c $(CFLAGS) ./$(CODE)/Main.cpp

LockFree.o: Main.o
	$(CC) -c $(CFLAGS) ./$(CODE)/LockFree.cpp
