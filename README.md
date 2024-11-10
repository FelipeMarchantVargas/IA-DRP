# IA-DRP: Solución al Defibrillator Relocation Problem (DRP) usando Algoritmos Greedy y Simulated Annealing

Este proyecto implementa una solución al **Defibrillator Relocation Problem (DRP)** utilizando algoritmos de optimización Greedy y **Simulated Annealing** en C++. El objetivo del DRP es maximizar la cobertura de desfibriladores automáticos externos (AEDs) para atender eventos de **Out-of-Hospital Cardiac Arrest (OHCA)**, optimizando su ubicación para reducir el tiempo de respuesta en emergencias.

## Tabla de Contenidos

- [Descripción del Proyecto](#descripción-del-proyecto)
- [Instalación](#instalación)
- [Uso](#uso)
- [Parámetros de Ejecución](#parámetros-de-ejecución)
- [Formato de Entrada](#formato-de-entrada)
- [Ejemplo de Ejecución](#ejemplo-de-ejecución)
- [Salida](#salida)
- [Autor](#autor)

## Descripción del Proyecto

Este proyecto resuelve el DRP mediante:
1. **Algoritmo Greedy**: Coloca AEDs en ubicaciones óptimas hasta agotar el presupuesto disponible.
2. **Simulated Annealing**: Optimiza la ubicación inicial de los AEDs mediante un proceso de recocido simulado para mejorar la cobertura en función de la temperatura y la tasa de enfriamiento.

El programa permite especificar un número máximo de iteraciones para el recocido simulado, además de ajustar parámetros como la temperatura inicial y la tasa de enfriamiento.

## Instalación

1. Clona el repositorio en tu máquina local:
   ```bash
   git clone https://github.com/FelipeMarchantVargas/IA-DRP.git
   cd IA-DRP
   ```
2. Compila el código usando "g++" y el Makefile:
   ```bash
   make
   ```
   Esto generará un ejecutable llamado proyecto.

## Uso

Para ejecutar el programa, usa el siguiente comando en la terminal:

   ```bash
   ./proyecto < <archivo_instancia> [temperatura_inicial] [tasa_enfriamiento] [max_iteraciones]
   ```
## Parámetros de Ejecución

- <archivo_instancia>: Ruta del archivo que contiene la instancia del problema (obligatorio).
- [temperatura_inicial]: (Opcional) Temperatura inicial para el algoritmo de Simulated Annealing. Valor por defecto: 1000.0.
- [tasa_enfriamiento]: (Opcional) Tasa de enfriamiento para el algoritmo de Simulated Annealing. Valor por defecto: 0.99.
- [max_iteraciones]: (Opcional) Número máximo de iteraciones para el algoritmo de Simulated Annealing. Valor por defecto: 1000.

### Ejemplo

   ```bash
   ./proyecto instancias/ejemplo_instancia.txt 1000.0 0.99 500
   ```

## Formato de Entrada

El archivo de entrada debe tener el siguiente formato:
   ```bash
   n p R
   x1 y1 f1
   x2 y2 f2
   ...
   xn yn fn
   ```

- n: Número de eventos OHCA a cubrir.
- p: Presupuesto disponible (cada nueva instalación cuesta 1.0 y cada reubicación cuesta 0.2).
- R: Radio de cobertura de los AEDs.
- Cada línea siguiente define un evento:
   - xi, yi: Coordenadas del evento o AED ya instalado.
   - fi: indicador (flag) que indica si ya hay un AED instalado en esas coordenadas (1 si hay un AED, 0 si no).

## Ejemplo de Ejecución

### Ejemplo de archivo de entrada

   ```bash
   10 5.0 100
   10 20 0
   15 25 0
   30 40 1
   50 60 0
   70 80 0
   20 10 1
   80 30 0
   60 10 0
   40 90 1
   10 70 0
   ```

## Salida

La salida del programa tiene el siguiente formato:
   ```bash
   Events: <número de eventos cubiertos>
   Percentage: <porcentaje de eventos cubiertos>%
   Time: <tiempo de ejecución en segundos> [seg]
   Iterations: <número de iteraciones>
   Budget: <presupuesto sobrante>
   1- (x1, y1) R (x1_original, y1_original)   # R para reposicionados con coordenadas originales
   2- (x2, y2) N                             # N para nuevos AEDs
   ...
   ```
### Ejemplo de Salida

   ```bash
   Events: 9
   Percentage: 90%
   Time: 0.001 [seg]
   Iterations: 5
   Budget: 0.1
   1- (10, 83) R (1, 90)
   2- (33, 70) N
   ```
- Events: Número total de eventos OHCA cubiertos por los AEDs.
- Percentage: Porcentaje de eventos OHCA cubiertos.
- Time: Tiempo de ejecución en segundos.
- Iterations: Número de iteraciones ejecutadas en el algoritmo de Simulated Annealing.
- Budget: Presupuesto restante después de colocar y optimizar la ubicación de los AEDs.
- Cada AED en la salida indica su posición final y si es nuevo (N) o reposicionado (R) junto con sus coordenadas originales si aplica.

## Autor

Proyecto desarrollado por Felipe Marchant Vargas como parte de un proyecto de Inteligencia Artificial para resolver el Defibrillator Relocation Problem (DRP) usando técnicas de optimización en C++.


Este archivo `README.md` incluye:
- Una descripción general del proyecto y sus algoritmos.
- Instrucciones de instalación y compilación.
- Detalles sobre cómo ejecutar el programa y los parámetros que admite.
- El formato de entrada y un ejemplo.
- La estructura de la salida esperada con ejemplos.
- Información del autor.

Este README proporciona toda la información que un usuario necesita para comprender, instalar, ejecutar y utilizar el proyecto.
