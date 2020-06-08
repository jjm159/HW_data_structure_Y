CC = g++
CFLAGS = -W -WALL -std=c++17
TARGET = Simuation
$(TARGET) : simulation_main.o
			$(CC) $(CFLAGS) -o $(TARGET) simulation_main.o

simulation_main.o : simulation_main.cpp
			$(CC) $(CFLAGS) -c -o simulation_main.o simulation_main.cpp

clean :
	rm *.o Simuation graph.txt
