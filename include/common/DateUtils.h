#ifndef DATE_UTILS_H
#define DATE_UTILS_H

#include "InputHelper.h"

#include <string>

namespace DateUtils {

inline bool isValidDate(const std::string& value) {
    if (value.size() != 10 || value[2] != '/' || value[5] != '/') {
        return false;
    }
    for (std::size_t i = 0; i < value.size(); ++i) {
        if (i != 2 && i != 5 && (value[i] < '0' || value[i] > '9')) {
            return false;
        }
    }

    const int day = (value[0] - '0') * 10 + (value[1] - '0');
    const int month = (value[3] - '0') * 10 + (value[4] - '0');
    const int year = (value[6] - '0') * 1000 + (value[7] - '0') * 100 +
                     (value[8] - '0') * 10 + (value[9] - '0');
    if (year < 1 || month < 1 || month > 12) {
        return false;
    }

    const int monthDays[] = {31, 28, 31, 30, 31, 30,
                             31, 31, 30, 31, 30, 31};
    int maximumDay = monthDays[month - 1];
    const bool leapYear = year % 400 == 0 || (year % 4 == 0 && year % 100 != 0);
    if (month == 2 && leapYear) {
        maximumDay = 29;
    }
    return day >= 1 && day <= maximumDay;
}

inline bool isDateBefore(const std::string& first, const std::string& second) {
    if (!isValidDate(first) || !isValidDate(second)) {
        return false;
    }
    const std::string firstSortable = first.substr(6, 4) +
                                      first.substr(3, 2) + first.substr(0, 2);
    const std::string secondSortable = second.substr(6, 4) +
                                       second.substr(3, 2) + second.substr(0, 2);
    return firstSortable < secondSortable;
}

inline std::string readDate(const std::string& prompt) {
    while (true) {
        const std::string value = InputHelper::readLine(prompt);
        if (isValidDate(value)) {
            return value;
        }
        std::cout << "Ngay khong hop le. Hay nhap theo dang dd/mm/yyyy.\n";
    }
}

}  // namespace DateUtils

#endif
