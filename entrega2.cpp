#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <random>
#include <chrono>
#include <algorithm>

/**
 * @struct AED
 * Representa un desfibrilador externo automático (AED).
 * @param x Coordenada x del AED.
 * @param y Coordenada y del AED.
 * @param is_new Indica si el AED fue recién instalado (true) o si ya estaba en el sistema (false).
 */
struct AED {
    int x, y;
    bool is_new;
};

/**
 * @struct Event
 * Representa un evento de paro cardíaco súbito (OHCA).
 * @param x Coordenada x del evento.
 * @param y Coordenada y del evento.
 * @param has_aed Indica si el evento ya tiene un AED cercano (true) o no (false).
 */
struct Event {
    int x, y;
    bool has_aed;
};

/**
 * @struct Instance
 * Contiene los datos principales de una instancia del problema.
 * @param n Número de eventos OHCA.
 * @param p Presupuesto disponible para instalar nuevos AEDs.
 * @param R Radio de cobertura de un AED.
 * @param events Vector que almacena todos los eventos.
 * @param aeds Vector que almacena todos los AEDs instalados.
 */
struct Instance {
    int n;
    float p;
    int R;
    std::vector<Event> events;
    std::vector<AED> aeds;
};

/**
 * Calcula la distancia euclidiana entre dos puntos.
 * @param x1 Coordenada x del primer punto.
 * @param y1 Coordenada y del primer punto.
 * @param x2 Coordenada x del segundo punto.
 * @param y2 Coordenada y del segundo punto.
 * @return Distancia euclidiana entre los dos puntos.
 */
double distance(int x1, int y1, int x2, int y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

/**
 * Cuenta la cantidad de eventos que están cubiertos por al menos un AED.
 * @param events Vector de eventos OHCA.
 * @param aeds Vector de AEDs instalados.
 * @param R Radio de cobertura de los AEDs.
 * @return Número de eventos cubiertos.
 */
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

/**
 * Lee los datos de una instancia desde la entrada estándar.
 * @return Una instancia con los datos leídos.
 */
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

/**
 * Genera una solución inicial utilizando un enfoque greedy.
 * Se instalan los AEDs en los eventos que cubran la mayor cantidad de otros eventos
 * dentro del radio de cobertura, respetando el presupuesto.
 * @param instance Referencia a la instancia del problema.
 */
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

/**
 * Aplica el algoritmo de Simulated Annealing para optimizar la distribución de los AEDs.
 * @param instance Referencia a la instancia del problema.
 * @param max_iterations Número máximo de iteraciones permitidas.
 * @param initial_temp Temperatura inicial del algoritmo.
 * @param final_temp Temperatura final mínima para detener el algoritmo.
 * @param cooling_factor Factor de enfriamiento para reducir la temperatura.
 */
void simulated_annealing(Instance& instance, int max_iterations, double initial_temp, double final_temp, double cooling_factor) {
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
        temp *= cooling_factor; // Factor de enfriamiento
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

/**
 * Punto de entrada principal del programa.
 * Permite configurar parámetros opcionales para el algoritmo desde la línea de comandos.
 * @param argc Número de argumentos pasados al programa.
 * @param argv Arreglo de argumentos (máximo 4: iteraciones, temperatura inicial, final y factor de enfriamiento).
 * @return Código de salida del programa (0 si todo salió correctamente).
 */
int main(int argc, char* argv[]) {
    // Valores por defecto
    int max_iterations = 5000;
    double initial_temp = 1000.0;
    double final_temp = 1.0;
    double cooling_factor = 0.95;

    // Leer argumentos opcionales
    if (argc >= 5) max_iterations = std::atoi(argv[1]);
    if (argc >= 5) initial_temp = std::atof(argv[2]);
    if (argc >= 5) final_temp = std::atof(argv[3]);
    if (argc >= 5) cooling_factor = std::atof(argv[4]);

    // Leer instancia
    Instance instance = read_instance();

    // Generar solución inicial
    greedy_solution(instance);

    // Optimizar con Simulated Annealing
    simulated_annealing(instance, max_iterations, initial_temp, final_temp, cooling_factor);

    return 0;
}
