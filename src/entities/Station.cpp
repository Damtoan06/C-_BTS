#include "../../include/entities/Station.h"

#include "../../include/common/AppException.h"
#include "../../include/common/InputHelper.h"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

namespace {

vector<string> splitRecord(const string& line) {
    vector<string> fields;
    stringstream stream(line);
    string field;
    while (getline(stream, field, '|')) {
        fields.push_back(field);
    }
    if (!line.empty() && line[line.size() - 1] == '|') {
        fields.push_back("");
    }
    return fields;
}

}

Station::Station() {}

Station::Station(const string& id, const string& name,
                 const string& location, const string& status) {
    validateId(id);
    setId(id);
    setName(name);
    setLocation(location);
    setStatus(status);
}

const string& Station::getName() const { return name_; }
const string& Station::getLocation() const { return location_; }
const string& Station::getStatus() const { return status_; }

void Station::validateId(const string& id) {
    if (id.size() != 6 || id.compare(0, 3, "BTS") != 0) {
        throw AppException("Ma tram phai co dang BTS001.");
    }
    for (size_t i = 3; i < id.size(); ++i) {
        if (id[i] < '0' || id[i] > '9') {
            throw AppException("Phan sau BTS trong ma tram phai la chu so.");
        }
    }
}

void Station::validateField(const string& value, const string& fieldName) {
    if (!InputHelper::isNonEmpty(value) || value.find('|') != string::npos ||
        value.find('\n') != string::npos || value.find('\r') != string::npos) {
        throw AppException(fieldName +
                           " khong duoc rong hoac chua ky tu | / xuong dong.");
    }
}

void Station::setName(const string& value) {
    validateField(value, "Ten tram");
    name_ = value;
}

void Station::setLocation(const string& value) {
    validateField(value, "Vi tri tram");
    location_ = value;
}

void Station::setStatus(const string& value) {
    validateField(value, "Trang thai");
    status_ = value;
}

void Station::input() {
    setId(InputHelper::readValidLine(
        "Ma tram (BTSxxx): ",
        [](const string& value) { validateId(value); }));
    InputHelper::readValidLine(
        "Ten tram: ", [this](const string& value) { setName(value); });
    InputHelper::readValidLine(
        "Vi tri tram: ", [this](const string& value) { setLocation(value); });
    InputHelper::readValidLine(
        "Trang thai: ", [this](const string& value) { setStatus(value); });
}

void Station::display() const {
    cout << left << setw(12) << getId()
         << setw(26) << name_.substr(0, 25)
         << setw(32) << location_.substr(0, 31)
         << status_ << '\n';
}

string Station::serialize() const {
    return getId() + '|' + name_ + '|' + location_ + '|' + status_;
}

void Station::deserialize(const string& line) {
    const vector<string> fields = splitRecord(line);
    if (fields.size() != 4) {
        throw AppException("Ban ghi tram phai co 4 truong.");
    }
    validateId(fields[0]);
    setId(fields[0]);
    setName(fields[1]);
    setLocation(fields[2]);
    setStatus(fields[3]);
}
