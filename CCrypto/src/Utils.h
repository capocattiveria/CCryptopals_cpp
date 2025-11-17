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
        // std::map<char, float> getFrequencyCharFromText(const std::string& str);
        std::vector<double> getFrequencyCharFromText(const std::string& str);


    }

}


#endif // UTILS_H
