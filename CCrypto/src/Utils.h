#ifndef UTILS_H
#define UTILS_H

#include <map>
#include <string>
#include <memory>
#include <vector>

/* CCrypto *********************************************************************************/
namespace CCrypto::Utils {

        
        /**
         * @brief Computes a normalized ASCII frequency vector from the contents of a file.
         *
         * This function loads the text from the specified file and builds a frequency
         * vector of size 256, where each index corresponds to an ASCII character.
         * Uppercase and lowercase letters share accumulated frequencies to make the
         * distribution case-insensitive. The resulting frequencies are then normalized
         * by the total text length.
         *
         * @param fileName  Path to the file from which to read the text.
         *
         * @return A vector of 256 doubles representing the normalized frequency of each
         *         ASCII character in the input text.
         */
        std::vector<double> getFrequencyCharInVector(const std::string& fileName);


        /**
         * @brief Loads the entire contents of a file into a string.
         *
         * @param fileName  Path to the file.
         * @return The file contents as a string, or an empty string if it cannot be opened.
         */
        std::string loadTextFromFile(const std::string& fileName);


        /**
         * @brief Computes an English-likelihood score for the given text.
         *
         * The score is calculated using a frequency vector where each element
         * corresponds to the weight of the ASCII character with the same index.
         *
         * @param text            The text to evaluate.
         * @param frequencyVector The ASCII-indexed character weight vector.
         *
         * @return The normalized English score of the text.
         */
        double englishScore(const std::string& text, const std::vector<double>& frequencyVector );



        /**
        * @brief Hex dump of a vector of bytes
        */
        void hexDump(const std::vector<uint8_t>& bytes );




}


#endif // UTILS_H
