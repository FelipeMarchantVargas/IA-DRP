all: entrega2

entrega2: entrega2.cpp
	g++ entrega2.cpp -o entrega2

run:
	./entrega2

clean:
	rm -f entrega2
