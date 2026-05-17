#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <limits>
#include <algorithm>

using namespace std;

// Estructura para representar la conexión (Arista/Edge del grafo)
struct Route {
    string destination;
    string transport;
    double distanceKM;
    int estimatedMinutes;
};

// Clase principal que maneja toda la lógica del Grafo
class MobilityAnalyzer {
private:
    // HASH INDEXING + GRAPH STRUCTURE:
    // Usamos unordered_map (Hash Table) para indexar las ubicaciones en O(1)
    // El valor es un vector de rutas (Lista de adyacencia del Grafo)
    unordered_map<string, vector<Route>> graph;
    
    // Set auxiliar para guardar todas las ubicaciones únicas de forma rápida
    unordered_set<string> locations;

public:
    // 1. Cargar el dataset desde un archivo CSV
    void loadDataset(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Error: No se pudo abrir el archivo " << filename << endl;
            return;
        }

        string line;
        int count = 0;
        while (getline(file, line)) {
            if (line.empty()) continue;

            stringstream ss(line);
            string origin, dest, transport, distStr, minStr;

            // Separamos por comas
            getline(ss, origin, ',');
            getline(ss, dest, ',');
            getline(ss, transport, ',');
            getline(ss, distStr, ',');
            getline(ss, minStr, ',');

            double dist = stod(distStr);
            int mins = stoi(minStr);

            // Añadimos al grafo (Grafo no dirigido, conectamos ida y vuelta)
            graph[origin].push_back({dest, transport, dist, mins});
            graph[dest].push_back({origin, transport, dist, mins});

            // Guardamos las locaciones únicas
            locations.insert(origin);
            locations.insert(dest);
            count++;
        }
        file.close();
        cout << "Exito: Se cargaron " << count << " conexiones correctamente.\n";
    }

    // 2. Mostrar ubicaciones
    void showLocations() {
        cout << "\n--- Ubicaciones Registradas (" << locations.size() << ") ---\n";
        for (const string& loc : locations) {
            cout << "- " << loc << "\n";
        }
    }

    // 3. Buscar ubicación en O(1) gracias al Hash map
    void searchLocation(const string& loc) {
        if (graph.find(loc) != graph.end()) {
            cout << "La ubicacion '" << loc << "' SI existe en el sistema y tiene " 
                 << graph[loc].size() << " conexiones directas.\n";
        } else {
            cout << "La ubicacion '" << loc << "' NO fue encontrada.\n";
        }
    }

    // 4. Mostrar conexiones de un punto específico
    void showConnections(const string& loc) {
        if (graph.find(loc) == graph.end()) {
            cout << "Ubicacion no encontrada.\n";
            return;
        }
        cout << "\n--- Conexiones desde " << loc << " ---\n";
        for (const Route& r : graph[loc]) {
            cout << "-> " << r.destination << " | Medio: " << r.transport 
                 << " | " << r.distanceKM << " km | " << r.estimatedMinutes << " min\n";
        }
    }

    // 5. EXPLORATION TRAVERSAL: Dijkstra's Algorithm para la ruta más rápida (en minutos)
    void exploreRoute(const string& start, const string& end) {
        if (graph.find(start) == graph.end() || graph.find(end) == graph.end()) {
            cout << "El origen o destino no existen en el sistema.\n";
            return;
        }

        // Tablas hash para distancias mínimas y rastreo de la ruta
        unordered_map<string, int> minTime;
        unordered_map<string, string> previous;
        
        for (const string& loc : locations) {
            minTime[loc] = numeric_limits<int>::max(); // Infinito
        }
        minTime[start] = 0;

        // Cola de prioridad: par(minutos_acumulados, nodo_actual)
        priority_queue<pair<int, string>, vector<pair<int, string>>, greater<pair<int, string>>> pq;
        pq.push({0, start});

        while (!pq.empty()) {
            int currentTime = pq.top().first;
            string currentLoc = pq.top().second;
            pq.pop();

            if (currentLoc == end) break; // Ya encontramos el camino más corto al destino

            // Explorar vecinos
            for (const Route& neighbor : graph[currentLoc]) {
                int newTime = currentTime + neighbor.estimatedMinutes;
                
                if (newTime < minTime[neighbor.destination]) {
                    minTime[neighbor.destination] = newTime;
                    previous[neighbor.destination] = currentLoc;
                    pq.push({newTime, neighbor.destination});
                }
            }
        }

        // Reconstruir la ruta
        if (minTime[end] == numeric_limits<int>::max()) {
            cout << "No hay ruta disponible entre " << start << " y " << end << ".\n";
            return;
        }

        vector<string> path;
        for (string at = end; at != ""; at = previous[at]) {
            path.push_back(at);
        }
        reverse(path.begin(), path.end());

        cout << "\n--- Ruta más rápida encontrada ---\n";
        cout << "Tiempo total estimado: " << minTime[end] << " minutos.\n";
        cout << "Camino: ";
        for (size_t i = 0; i < path.size(); i++) {
            cout << path[i] << (i == path.size() - 1 ? "" : " -> ");
        }
        cout << "\n";
    }

    // 6. Generar estructura en formato DOT para Graphviz (MANDATORIO)
    void generateStructure() {
        cout << "\n--- Copia el siguiente texto en https://dreampuf.github.io/GraphvizOnline/ ---\n\n";
        cout << "graph G {\n";
        
        // Usamos un set para evitar imprimir conexiones duplicadas (A--B y B--A)
        unordered_set<string> printedEdges;

        for (auto const& pair : graph) {
            string origin = pair.first;
            for (const Route& r : pair.second) {
                // Creamos un identificador único para la arista sin importar el orden
                string edgeId1 = origin + "-" + r.destination;
                string edgeId2 = r.destination + "-" + origin;

                if (printedEdges.find(edgeId1) == printedEdges.end() && 
                    printedEdges.find(edgeId2) == printedEdges.end()) {
                    
                    cout << "  \"" << origin << "\" -- \"" << r.destination 
                         << "\" [label=\"" << r.transport << "\"];\n";
                    
                    printedEdges.insert(edgeId1);
                }
            }
        }
        cout << "}\n\n";
    }
};

int main() {
    MobilityAnalyzer analyzer;
    int option;
    string input1, input2;

    do {
        cout << "\n=== SMART CAMPUS MOBILITY ANALYZER ===\n";
        cout << "1. Load dataset\n";
        cout << "2. Show locations\n";
        cout << "3. Search location\n";
        cout << "4. Show connections\n";
        cout << "5. Explore routes / traversal (Shortest Path)\n";
        cout << "6. Generate structure\n";
        cout << "7. Exit\n";
        cout << "Select an option: ";
        cin >> option;

        switch (option) {
            case 1:
                analyzer.loadDataset("dataset.txt");
                break;
            case 2:
                analyzer.showLocations();
                break;
            case 3:
                cout << "Ingrese el nombre de la ubicacion (ej. EAFIT): ";
                cin >> input1;
                analyzer.searchLocation(input1);
                break;
            case 4:
                cout << "Ingrese el nombre de la ubicacion: ";
                cin >> input1;
                analyzer.showConnections(input1);
                break;
            case 5:
                cout << "Desde donde sale? (Origen): ";
                cin >> input1;
                cout << "A donde va? (Destino): ";
                cin >> input2;
                analyzer.exploreRoute(input1, input2);
                break;
            case 6:
                analyzer.generateStructure();
                break;
            case 7:
                cout << "Saliendo del sistema...\n";
                break;
            default:
                cout << "Opcion invalida.\n";
        }
    } while (option != 7);

    return 0;
}