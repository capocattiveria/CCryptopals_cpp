#ifndef UTILS_H
#define UTILS_H

#include <map>
#include <string>
#include <memory>
#include <vector>

/* CCrypto *********************************************************************************/
namespace CCrypto {

        
    /* CCrypto::Utils **************************************************************************/
    namespace Utils{

        // From the string passed, compute the frequency map
        std::vector<double> getFrequencyCharInVector(const std::string& fileName);
        std::string loadTextFromFile(const std::string& fileName);

        double englishScore(const std::string& text, const std::vector<double>& frequencyVector );

        


    }

}


#endif // UTILS_H
