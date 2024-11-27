# Manual del Programa para el Problema DRP (Defibrillator Relocation Problem)

Este programa resuelve instancias del problema DRP utilizando un enfoque basado en una solución inicial greedy optimizada con el algoritmo Simulated Annealing (SA).

## Requisitos
1. Un compilador compatible con C++11 o superior.
2. `make` instalado para compilar el programa.
3. Un archivo de entrada en formato `instancia.txt` con las especificaciones del problema.

## Estructura del Archivo de Entrada
El archivo `instancia.txt` debe contener:
1. El número de eventos OHCA (n), el presupuesto disponible (p) y el radio de cobertura (R).
2. Una lista de eventos con las siguientes propiedades por línea:  
   - Coordenada X del evento.  
   - Coordenada Y del evento.  
   - Un indicador binario (0 o 1) de si el evento ya tiene un AED instalado.


## Compilación
Para compilar el programa, simplemente ejecuta:
make

Esto generará un archivo ejecutable llamado `entrega2`.

## Ejecución
### Uso Básico
Para ejecutar el programa con los parámetros predeterminados:

./entrega2 < instancia.txt

Parámetros predeterminados:
- Iteraciones máximas (`max_it`): 5000.
- Temperatura inicial (`init_temp`): 1000.0.
- Temperatura final (`fin_temp`): 1.0.
- Factor de enfriamiento (`cooling_factor`): 0.95.

### Uso Avanzado
Para personalizar los parámetros del algoritmo Simulated Annealing:

./entrega2 max_it init_temp fin_temp cooling_factor < instancia.txt

- `max_it`: Número máximo de iteraciones del algoritmo SA.
- `init_temp`: Temperatura inicial para el algoritmo SA.
- `fin_temp`: Temperatura final para detener el algoritmo SA.
- `cooling_factor`: Factor de enfriamiento para diversificar en el algoritmo de SA.

**Ejemplo**:

./entrega2 10000 1500.0 0.5 0.9 < instancia.txt


## Salidas
1. **Resultados en Consola**:
   - Eventos cubiertos.
   - Porcentaje de cobertura respecto al total de eventos.
   - Número de iteraciones realizadas.
   - Tiempo de ejecución en segundos.

2. **Archivo de Salida**:
   El programa genera un archivo `Posiciones.txt` con las coordenadas de los AEDs instalados:


## Notas
- Si el archivo `instancia.txt` no está correctamente formateado, el programa puede fallar o generar resultados incorrectos.
- Es importante ajustar `max_it`, `init_temp` y `fin_temp` según el tamaño de la instancia para obtener resultados óptimos.
