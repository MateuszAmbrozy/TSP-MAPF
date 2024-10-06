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
    std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, bool>>> reservations;
    
    // Rezerwacje krawędzi: klucz to ((x1, y1), t), wartość to (x2, y2) 
    std::unordered_map<std::pair<int, int>, std::unordered_map<int, std::pair<int, int>>, PairHash> edgeReservations;

public:
    Reservation() = default;

    bool isReserved(int x, int y, int t) const {
        if (reservations.count(x) && reservations.at(x).count(y) && reservations.at(x).at(y).count(t)) {
            return reservations.at(x).at(y).at(t);
        }
        return false;
    }

    void reserve(int x, int y, int t) {
        reservations[x][y][t] = true;
    }

    void reserveEdge(int x1, int y1, int x2, int y2, int t) {
        edgeReservations[{x1, y1}][t] = {x2, y2};
    }

    // Sprawdza, czy krawędź z (x1, y1) do (x2, y2) w czasie t powoduje kolizję krawędziową (impas na krawędzi)
    bool wouldCauseEdgeCollision(int x1, int y1, int t1, int x2, int y2, int t2) const {
        if (edgeReservations.count({x2, y2}) && edgeReservations.at({x2, y2}).count(t1)) {
            auto reservedMove = edgeReservations.at({x2, y2}).at(t1);
            if (reservedMove.first == x1 && reservedMove.second == y1) {
                return true;
            }
        }
        return false;
    }

    bool wouldCauseCollision(int x1, int y1, int t1, int x2, int y2, int t2) const {
        return isReserved(x2, y2, t1) && isReserved(x1, x1, t2);
    }

    void removeReservation(int x, int y, int t) {
        if (reservations.count(x) && reservations[x].count(y) && reservations[x][y].count(t)) {
            reservations[x][y].erase(t); 
            if (reservations[x][y].empty()) {
                reservations[x].erase(y);
            }
            if (reservations[x].empty()) {
                reservations.erase(x);
            }
        }
    }
    void removeEdgeReservation(int x1, int y1, int x2, int y2, int t) {
        if (edgeReservations.count({x1, y1}) && edgeReservations[{x1, y1}].count(t)) {
            edgeReservations[{x1, y1}].erase(t);
            if (edgeReservations[{x1, y1}].empty()) {
                edgeReservations.erase({x1, y1});
            }
        }
    }

};
