#include <iostream>
#include <map>
#include <string>

#include "set1/S01C01.h"
#include "set1/S01C02.h"
#include "set1/S01C03.h"
#include "set1/S01C04.h"
#include "set1/S01C05.h"
#include "set1/S01C06.h"
#include "set1/S01C07.h"
#include "set1/S01C08.h"
#include "set2/S02C01.h"
#include "set2/S02C02.h"

// Registro dei challenge: per aggiungerne uno basta includere l'header e
// aggiungere una riga qui.
static const std::map<std::string, void (*)()> challenges = {
    {"s01c01", s01c01}, {"s01c02", s01c02}, {"s01c03", s01c03},
    {"s01c04", s01c04}, {"s01c05", s01c05}, {"s01c06", s01c06},
    {"s01c07", s01c07}, {"s01c08", s01c08},
    {"s02c01", s02c01}, {"s02c02", s02c02}, //{"s02c03",s02c03}
};

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <sNNcMM>\nAvailable:";
        for (const auto& [name, fn] : challenges) std::cerr << ' ' << name;
        std::cerr << '\n';
        return 1;
    }

    auto it = challenges.find(argv[1]);
    if (it == challenges.end()) {
        std::cerr << "Challenge not defined: " << argv[1] << '\n';
        return 1;
    }

	// Chiama la funzione
    it->second();
    return 0;
}
