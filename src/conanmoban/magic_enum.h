#pragma once
#include <algorithm>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

std::vector<std::string> splitString(std::string str, char sep = ',');

#define DECLARE_ENUM_INTERFACE_WITH_TYPE(E, T, ...)        \
    enum class E : T { __VA_ARGS__ };                      \
    std::ostream &operator<<(std::ostream &os, E enumTmp); \
    size_t operator*(E enumTmp);                           \
    std::string operator~(E enumTmp);                      \
    std::string operator+(std::string &&str, E enumTmp);   \
    std::string operator+(E enumTmp, std::string &&str);   \
    std::string &operator+=(std::string &str, E enumTmp);  \
    E operator++(E &enumTmp);                              \
    bool valid##E(T value);

#define DECLARE_ENUM_INTERFACE(E, ...) \
    DECLARE_ENUM_INTERFACE_WITH_TYPE(E, int32_t, __VA_ARGS__)

#define DECLARE_ENUM_WITH_TYPE(E, T, ...)                                    \
    std::map<T, std::string> E##MapName(generateEnumMap<T>(#__VA_ARGS__));   \
    std::ostream &operator<<(std::ostream &os, E enumTmp) {                  \
        os << E##MapName[static_cast<T>(enumTmp)];                           \
        return os;                                                           \
    }                                                                        \
    size_t operator*(E enumTmp) {                                            \
        (void)enumTmp;                                                       \
        return E##MapName.size();                                            \
    }                                                                        \
    std::string operator~(E enumTmp) {                                       \
        return E##MapName[static_cast<T>(enumTmp)];                          \
    }                                                                        \
    std::string operator+(std::string &&str, E enumTmp) {                    \
        return str + E##MapName[static_cast<T>(enumTmp)];                    \
    }                                                                        \
    std::string operator+(E enumTmp, std::string &&str) {                    \
        return E##MapName[static_cast<T>(enumTmp)] + str;                    \
    }                                                                        \
    std::string &operator+=(std::string &str, E enumTmp) {                   \
        str += E##MapName[static_cast<T>(enumTmp)];                          \
        return str;                                                          \
    }                                                                        \
    E operator++(E &enumTmp) {                                               \
        auto iter = E##MapName.find(static_cast<T>(enumTmp));                \
        if (iter == E##MapName.end() || std::next(iter) == E##MapName.end()) \
            iter = E##MapName.begin();                                       \
        else {                                                               \
            ++iter;                                                          \
        }                                                                    \
        enumTmp = static_cast<E>(iter->first);                               \
        return enumTmp;                                                      \
    }                                                                        \
    bool valid##E(T value) {                                                 \
        return (E##MapName.find(value) != E##MapName.end());                 \
    }

#define DECLARE_ENUM(E, ...) DECLARE_ENUM_WITH_TYPE(E, int32_t, __VA_ARGS__)

template <typename T>
std::map<T, std::string> generateEnumMap(std::string strMap);

// DECLARE_ENUM_WITH_TYPE(TestEnumClass, int32_t, ZERO = 0x00, TWO = 0x02,
//                        ONE = 0x01, THREE = 0x03, FOUR);

// int main(void) {
//     TestEnumClass first, second;
//     first = TestEnumClass::FOUR;
//     second = TestEnumClass::TWO;

//     std::cout << first << "(" << static_cast<uint32_t>(first) << ")"
//               << std::endl;  // FOUR(4)

//     std::string strOne;
//     strOne = ~first;
//     std::cout << strOne << std::endl;  // FOUR

//     std::string strTwo;
//     strTwo = ("Enum-" + second) + (TestEnumClass::THREE + "-test");
//     std::cout << strTwo << std::endl;  // Enum-TWOTHREE-test

//     std::string strThree("TestEnumClass: ");
//     strThree += second;
//     std::cout << strThree << std::endl;  // TestEnumClass: TWO
//     std::cout << "Enum count=" << *first << std::endl;
// }
