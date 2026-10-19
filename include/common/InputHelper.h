#ifndef INPUT_HELPER_H
#define INPUT_HELPER_H

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace InputHelper {

inline bool isNonEmpty(const std::string& value) {
    for (std::string::const_iterator it = value.begin(); it != value.end(); ++it) {
        if (*it != ' ' && *it != '\t' && *it != '\n' && *it != '\r') {
            return true;
        }
    }
    return false;
}

inline std::string readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string value;
    if (!std::getline(std::cin, value)) {
        throw std::runtime_error("Da ket thuc luong nhap.");
    }
    return value;
}

inline std::string readNonEmpty(const std::string& prompt) {
    while (true) {
        const std::string value = readLine(prompt);
        if (isNonEmpty(value)) {
            return value;
        }
        std::cout << "Gia tri khong duoc de trong.\n";
    }
}

template <typename Validator>
inline std::string readValidLine(const std::string& prompt,
                                 Validator validator) {
    while (true) {
        const std::string value = readLine(prompt);
        try {
            validator(value);
            return value;
        } catch (const std::exception& error) {
            std::cout << error.what() << '\n';
        }
    }
}

inline int readChoice(const std::string& prompt, int minimum, int maximum) {
    while (true) {
        const std::string value = readLine(prompt);
        std::istringstream parser(value);
        int choice;
        char extra;
        if (parser >> choice && !(parser >> extra) &&
            choice >= minimum && choice <= maximum) {
            return choice;
        }
        std::cout << "Lua chon khong hop le. Vui long nhap lai.\n";
    }
}

inline bool isConfirmed(const std::string& prompt) {
    while (true) {
        const std::string answer = readLine(prompt);
        if (answer == "y" || answer == "Y") {
            return true;
        }
        if (answer == "n" || answer == "N") {
            return false;
        }
        std::cout << "Chi chap nhan y hoac n.\n";
    }
}

inline std::string lower(std::string value) {
    for (std::string::iterator it = value.begin(); it != value.end(); ++it) {
        if (*it >= 'A' && *it <= 'Z') {
            *it = static_cast<char>(*it - 'A' + 'a');
        }
    }
    return value;
}

}  // namespace InputHelper

#endif
