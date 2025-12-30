#include <iostream>
#include <vector>
// #include <memory>

#include "Utils.h"

#include "StringConverter.h"

// Include condizionali
#if SET == 1
    #include "set1/S01C01.h"
    #include "set1/S01C02.h"
    #include "set1/S01C03.h"
    #include "set1/S01C04.h"
    #include "set1/S01C05.h"
#elif SET == 2
    #include "set2/c01.hpp"
#endif

#if SET==1
int main() {
    #if CHALLENGE == 1
        s01c01();
        //challenge::set1::c01();
    #elif CHALLENGE == 2
        s01c02();
    #elif CHALLENGE == 3
        s01c03();
    #elif CHALLENGE == 4
        s01c04();
    #elif CHALLENGE == 5
        s01c05();
    #else
        std::cerr << "Challenge not defined\n";
        return 1;
    #endif
    
    return 0;
}
#endif


#if SET==2
int main() {
    #if SET == 1 && CHALLENGE == 1
        s01c01();
        //challenge::set1::c01();
    #elif SET == 1 && CHALLENGE == 2
        challenge::set1::c02();
    #elif SET == 2 && CHALLENGE == 1
        challenge::set2::c01();
    #else
        std::cerr << "Challenge not defined\n";
        return 1;
    #endif
    
    return 0;
}
#endif
