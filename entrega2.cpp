#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <random>
#include <chrono>
#include <algorithm>

struct AED {
    int x, y;
    bool is_new;
};

struct Event {
    int x, y;
    bool has_aed;
};

struct Instance {
    int n; // Número de eventos OHCA
    float p; // Presupuesto
    int R; // Radio de cobertura
    std::vector<Event> events; // Vector de eventos
    std::vector<AED> aeds; // Vector de AEDs instalados
};

// Función para calcular la distancia entre dos puntos
double distance(int x1, int y1, int x2, int y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

// Función para contar eventos cubiertos
int count_covered_events(const std::vector<Event>& events, const std::vector<AED>& aeds, int R) {
    int covered = 0;
    for (const auto& event : events) {
        for (const auto& aed : aeds) {
            if (distance(event.x, event.y, aed.x, aed.y) <= R) {
                covered++;
                break;
            }
        }
    }
    return covered;
}

// Leer instancia desde entrada
Instance read_instance() {
    Instance instance;
    std::cin >> instance.n >> instance.p >> instance.R;
    instance.events.resize(instance.n);
    for (int i = 0; i < instance.n; ++i) {
        int x, y, f;
        std::cin >> x >> y >> f;
        instance.events[i] = {x, y, f == 1};
    }
    return instance;
}

// Generar solución inicial con Greedy
void greedy_solution(Instance& instance) {
    while (instance.p >= 1.0) {
        int best_x = 0, best_y = 0, max_cover = 0;
        int best_event_index = -1;

        for (size_t i = 0; i < instance.events.size(); ++i) {
            const auto& event = instance.events[i];
            if (event.has_aed) continue;

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
                best_event_index = i;
            }
        }

        if (max_cover > 0 && best_event_index != -1) {
            instance.aeds.push_back({best_x, best_y, true});
            instance.p -= 1.0;
            instance.events[best_event_index].has_aed = true;
        } else {
            break;
        }
    }
}

// Función para aplicar Simulated Annealing
void simulated_annealing(Instance& instance, int max_iterations, double initial_temp, double final_temp) {
    auto start_time = std::chrono::high_resolution_clock::now();

    std::vector<AED> current_solution = instance.aeds;
    std::vector<AED> best_solution = current_solution;
    int current_covered = count_covered_events(instance.events, current_solution, instance.R);
    int best_covered = current_covered;

    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> event_dist(0, instance.events.size() - 1);
    std::uniform_real_distribution<double> prob_dist(0.0, 1.0);

    double temp = initial_temp;
    int iteration = 0;

    while (temp > final_temp && iteration < max_iterations) {
        iteration++;

        // Generar nueva solución por movimiento (reposicionar un AED)
        std::vector<AED> new_solution = current_solution;
        int random_aed_index = rng() % current_solution.size();
        int random_event_index = event_dist(rng);

        new_solution[random_aed_index] = {
            instance.events[random_event_index].x,
            instance.events[random_event_index].y,
            true
        };

        int new_covered = count_covered_events(instance.events, new_solution, instance.R);

        // Criterio de aceptación
        if (new_covered > current_covered || prob_dist(rng) < exp((new_covered - current_covered) / temp)) {
            current_solution = new_solution;
            current_covered = new_covered;

            // Actualizar mejor solución
            if (current_covered > best_covered) {
                best_solution = current_solution;
                best_covered = current_covered;
            }
        }

        // Enfriar la temperatura
        temp *= 0.95; // Factor de enfriamiento
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;

    // Guardar mejor solución en la instancia
    instance.aeds = best_solution;

    // Imprimir resultados
    std::cout << "Covered Events: " << best_covered << "\n";
    std::cout << "Percentage: " << (double)best_covered / instance.n * 100 << "%\n";
    std::cout << "Iterations: " << iteration << "\n";
    std::cout << "Execution Time: " << elapsed.count() << "s\n";

    // Guardar posiciones de AEDs en un archivo
    std::ofstream out("Posiciones.txt");
    for (const auto& aed : best_solution) {
        out << aed.x << " " << aed.y << "\n";
    }
    out.close();
}

int main(int argc, char* argv[]) {
    // Valores por defecto
    int max_iterations = 5000;
    double initial_temp = 1000.0;
    double final_temp = 1.0;

    // Leer argumentos opcionales
    if (argc >= 2) max_iterations = std::atoi(argv[1]);
    if (argc >= 3) initial_temp = std::atof(argv[2]);
    if (argc >= 4) final_temp = std::atof(argv[3]);

    // Leer instancia
    Instance instance = read_instance();

    // Generar solución inicial
    greedy_solution(instance);

    // Optimizar con Simulated Annealing
    simulated_annealing(instance, max_iterations, initial_temp, final_temp);

    return 0;
}
