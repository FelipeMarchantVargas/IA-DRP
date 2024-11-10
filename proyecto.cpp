#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <fstream>

struct AED {
    int x, y;
    bool is_new; // true if the AED is new, false if it’s repositioned
    int original_x, original_y; // only used if repositioned
};

struct Event {
    int x, y;
};

struct Instance {
    int n; // number of OHCA events
    float p; // budget
    int R; // coverage radius
    std::vector<Event> events;
    std::vector<AED> aeds;
};

// Función para calcular distancia entre un AED y un evento
double distance(int x1, int y1, int x2, int y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

// Función para contar eventos cubiertos por los AEDs
int count_covered_events(const std::vector<Event>& events, const std::vector<AED>& aeds, int R) {
    int covered = 0;
    for (const auto& event : events) {
        bool is_covered = false;
        for (const auto& aed : aeds) {
            if (distance(event.x, event.y, aed.x, aed.y) <= R) {
                is_covered = true;
                break;
            }
        }
        if (is_covered) covered++;
    }
    return covered;
}

// Algoritmo Greedy para colocar nuevos AEDs hasta agotar presupuesto
void greedy_solution(Instance& instance) {
    while (instance.p >= 1.0) { // Asumiendo 1.0 es el costo de un nuevo AED
        // Encontrar el evento no cubierto más cercano y colocar un AED allí
        int best_x = 0, best_y = 0;
        int max_cover = 0;
        
        for (const auto& event : instance.events) {
            int cover_count = 0;
            for (const auto& e : instance.events) {
                if (distance(event.x, event.y, e.x, e.y) <= instance.R) {
                    cover_count++;
                }
            }
            if (cover_count > max_cover) {
                max_cover = cover_count;
                best_x = event.x;
                best_y = event.y;
            }
        }

        // Colocar un AED en la mejor posición
        instance.aeds.push_back({best_x, best_y, true, best_x, best_y});
        instance.p -= 1.0;
    }
}

// Algoritmo Simulated Annealing para optimizar la ubicación de los AEDs
int simulated_annealing(Instance& instance, double initial_temp, double cooling_rate, int max_iterations) {
    double temp = initial_temp;
    int best_covered = count_covered_events(instance.events, instance.aeds, instance.R);
    std::vector<AED> best_aeds = instance.aeds;
    int iterations = 0;

    while (temp > 1 && iterations < max_iterations) {

	iterations++;

        // Realizar una modificación aleatoria en la ubicación de un AED
        int idx = rand() % instance.aeds.size();
        int old_x = instance.aeds[idx].x;
        int old_y = instance.aeds[idx].y;

        // Movimiento aleatorio (reubicación)
        instance.aeds[idx].x += (rand() % 21 - 10); // Desplazamiento aleatorio de -10 a +10
        instance.aeds[idx].y += (rand() % 21 - 10);

        int covered = count_covered_events(instance.events, instance.aeds, instance.R);

        // Verificar si aceptar el nuevo estado
        if (covered > best_covered || exp((covered - best_covered) / temp) > (double)rand() / RAND_MAX) {
            if (covered > best_covered) {
                best_covered = covered;
                best_aeds = instance.aeds;
            }
        } else {
            // Revertir cambio si no es mejor
            instance.aeds[idx].x = old_x;
            instance.aeds[idx].y = old_y;
        }

        // Enfriamiento
        temp *= cooling_rate;
    }

    // Actualizar la solución con la mejor configuración encontrada
    instance.aeds = best_aeds;

    return iterations;
}

// Función para leer una instancia desde la entrada
Instance read_instance() {
    Instance instance;
    std::cin >> instance.n >> instance.p >> instance.R;

    instance.events.resize(instance.n);
    for (int i = 0; i < instance.n; ++i) {
        int x, y, f;
        std::cin >> x >> y >> f;
        instance.events[i] = {x, y};
        if (f) { // Si hay un AED ya instalado
            instance.aeds.push_back({x, y, false, x, y});
        }
    }

    return instance;
}

// Función para imprimir resultados
void print_results(const Instance& instance, int covered_events, int iterations, double exec_time) {
    double coverage_percent = (double)covered_events / instance.n * 100;
    std::cout << "Events: " << covered_events << "\n";
    std::cout << "Percentage: " << coverage_percent << "%\n";
    std::cout << "Time: " << exec_time << "[seg]\n";
    std::cout << "Iterations: " << iterations << "\n";
    std::cout << "Remaining Budget: " << instance.p << "\n";
    for (size_t i = 0; i < instance.aeds.size(); ++i) {
	std::cout << i + 1 << "- (" << instance.aeds[i].x << ", " << instance.aeds[i].y << ") ";
        std::cout << (instance.aeds[i].is_new ? "N" : "R");
        if (!instance.aeds[i].is_new) {
            std::cout << " (" << instance.aeds[i].original_x << ", " << instance.aeds[i].original_y << ")";
        }
        std::cout << "\n";
    }
}

int main(int argc, char* argv[]) {
    srand(time(0));

    // Leer instancia
    Instance instance = read_instance();

    // Parámetros de Simulated Annealing
    double initial_temp = (argc > 1) ? atof(argv[1]) : 1000.0;
    double cooling_rate = (argc > 2) ? atof(argv[2]) : 0.99;
    int max_iterations = (argc > 3) ? atof(argv[3]) : 1000;

    // Solución Greedy inicial
    greedy_solution(instance);

    // Optimización con Simulated Annealing
    std::clock_t start = std::clock();
    int iterations = simulated_annealing(instance, initial_temp, cooling_rate, max_iterations);
    std::clock_t end = std::clock();
    double exec_time = double(end - start) / CLOCKS_PER_SEC;

    // Resultados finales
    int covered_events = count_covered_events(instance.events, instance.aeds, instance.R);
    print_results(instance, covered_events, iterations, exec_time);

    return 0;
}

