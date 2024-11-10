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

