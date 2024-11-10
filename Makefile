all: proyecto

proyecto: proyecto.cpp
	g++ proyecto.cpp -o proyecto

run:
	./proyecto

clean:
	rm -f proyecto
