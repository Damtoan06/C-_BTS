#include "../../include/entities/Technician.h"

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

Technician::Technician() {}

Technician::Technician(const string& id, const string& fullName,
                       const string& phoneNumber,
                       const string& specialization,
                       const string& certLevel, const string& status) {
    validateId(id);
    setId(id);
    setFullName(fullName);
    setPhoneNumber(phoneNumber);
    setSpecialization(specialization);
    setCertLevel(certLevel);
    setStatus(status);
}

const string& Technician::getFullName() const { return fullName_; }
const string& Technician::getPhoneNumber() const { return phoneNumber_; }
const string& Technician::getSpecialization() const { return specialization_; }
const string& Technician::getCertLevel() const { return certLevel_; }
const string& Technician::getStatus() const { return status_; }

void Technician::validateField(const string& value,
                               const string& fieldName) {
    if (!InputHelper::isNonEmpty(value) || value.find('|') != string::npos ||
        value.find('\n') != string::npos ||
        value.find('\r') != string::npos) {
        throw AppException(fieldName +
                           " khong duoc rong hoac chua ky tu | / xuong dong.");
    }
}

void Technician::validateId(const string& id) {
    if (id.size() != 6 || id.substr(0, 3) != "KTV") {
        throw AppException("Ma ky thuat vien phai co dang KTV001.");
    }
    for (size_t i = 3; i < id.size(); ++i) {
        if (id[i] < '0' || id[i] > '9') {
            throw AppException("Phan sau KTV trong ma phai la chu so.");
        }
    }
}

void Technician::setFullName(const string& value) {
    validateField(value, "Ho ten");
    fullName_ = value;
}
void Technician::setPhoneNumber(const string& value) {
    validateField(value, "So dien thoai");
    for (string::const_iterator it = value.begin(); it != value.end(); ++it) {
        if ((*it < '0' || *it > '9') && *it != '+' && *it != '-' && *it != ' ') {
            throw AppException("So dien thoai chi gom chu so, +, - va khoang trang.");
        }
    }
    phoneNumber_ = value;
}
void Technician::setSpecialization(const string& value) {
    validateField(value, "Chuyen mon");
    specialization_ = value;
}
void Technician::setCertLevel(const string& value) {
    validateField(value, "Cap chung chi");
    certLevel_ = value;
}
void Technician::setStatus(const string& value) {
    validateField(value, "Trang thai");
    status_ = value;
}

void Technician::input() {
    setId(InputHelper::readValidLine(
        "Ma KTV (KTVxxx): ",
        [](const string& value) { validateId(value); }));
    InputHelper::readValidLine(
        "Ho va ten: ",
        [this](const string& value) { setFullName(value); });
    InputHelper::readValidLine(
        "So dien thoai: ",
        [this](const string& value) { setPhoneNumber(value); });
    InputHelper::readValidLine(
        "Chuyen mon: ",
        [this](const string& value) { setSpecialization(value); });
    InputHelper::readValidLine(
        "Cap chung chi: ",
        [this](const string& value) { setCertLevel(value); });
    InputHelper::readValidLine(
        "Trang thai: ",
        [this](const string& value) { setStatus(value); });
}

void Technician::displayHeader() {
    cout << left << setw(12) << "Ma KTV"
              << setw(26) << "Ho va ten"
              << setw(17) << "Dien thoai"
              << setw(22) << "Chuyen mon"
              << setw(14) << "Chung chi"
              << setw(16) << "Trang thai" << '\n';
    cout << string(107, '-') << '\n';
}

void Technician::display() const {
    cout << left << setw(12) << getId().substr(0, 11)
              << setw(26) << fullName_.substr(0, 25)
              << setw(17) << phoneNumber_.substr(0, 16)
              << setw(22) << specialization_.substr(0, 21)
              << setw(14) << certLevel_.substr(0, 13)
              << setw(16) << status_.substr(0, 15) << '\n';
}

string Technician::serialize() const {
    ostringstream record;
    record << getId() << '|' << fullName_ << '|' << phoneNumber_ << '|'
           << specialization_ << '|' << certLevel_ << '|' << status_;
    return record.str();
}

void Technician::deserialize(const string& line) {
    const vector<string> fields = splitRecord(line);
    if (fields.size() != 6) {
        throw AppException("Ban ghi ky thuat vien phai co 6 truong.");
    }
    validateId(fields[0]);
    setId(fields[0]);
    setFullName(fields[1]);
    setPhoneNumber(fields[2]);
    setSpecialization(fields[3]);
    setCertLevel(fields[4]);
    setStatus(fields[5]);
}
