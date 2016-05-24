#include "utilities.hpp"

int main() {
    std::string types[3] = {"Watches", "Shoes", "NeckTies"};
    for (int i = 0; i < 3; i++) {
        normalize(types[i]);
    }
    return 0;
}
