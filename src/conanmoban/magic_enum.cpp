#include "magic_enum.h"
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#define STRING_REMOVE_CHAR(str, ch) \
    str.erase(std::remove(str.begin(), str.end(), ch), str.end())

template <typename T>
std::unordered_map<T, std::string> generateEnumMap(std::string strMap) {
    STRING_REMOVE_CHAR(strMap, ' ');
    STRING_REMOVE_CHAR(strMap, '(');
    std::vector<std::string> enumTokens(splitString(strMap));
    std::unordered_map<T, std::string> retMap;
    T inxMap;
    inxMap = 0;
    for (auto iter = enumTokens.begin(); iter != enumTokens.end(); ++iter) {
        // Token: [EnumName | EnumName=EnumValue]
        std::string enumName;
        T enumValue;
        if (iter->find('=') == std::string::npos) {
            enumName = *iter;
        } else {
            std::vector<std::string> enumNameValue(splitString(*iter, '='));
            enumName = enumNameValue[0];
            // inxMap = static_cast<T>(enumNameValue[1]);
            if (std::is_unsigned<T>::value) {
                inxMap = static_cast<T>(std::stoull(enumNameValue[1], 0, 0));
            } else {
                inxMap = static_cast<T>(std::stoll(enumNameValue[1], 0, 0));
            }
        }
        retMap[inxMap++] = enumName;
    }
    return retMap;
}

std::vector<std::string> splitString(std::string str, char sep = ',') {
    std::vector<std::string> vecString;
    std::string item;
    std::stringstream stringStream(str);
    while (std::getline(stringStream, item, sep)) {
        vecString.push_back(item);
    }
    return vecString;
}
