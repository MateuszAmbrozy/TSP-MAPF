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

    // // Sprawdza, czy krawędź z (x1, y1) do (x2, y2) w czasie t powoduje kolizję krawędziową (impas na krawędzi)
    // bool wouldCauseEdgeCollision(int x1, int y1, int t1, int x2, int y2, int t2) const {
    //     if (edgeReservations.count({x2, y2}) && edgeReservations.at({x2, y2}).count(t1)) {
    //         auto reservedMove = edgeReservations.at({x2, y2}).at(t1);
    //         if (reservedMove.first == x1 && reservedMove.second == y1) {
    //             return true;
    //         }
    //     }
    //     return false;
    // }
    bool wouldCauseEdgeCollision(int x1, int y1, int t1, int x2, int y2, int t2) const {
        // Klucz do rezerwacji dla (x2, y2) - sprawdzamy rezerwacje w tym punkcie w czasie t1
        auto reverseEdgeKey = std::make_pair(x2, y2);

        // Sprawdzenie kolizji dla odwrotnego ruchu (z (x2, y2) do (x1, y1)) w czasie t1
        if (edgeReservations.count(reverseEdgeKey) && edgeReservations.at(reverseEdgeKey).count(t1)) {
            auto reservedMove = edgeReservations.at(reverseEdgeKey).at(t1);
            // Sprawdzamy, czy rezerwowany ruch prowadzi z powrotem do (x1, y1)
            if (reservedMove.first == x1 && reservedMove.second == y1) {
                return true;  // Istnieje kolizja na początku ruchu
            }
        }

        // // Dodatkowo sprawdzamy, czy istnieje kolizja w czasie t2 (koniec ruchu)
        // if (edgeReservations.count(reverseEdgeKey) && edgeReservations.at(reverseEdgeKey).count(t2)) {
        //     auto reservedMove = edgeReservations.at(reverseEdgeKey).at(t2);
        //     // Sprawdzamy, czy na końcu ruchu ktoś próbuje poruszać się z (x2, y2) do (x1, y1)
        //     if (reservedMove.first == x1 && reservedMove.second == y1) {
        //         return true;  // Istnieje kolizja na końcu ruchu
        //     }
        // }

        // Jeśli żadna kolizja nie została wykryta, zwracamy false
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
    void removeEdgeReservation(int x1, int y1, int t) {
        // Klucz do pierwszej mapy to (x1, y1)
        auto edgeKey = std::make_pair(x1, y1);

        // Sprawdzamy, czy istnieje wpis w mapie o kluczu (x1, y1)
        if (edgeReservations.count(edgeKey)) {
            // Sprawdzamy, czy istnieje rezerwacja dla czasu t
            if (edgeReservations[edgeKey].count(t)) {
                // Usuwamy rezerwację dla czasu t
                edgeReservations[edgeKey].erase(t);

                // Jeśli mapa czasu dla (x1, y1) jest pusta, usuwamy całe wejście
                if (edgeReservations[edgeKey].empty()) {
                    edgeReservations.erase(edgeKey);
                }
            }
        }
    }

};
