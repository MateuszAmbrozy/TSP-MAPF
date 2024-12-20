#include "lib/CA_Environment.h"
#include "lib/WHCA_Environment.h"
#include "lib/Agent.h"
#include "lib/Agent.h"
#include "lib/TaskGroup.h"
#include "lib/AStar/Astar.h" 
#include "lib/Space-Time-AStar/Reservation.h"
#include "lib/Space-Time-AStar/SpaceTimeAStar.h"
#include <algorithm>
#include <chrono>
#include "json/single_include/nlohmann/json.hpp"
#include "matplotlibcpp.h"
#include <fstream>

namespace plt = matplotlibcpp;
int width, height;
std::vector<map::Cell> obstacles;
std::vector<Agent> agents;
std::vector<std::pair<int, int>> pickupsData, dropoffData;

// Przykład funkcji losowego wyboru agentów
std::vector<Agent> getRandomAgents(std::vector<Agent>& agents, int num) {
    if (num > agents.size()) {
        std::cerr << "Liczba losowanych agentów przekracza dostępnych: " << agents.size() << std::endl;
        return {};
    }

    // Ziarno do losowania dla powtarzalności wyników (opcjonalnie można zmienić)
    std::random_device rd;
    std::mt19937 generator(rd());

    // Przemieszaj agentów
    std::shuffle(agents.begin(), agents.end(), generator);

    // Wybierz pierwsze `num` agentów
    return std::vector<Agent>(agents.begin(), agents.begin() + num);
}
void loadMap(std::string fileName){
    fileName = "D:\\All\\Studia\\Zajecia\\Semestr_7\\Inzynierka\\TEST\\TSP-MAPD\\vis2\\build\\Desktop_Qt_6_7_1_MinGW_64_bit-Debug\\test_graph_size\\" + fileName;
    //std::ifstream file("D:\\All\\Studia\\Zajecia\\Semestr_7\\Inzynierka\\TEST\\TSP-MAPD\\vis2\\build\\Desktop_Qt_6_7_1_MinGW_64_bit-Debug\\huge_map_with_pickuppoints_and_dropoffpoints.json", std::ifstream::in);
    std::ifstream file(fileName, std::ifstream::in);
    if (!file.is_open()) 
    {
        std::cerr << "Could not open file: " << fileName << std::endl;
        return;
    }

    // Wczytywanie danych z pliku do stringa
    std::string jsonData((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    // Parsowanie JSON
    nlohmann::json jsonDoc;
    try {
        jsonDoc = nlohmann::json::parse(jsonData);
    } catch (const std::exception &e) {
        std::cerr << "Invalid JSON format in file: " << fileName << "\nError: " << e.what() << std::endl;
        return;
    }

    // Pobieranie danych z JSON
    try {
        width = jsonDoc.at("width").get<int>();
        height = jsonDoc.at("height").get<int>();

        for (const auto& obstacleObj : jsonDoc.at("obstacles")) {
            int x = obstacleObj.at("x").get<int>();
            int y = obstacleObj.at("y").get<int>();
            obstacles.emplace_back(x, y, true);
        }

        auto agentsObj = jsonDoc.at("agents");
        for (int i=0; i<agentsObj.size(); i++) 
        {
            int x = agentsObj[i].at("x").get<int>();
            int y = agentsObj[i].at("y").get<int>();
            int capacity = agentsObj[i].at("capacity").get<int>();
            agents.emplace_back(i, capacity, map::Cell(x, y, false));
        }

        for (const auto& pickupObj : jsonDoc.at("avaliablePickups")) {
            int x = pickupObj.at("x").get<int>();
            int y = pickupObj.at("y").get<int>();
            pickupsData.push_back(std::make_pair(x, y));
        }

        for (const auto& dropoffObj : jsonDoc.at("avaliableDropoffs")) {
            int x = dropoffObj.at("x").get<int>();
            int y = dropoffObj.at("y").get<int>();
            dropoffData.push_back(std::make_pair(x, y));
        }
    } catch (const std::exception &e) {
        std::cerr << "Error processing JSON data: " << e.what() << std::endl;
        return;
    }

    std::cout << "Map data loaded from " << fileName << std::endl; 

    std::cout<<"-----AGENT------\n";   
    for(const auto& agent: agents )
    {
        std::cout<< "id: " << agent.getId() << ": (" << agent.getPosition().x << ", " << agent.getPosition().y << ") ";
    }
    std::cout<<std::endl << "-------PICKUPS-------\n";
    for(const auto& pickup : pickupsData)
    {
        std::cout<<"(" << pickup.first << ", " << pickup.second << ")  ";
    }
        std::cout<<std::endl << "-------DROPOOFS-------\n";
    for(const auto& dropoff : dropoffData)
    {
        std::cout<<"(" << dropoff.first << ", " << dropoff.second << ")  ";
    }
    
    std::cout<<std::endl << "-------OBSTACLES-------\n";
    for(const auto& obstacle : obstacles)
    {
        std::cout<<"(" << obstacle.x << ", " << obstacle.y << ")  ";
    }
    std::cout<<std::endl;
}

void testEnvi()
{
    const int seed = 42;  // Using a fixed seed value
    srand(time(0));

    std::vector<int> numValues;
    std::vector<int> results;
    std::vector<int> whca_results;

    std::vector<double> whca_times;
    std::vector<double> ca_times;


    // Loop over different values of num
    for (int num = 1; num <= 33; num++)
    
    {
        map::Graph graph(width, height, obstacles);
        CA_Environment e(agents, graph, pickupsData, dropoffData);
        WHCA_Environment whca_e(agents, graph, pickupsData, dropoffData);
        std::cout<<std::endl << num << std::endl;
        std::vector<TaskGroup> tasks;

        // Generate tasks
        for (int i = 1; i <= num * 3; ++i)
        {
            TaskGroup task = e.TASKGROUPGENERATOR();
            tasks.push_back(task);
        }

        // CA_Environment
        e.assignTasks(tasks);
        e.assignVacantAgents();
        auto start = std::chrono::high_resolution_clock::now();
        for (size_t timestep = 0; true; ++timestep)
        {
            e.runTimestep(timestep);
            if (e.allTasksCompleted())
            {
                results.push_back(timestep);
                std::cout << "All tasks completed in " << timestep << " timesteps.\n";
                break;
            }
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;
        double czasWSekundach = duration.count();
        std::cout << "CA Czas wykonania: " << czasWSekundach << " sekund" << std::endl;
        ca_times.push_back(czasWSekundach);

        // WHCA_Environment
        whca_e.assignTasks(tasks);
        whca_e.assignVacantAgents();
        start = std::chrono::high_resolution_clock::now();
        for (size_t timestep = 0; true; ++timestep)
        {
            whca_e.runTimestep(timestep);
            if (whca_e.allTasksCompleted())
            {
                whca_results.push_back(timestep);
                std::cout << "WHCA: All tasks completed in " << timestep << " timesteps.\n";
                break;
            }
        }

        end = std::chrono::high_resolution_clock::now();
        duration = end - start;
        czasWSekundach = duration.count();
        std::cout << "WHCA Czas wykonania: " << czasWSekundach << " sekund" << std::endl;
        whca_times.push_back(czasWSekundach);

        numValues.push_back(num*3);
    }

    plt::plot(numValues, results, "b-");  // Linia niebieska dla CA_Environment
    plt::plot(numValues, whca_results, "r-");  // Linia czerwona dla WHCA_Environment
    plt::xlabel("num (Liczba zadań)");
    plt::ylabel("result (Kroki czasowe)");
    plt::title("Kroki czasowe vs Liczba zadań");
    plt::show();
    plt::save("s1.png");

        // Nowa figura i drugi wykres dla czasów wykonania
    plt::figure();
    plt::plot(numValues, ca_times, "b-");  // Linia niebieska dla czasów CA_Environment
    plt::plot(numValues, whca_times, "r-");  // Linia czerwona dla czasów WHCA_Environment
    plt::xlabel("num (Liczba zadań)");
    plt::ylabel("Czas wykonania (sekundy)");
    plt::title("Czas wykonania vs Liczba zadań");
    plt::show();
    plt::save("s2.png");
}
void testEnvi_usrednione()
{
    const int seed = 42;  // Using a fixed seed value
    srand(time(NULL));

    const int iterations = 15;

    std::vector<int> numValues;  // Liczba zadań
    std::vector<std::vector<int>> all_ca_results;  // Wyniki CA dla każdej iteracji
    std::vector<std::vector<int>> all_whca_results;  // Wyniki WHCA dla każdej iteracji
    std::vector<std::vector<double>> all_ca_times;  // Czasy CA dla każdej iteracji
    std::vector<std::vector<double>> all_whca_times;  // Czasy WHCA dla każdej iteracji

    // Loop over different values of num
    for (int num = 1; num <= 33; num++) {
        std::cout<<"----------------------- " << num << " -----------------------\n";
        std::vector<int> ca_results(iterations, 0);  // Wyniki dla każdej iteracji
        std::vector<int> whca_results(iterations, 0);
        std::vector<double> ca_times(iterations, 0.0);  // Czasy dla każdej iteracji
        std::vector<double> whca_times(iterations, 0.0);

        for (int iter = 0; iter < iterations; ++iter) {
            map::Graph graph(width, height, obstacles);
            CA_Environment e(agents, graph, pickupsData, dropoffData);
            WHCA_Environment whca_e(agents, graph, pickupsData, dropoffData);

            std::vector<TaskGroup> tasks;

            // Generate tasks
            for (int i = 1; i <= num * 3; ++i) {
                TaskGroup task = e.TASKGROUPGENERATOR();
                tasks.push_back(task);
            }

            // CA_Environment
            e.assignTasks(tasks);
            e.assignVacantAgents();
            auto start = std::chrono::high_resolution_clock::now();
            for (size_t timestep = 0; true; ++timestep) {
                e.runTimestep(timestep);
                if (e.allTasksCompleted()) {
                    ca_results[iter] = timestep;
                    break;
                }
            }
            auto end = std::chrono::high_resolution_clock::now();
            ca_times[iter] = std::chrono::duration<double>(end - start).count();

            // WHCA_Environment
            whca_e.assignTasks(tasks);
            whca_e.assignVacantAgents();
            start = std::chrono::high_resolution_clock::now();
            for (size_t timestep = 0; true; ++timestep) {
                whca_e.runTimestep(timestep);
                if (whca_e.allTasksCompleted()) {
                    whca_results[iter] = timestep;
                    break;
                }
            }
            end = std::chrono::high_resolution_clock::now();
            whca_times[iter] = std::chrono::duration<double>(end - start).count();
        }

        // Zapisanie wyników dla tej liczby zadań
        all_ca_results.push_back(ca_results);
        all_whca_results.push_back(whca_results);
        all_ca_times.push_back(ca_times);
        all_whca_times.push_back(whca_times);
        numValues.push_back(num * 3);
    }

    // Obliczanie średnich wyników
    std::vector<double> avg_results;
    std::vector<double> avg_whca_results;
    std::vector<double> avg_ca_times;
    std::vector<double> avg_whca_times;

    for (size_t i = 0; i < numValues.size(); ++i) {
        double ca_result_avg = std::accumulate(all_ca_results[i].begin(), all_ca_results[i].end(), 0.0) / iterations;
        double whca_result_avg = std::accumulate(all_whca_results[i].begin(), all_whca_results[i].end(), 0.0) / iterations;
        double ca_time_avg = std::accumulate(all_ca_times[i].begin(), all_ca_times[i].end(), 0.0) / iterations;
        double whca_time_avg = std::accumulate(all_whca_times[i].begin(), all_whca_times[i].end(), 0.0) / iterations;

        avg_results.push_back(ca_result_avg);
        avg_whca_results.push_back(whca_result_avg);
        avg_ca_times.push_back(ca_time_avg);
        avg_whca_times.push_back(whca_time_avg);
    }
    // Wykres 1: Kroki czasowe vs liczba zadań
    plt::plot(numValues, avg_results, "b-");
    plt::plot(numValues, avg_whca_results, "r-");
    plt::xlabel("num (Liczba zadań)");
    plt::ylabel("Średnie kroki czasowe");
    plt::title("Kroki czasowe vs Liczba zadań (średnie z 15 iteracji)");

    plt::show();
    plt::save("s1_avg.png");

    // Wykres 2: Czas wykonania vs liczba zadań
    plt::figure();
    plt::plot(numValues, avg_ca_times, "b-");
    plt::plot(numValues, avg_whca_times, "r-");
    plt::xlabel("num (Liczba zadań)");
    plt::ylabel("Średni czas wykonania (sekundy)");
    plt::title("Czas wykonania vs Liczba zadań (średnie z 15 iteracji)");
    plt::show();
    plt::save("s2_avg.png");
}

void testAgents()
{
    const int seed = 42;  // Using a fixed seed value
    srand(seed);

    // Zdefiniowanie dostępnych pozycji dla pickup i dropoff

    std::vector<int> numAgents;
    std::vector<int> results;
    std::vector<int> whca_results;

    std::vector<double> whca_times;
    std::vector<double> ca_times;

    const int numTasks = 31;
    // Generowanie zadań
    map::Graph graph(width, height, obstacles);
    std::vector<TaskGroup> tasks;
    BaseEnvironment* base = new CA_Environment({}, graph, pickupsData, dropoffData);
    for (int i = 1; i <= numTasks; ++i)
    {
        TaskGroup task = base->TASKGROUPGENERATOR();
        tasks.push_back(task);
    }
    delete base;

    //Iteracja dla różnej liczby agentów
    for (int num = 1; num <= numTasks; ++num)
    {
        std::cout<<"------------------------------------   " << num << "------------------------------------\n"; 
        std::vector<Agent> currentAgents = getRandomAgents(agents, num);
        CA_Environment currentCA(currentAgents, graph, pickupsData, dropoffData);
        WHCA_Environment currentWHCA(currentAgents, graph, pickupsData, dropoffData);

        // CA_Environment
        currentCA.assignTasks(tasks);
        currentCA.assignVacantAgents();
        auto start = std::chrono::high_resolution_clock::now();
        for (size_t timestep = 0; true; ++timestep)
        {
            currentCA.runTimestep(timestep);
            if (currentCA.allTasksCompleted())
            {
                results.push_back(timestep);
                std::cout << "CA: All tasks completed in " << timestep << " timesteps.\n";
                break;
            }
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;
        double czasWSekundach = duration.count();
        std::cout << "CA Czas wykonania: " << czasWSekundach << " sekund" << std::endl;
        ca_times.push_back(czasWSekundach);

        // WHCA_Environment
        currentWHCA.assignTasks(tasks);
        currentWHCA.assignVacantAgents();
        start = std::chrono::high_resolution_clock::now();
        for (size_t timestep = 0; true; ++timestep)
        {
            currentWHCA.runTimestep(timestep);
            if (currentWHCA.allTasksCompleted())
            {
                whca_results.push_back(timestep);
                std::cout << "WHCA: All tasks completed in " << timestep << " timesteps.\n";
                break;
            }
        }
        end = std::chrono::high_resolution_clock::now();
        duration = end - start;
        czasWSekundach = duration.count();
        std::cout << "WHCA Czas wykonania: " << czasWSekundach << " sekund" << std::endl;
        whca_times.push_back(czasWSekundach);

        numAgents.push_back(num);
   }

    // Wykres Makespan
    plt::plot(numAgents, results, "b-");  // Linia niebieska
    plt::plot(numAgents, whca_results, "r-");  // Linia czerwona
    plt::xlabel("Liczba agentów");
    plt::ylabel("Kroki czasowe (Makespan)");
    plt::title("Makespan vs Liczba agentów");
    plt::show();

    // Wykres czasów wykonania
    plt::figure();
    plt::plot(numAgents, ca_times, "b-");  // Linia niebieska
    plt::plot(numAgents, whca_times, "r-");  // Linia czerwona
    plt::xlabel("Liczba agentów");
    plt::ylabel("Czas wykonania (sekundy)");
    plt::title("Czas wykonania vs Liczba agentów");
    plt::show();
}
void testAgents_multi_times()
{
    map::Graph graph(15, 10);

    // Tablica pozycji na obwodzie mapy 15x10 (x, y)
    std::vector<std::pair<int, int>> agentPositions;

    // Dodanie pozycji na górnej i dolnej krawędzi
    for (int x = 0; x < 15; ++x)
    {
        agentPositions.emplace_back(x, 0);  // Górna krawędź
        agentPositions.emplace_back(x, 9);  // Dolna krawędź
    }

    // Dodanie pozycji na lewej i prawej krawędzi (bez rogów, bo już dodane)
    for (int y = 1; y < 9; ++y)
    {
        agentPositions.emplace_back(0, y);  // Lewa krawędź
        agentPositions.emplace_back(14, y);  // Prawa krawędź
    }

    // Przygotowanie agentów w oparciu o tablicę pozycji
    std::vector<Agent> agentsLocal;
    for (size_t i = 0; i < agentPositions.size(); ++i)
    {
        const auto& [x, y] = agentPositions[i];
        agentsLocal.emplace_back(i, 15, map::Cell(x, y));
    }

    // Zdefiniowanie dostępnych pozycji dla pickup i dropoff
    std::vector<int> availablePickupX = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
    std::vector<int> availablePickupY = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::vector<int> availableDropofX = {3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
    std::vector<int> availableDropofY = {1, 8};

    std::vector<int> numAgents;
    std::vector<int> results;
    std::vector<int> whca_results;

    std::vector<double> whca_times;
    std::vector<double> ca_times;

    const int numTasks = 20;
    // Generowanie zadań
    std::vector<TaskGroup> tasks;
    BaseEnvironment* base = new CA_Environment({}, graph, pickupsData, dropoffData);
    for (int i = 0; i < numTasks; ++i)
    {
        TaskGroup task = base->TASKGROUPGENERATOR();
        tasks.push_back(task);
    }
    delete base;

    // Iteracja dla różnej liczby agentów
    for (int num = 1; num <= 20; ++num)
    {
        std::vector<Agent> currentAgents(agentsLocal.begin(), agentsLocal.begin() + num);
        CA_Environment currentCA(currentAgents, graph, pickupsData, dropoffData);
        WHCA_Environment currentWHCA(currentAgents, graph, pickupsData, dropoffData);

        // CA_Environment
        currentCA.assignTasks(tasks);
        currentCA.assignVacantAgents();
        auto start = std::chrono::high_resolution_clock::now();
        for (size_t timestep = 0; true; ++timestep)
        {
            currentCA.runTimestep(timestep);
            if (currentCA.allTasksCompleted())
            {
                results.push_back(timestep);
                std::cout << "CA: All tasks completed in " << timestep << " timesteps.\n";
                break;
            }
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;
        double czasWSekundach = duration.count();
        std::cout << "CA Czas wykonania: " << czasWSekundach << " sekund" << std::endl;
        ca_times.push_back(czasWSekundach);

        // WHCA_Environment
        currentWHCA.assignTasks(tasks);
        currentWHCA.assignVacantAgents();
        start = std::chrono::high_resolution_clock::now();
        for (size_t timestep = 0; true; ++timestep)
        {
            currentWHCA.runTimestep(timestep);
            if (currentWHCA.allTasksCompleted())
            {
                whca_results.push_back(timestep);
                std::cout << "WHCA: All tasks completed in " << timestep << " timesteps.\n";
                break;
            }
        }
        end = std::chrono::high_resolution_clock::now();
        duration = end - start;
        czasWSekundach = duration.count();
        std::cout << "WHCA Czas wykonania: " << czasWSekundach << " sekund" << std::endl;
        whca_times.push_back(czasWSekundach);

        numAgents.push_back(num);
    }

    // Wykres Makespan
    plt::plot(numAgents, results, "b-");  // Linia niebieska
    plt::plot(numAgents, whca_results, "r-");  // Linia czerwona
    plt::xlabel("Liczba agentów");
    plt::ylabel("Kroki czasowe (Makespan)");
    plt::title("Makespan vs Liczba agentów");
    plt::show();

    // Wykres czasów wykonania
    plt::figure();
    plt::plot(numAgents, ca_times, "b-");  // Linia niebieska
    plt::plot(numAgents, whca_times, "r-");  // Linia czerwona
    plt::xlabel("Liczba agentów");
    plt::ylabel("Czas wykonania (sekundy)");
    plt::title("Czas wykonania vs Liczba agentów");
    plt::show();
}

void testMaps()
{
    srand(42);
    // Lista map do przetestowania
    std::vector<std::string> maps = {"10x10.json", "15x15.json", "20x20.json", "25x25.json", "30x30.json"};
    int numTasks = 40; // Stała liczba zadań

    // Wyniki dla różnych map
    std::vector<int> makespan_CA, makespan_WHCA;
    std::vector<double> time_CA, time_WHCA;
    std::vector<std::string> mapNames; // Nazwy map do etykiet

    for (const auto& mapFile : maps)
    {
        // Wczytaj mapę
        agents.clear();
        pickupsData.clear();
        dropoffData.clear();
        obstacles.clear();
        std::cout << "Loading map: " << mapFile << std::endl;
        loadMap(mapFile);

        // Stwórz graf na podstawie wczytanej mapy
        map::Graph graph(width, height, obstacles);
        BaseEnvironment* base = new CA_Environment({}, graph, pickupsData, dropoffData);
                // Przygotowanie zadań
        std::vector<TaskGroup> tasks;
        for (int i = 1; i <= numTasks; ++i)
        {
            TaskGroup task = base->TASKGROUPGENERATOR();
            tasks.push_back(task);
        }
        delete base;



        // Test CA_Environment
        CA_Environment e(agents, graph, pickupsData, dropoffData);
        e.assignTasks(tasks);
        e.assignVacantAgents();
        auto start = std::chrono::high_resolution_clock::now();
        for (size_t timestep = 0; true; ++timestep)
        {
            e.runTimestep(timestep);
            if (e.allTasksCompleted())
            {
                makespan_CA.push_back(timestep);
                std::cout << "CA: All tasks completed in " << timestep << " timesteps.\n";
                break;
            }
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;
        time_CA.push_back(duration.count());
        std::cout << "CA Execution time: " << duration.count() << " seconds.\n";

        // Test WHCA_Environment
        WHCA_Environment whca_e(agents, graph, pickupsData, dropoffData);
        whca_e.assignTasks(tasks);
        whca_e.assignVacantAgents();
        start = std::chrono::high_resolution_clock::now();
        for (size_t timestep = 0; true; ++timestep)
        {
            whca_e.runTimestep(timestep);
            if (whca_e.allTasksCompleted())
            {
                makespan_WHCA.push_back(timestep);
                std::cout << "WHCA: All tasks completed in " << timestep << " timesteps.\n";
                break;
            }
        }
        end = std::chrono::high_resolution_clock::now();
        duration = end - start;
        time_WHCA.push_back(duration.count());
        std::cout << "WHCA Execution time: " << duration.count() << " seconds.\n";

        // Dodaj nazwę mapy do wyników
        mapNames.push_back(mapFile);
    }

    // Wizualizacja wyników
    std::vector<int> mapSizes = {10, 15, 20, 25, 30}; // Rozmiary map odpowiadające nazwom

    // Wykres 1: Wielkość mapy vs Makespan
    plt::figure();
    plt::plot(mapSizes, makespan_CA, "b-");
    plt::plot(mapSizes, makespan_WHCA, "r-");
    plt::xlabel("Rozmiar mapy (bok kwadratu)");
    plt::ylabel("Makespan (kroki czasowe)");
    plt::title("Liczba kroków vs Rozmiar mapy");
    plt::show();
    plt::save("makespan_vs_map_size.png");

    // Wykres 2: Wielkość mapy vs Czas wykonania
    plt::figure();
    plt::plot(mapSizes, time_CA, "b-");
    plt::plot(mapSizes, time_WHCA, "r-");
    plt::xlabel("Rozmiar mapy (bok kwadratu)");
    plt::ylabel("Czas wykonania (sekundy)");
    plt::title("Czas wykonania vs Rozmiar mapy");
    plt::show();
    plt::save("time_vs_map_size.png");
}

int main() 
{
   // testEnvi();
    //loadMap("D:\\All\\Studia\\Zajecia\\Semestr_7\\Inzynierka\\TEST\\TSP-MAPD\\vis2\\build\\Desktop_Qt_6_7_1_MinGW_64_bit-Debug\\map2.json");
    //testEnvi();
    //testEnvi_usrednione();
    //testAgents();
    testMaps();
    return 0;
}
