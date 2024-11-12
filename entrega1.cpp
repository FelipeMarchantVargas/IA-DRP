#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>

struct AED {
    int x, y;
    bool is_new;
    int original_x, original_y;
};

struct Event {
    int x, y;
    bool has_aed;

};

struct Instance {
    int n; // num de eventos OHCA
    float p; // presupuesto
    int R; // radio de cobertura
    std::vector<Event> events; // Vector para los eventos
    std::vector<AED> aeds; // vector para los AEDs
};

// Funcion para calcular la distancia entre un AED y un evento
double distance(int x1, int y1, int x2, int y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

// Funcion de evaluacion: cuenta eventos cubiertos por los AEDs
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

// Lectura de datos desde un archivo
Instance read_instance() {
    Instance instance;
    std::cin >> instance.n >> instance.p >> instance.R;
    instance.events.resize(instance.n);

    for (int i = 0; i < instance.n; ++i) {
        int x, y, f;
        std::cin >> x >> y >> f;
        instance.events[i] = {x, y, f==1};
        if (f==1) {
            instance.aeds.push_back({x, y, false});
        }
    }

    return instance;
}

// Generacion de la solucion inicial con Greedy
void greedy_solution(Instance& instance) {
    while (instance.p >= 1.0) {
        int best_x = 0, best_y = 0;
        int max_cover = 0;
	int best_event_index = -1;
        
        for (int i = 0; i < instance.events.size(); ++i) {
	    const auto& event = instance.events[i];
	    if(event.has_aed) continue;

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
	
	if(max_cover > 0 && best_event_index != -1){
            instance.aeds.push_back({best_x, best_y, true});
            instance.p -= 1.0;
	    instance.events[best_event_index].has_aed = true;
	} else {
	    break;
	}
    }
}

// Salida de datos
void print_results(const Instance& instance, int covered_events) {
    double coverage_percent = (double)covered_events / instance.n * 100;
    std::cout << "Events: " << covered_events << "\n";
    std::cout << "Percentage: " << coverage_percent << "%\n";
    std::cout << "Budget: " << instance.p << "\n";
    for (size_t i = 0; i < instance.aeds.size(); ++i) {
        std::cout << i + 1 << "- (" << instance.aeds[i].x << ", " << instance.aeds[i].y << ") ";
        std::cout << (instance.aeds[i].is_new ? "N" : "R") << "\n";
    }
}

int main() {
    // Lectura de instancia desde archivo
    Instance instance = read_instance();

    // Generacion de la solucion inicial
    greedy_solution(instance);

    // Evaluacion de la cobertura
    int covered_events = count_covered_events(instance.events, instance.aeds, instance.R);

    // Impresion de resultados
    print_results(instance, covered_events);

    return 0;
}
