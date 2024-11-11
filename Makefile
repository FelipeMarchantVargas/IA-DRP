all: entrega1

entrega1: entrega1.cpp
	g++ entrega1.cpp -o entrega1

run:
	./entrega1

clean:
	rm -f entrega1
