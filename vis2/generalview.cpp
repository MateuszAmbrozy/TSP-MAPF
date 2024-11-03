#include "generalview.h"

GeneralView::GeneralView() : selectedAlgorithm(A_STAR) {}

void GeneralView::show()
{
    std::cout << "Choose Algorithm:\n";
    std::cout << "1. A* Algorithm\n";
    std::cout << "2. WHCA Algorithm\n";
    std::cout << "Enter the number of the algorithm you want to select (1 or 2): ";

    int choice;
    std::cin >> choice;
    selectAlgorithm(choice);
}

GeneralView::Algorithm GeneralView::getSelectedAlgorithm() const
{
    return selectedAlgorithm;
}

void GeneralView::printSelectedAlgorithm() const
{
    std::string algorithmName = (selectedAlgorithm == A_STAR) ? "A* Algorithm" : "WHCA Algorithm";
    std::cout << "Selected algorithm: " << algorithmName << "\n";
}

void GeneralView::selectAlgorithm(int choice)
{
    if (choice == 1) {
        selectedAlgorithm = A_STAR;
    } else if (choice == 2) {
        selectedAlgorithm = WHCA;
    }
}
