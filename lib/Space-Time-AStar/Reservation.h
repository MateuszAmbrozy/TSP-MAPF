#pragma once
#include <unordered_map>
#include <utility>

struct PairHash {
    std::size_t operator()(const std::pair<int, int>& p) const {
        return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
    }
};
class Reservation 
{
public:
    // Rezerwacje komórek: klucz to (x, y, t)
    std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, bool>>> reservations;
    
    // Rezerwacje krawędzi: klucz to ((x1, y1), t), wartość to (x2, y2) 
    std::unordered_map<std::pair<int, int>, std::unordered_map<int, std::pair<int, int>>, PairHash> edgeReservations;

public:
    Reservation() = default;

    // Sprawdza, czy komórka (x, y) jest zarezerwowana w czasie t
    bool isReserved(int x, int y, int t) const {
        if (reservations.count(x) && reservations.at(x).count(y) && reservations.at(x).at(y).count(t)) {
            return reservations.at(x).at(y).at(t);
        }
        return false;
    }

    // Rezerwuje komórkę (x, y) w czasie t
    void reserve(int x, int y, int t) {
        reservations[x][y][t] = true;
    }

    // Rezerwuje krawędź z (x1, y1) do (x2, y2) w czasie t
    void reserveEdge(int x1, int y1, int x2, int y2, int t) {
        edgeReservations[{x1, y1}][t] = {x2, y2};
    }

    // Sprawdza, czy krawędź z (x1, y1) do (x2, y2) w czasie t powoduje kolizję krawędziową (impas na krawędzi)
    bool wouldCauseEdgeCollision(int x1, int y1, int t1, int x2, int y2, int t2) const {
        if (edgeReservations.count({x2, y2}) && edgeReservations.at({x2, y2}).count(t1)) {
            auto reservedMove = edgeReservations.at({x2, y2}).at(t1);
            if (reservedMove.first == x1 && reservedMove.second == y1) {
                return true;  // Konflikt krawędziowy wykryty (dwaj agenci próbują zamienić się miejscami)
            }
        }
        return false;
    }

    // Sprawdza, czy kolizja wystąpi w przypadku rezerwacji krawędzi
    bool wouldCauseCollision(int x1, int y1, int t1, int x2, int y2, int t2) const {
        return isReserved(x2, y2, t1) && isReserved(x1, x1, t2);
    }

    void removeReservation(int x, int y, int t) {
        if (reservations.count(x) && reservations[x].count(y) && reservations[x][y].count(t)) {
            reservations[x][y].erase(t);  // Remove the reservation at time t
            if (reservations[x][y].empty()) {
                reservations[x].erase(y);  // Clean up if the map for (x, y) is now empty
            }
            if (reservations[x].empty()) {
                reservations.erase(x);  // Clean up if the map for x is now empty
            }
        }
    }
    void removeEdgeReservation(int x1, int y1, int x2, int y2, int t) {
        if (edgeReservations.count({x1, y1}) && edgeReservations[{x1, y1}].count(t)) {
            edgeReservations[{x1, y1}].erase(t);  // Remove the reservation at time t
            if (edgeReservations[{x1, y1}].empty()) {
                edgeReservations.erase({x1, y1});  // Clean up if no more edges for (x1, y1)
            }
        }
    }

};
