#include "../../include/entities/SiteRentalContract.h"

#include "../../include/common/AppException.h"
#include "../../include/common/DateUtils.h"
#include "../../include/common/InputHelper.h"

#include <cmath>
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

bool parseRent(const string& value, double& rent) {
    istringstream parser(value);
    char extra;
    return (parser >> rent) && !(parser >> extra) &&
           isfinite(rent) && rent > 0.0;
}

string normalizeDate(const string& value) {
    if (value.size() == 10 && value[4] == '-' && value[7] == '-') {
        return value.substr(8, 2) + "/" + value.substr(5, 2) + "/" +
               value.substr(0, 4);
    }
    return value;
}

}

SiteRentalContract::SiteRentalContract() : monthlyRent_(0.0) {}

SiteRentalContract::SiteRentalContract(
    const string& id, const string& stationId,
    const string& lessorName, const string& lessorPhone,
    const string& startDate, const string& endDate,
    double monthlyRent, const string& paymentCycle,
    const string& status) : monthlyRent_(0.0) {
    validateId(id);
    setId(id);
    setStationId(stationId);
    setLessorName(lessorName);
    setLessorPhone(lessorPhone);
    setStartDate(startDate);
    setEndDate(endDate);
    setMonthlyRent(monthlyRent);
    setPaymentCycle(paymentCycle);
    setStatus(status);
}

const string& SiteRentalContract::getStationId() const { return stationId_; }
const string& SiteRentalContract::getLessorName() const { return lessorName_; }
const string& SiteRentalContract::getLessorPhone() const { return lessorPhone_; }
const string& SiteRentalContract::getStartDate() const { return startDate_; }
const string& SiteRentalContract::getEndDate() const { return endDate_; }
double SiteRentalContract::getMonthlyRent() const { return monthlyRent_; }
const string& SiteRentalContract::getPaymentCycle() const { return paymentCycle_; }
const string& SiteRentalContract::getStatus() const { return status_; }

void SiteRentalContract::validateField(const string& value,
                                       const string& fieldName) {
    if (!InputHelper::isNonEmpty(value) || value.find('|') != string::npos ||
        value.find('\n') != string::npos ||
        value.find('\r') != string::npos) {
        throw AppException(fieldName +
                           " khong duoc rong hoac chua ky tu | / xuong dong.");
    }
}

void SiteRentalContract::validateId(const string& id) {
    if (id.size() != 5 || id.substr(0, 2) != "HD") {
        throw AppException("Ma hop dong phai co dang HD001.");
    }
    for (size_t i = 2; i < id.size(); ++i) {
        if (id[i] < '0' || id[i] > '9') {
            throw AppException("Phan sau HD trong ma phai la chu so.");
        }
    }
}

void SiteRentalContract::validateStationId(const string& id) {
    if (id.size() != 6 || id.substr(0, 3) != "BTS") {
        throw ReferenceIntegrityException("Ma tram phai co dang BTS001.");
    }
    for (size_t i = 3; i < id.size(); ++i) {
        if (id[i] < '0' || id[i] > '9') {
            throw ReferenceIntegrityException(
                "Phan sau BTS trong ma tram phai la chu so.");
        }
    }
}

void SiteRentalContract::setStationId(const string& value) {
    validateStationId(value);
    stationId_ = value;
}
void SiteRentalContract::setLessorName(const string& value) {
    validateField(value, "Ten ben cho thue");
    lessorName_ = value;
}
void SiteRentalContract::setLessorPhone(const string& value) {
    validateField(value, "So dien thoai ben cho thue");
    lessorPhone_ = value;
}
void SiteRentalContract::setStartDate(const string& value) {
    if (!DateUtils::isValidDate(value)) {
        throw InvalidDateException("Ngay bat dau khong hop le (dung dd/mm/yyyy).");
    }
    if (!endDate_.empty() && !DateUtils::isDateBefore(value, endDate_)) {
        throw InvalidDateException("Ngay bat dau phai truoc ngay het han.");
    }
    startDate_ = value;
}
void SiteRentalContract::setEndDate(const string& value) {
    if (!DateUtils::isValidDate(value)) {
        throw InvalidDateException("Ngay het han khong hop le (dung dd/mm/yyyy).");
    }
    if (!startDate_.empty() && !DateUtils::isDateBefore(startDate_, value)) {
        throw InvalidDateException("Ngay het han phai sau ngay bat dau.");
    }
    endDate_ = value;
}
void SiteRentalContract::setMonthlyRent(double value) {
    if (!isfinite(value) || value <= 0.0) {
        throw AppException("Gia thue hang thang phai lon hon 0.");
    }
    monthlyRent_ = value;
}
void SiteRentalContract::setPaymentCycle(const string& value) {
    validateField(value, "Chu ky thanh toan");
    paymentCycle_ = value;
}
void SiteRentalContract::setStatus(const string& value) {
    validateField(value, "Trang thai");
    status_ = value;
}

void SiteRentalContract::input() {
    setId(InputHelper::readValidLine(
        "Ma hop dong (HDxxx): ",
        [](const string& value) { validateId(value); }));
    InputHelper::readValidLine(
        "Ma tram (BTSxxx): ",
        [this](const string& value) { setStationId(value); });
    InputHelper::readValidLine(
        "Ten ben cho thue: ",
        [this](const string& value) { setLessorName(value); });
    InputHelper::readValidLine(
        "So dien thoai ben cho thue: ",
        [this](const string& value) { setLessorPhone(value); });
    setStartDate(DateUtils::readDate("Ngay bat dau (dd/mm/yyyy): "));
    while (true) {
        const string date = DateUtils::readDate("Ngay het han (dd/mm/yyyy): ");
        try {
            setEndDate(date);
            break;
        } catch (const InvalidDateException& error) {
            cout << error.what() << '\n';
        }
    }
    while (true) {
        const string value = InputHelper::readLine("Gia thue hang thang: ");
        double rent;
        if (parseRent(value, rent)) {
            setMonthlyRent(rent);
            break;
        }
        cout << "Gia thue phai la so lon hon 0.\n";
    }
    InputHelper::readValidLine(
        "Chu ky thanh toan: ",
        [this](const string& value) { setPaymentCycle(value); });
    InputHelper::readValidLine(
        "Trang thai: ",
        [this](const string& value) { setStatus(value); });
}

void SiteRentalContract::displayHeader() {
    cout << left << setw(11) << "Ma HD"
              << setw(11) << "Ma tram"
              << setw(23) << "Ben cho thue"
              << setw(17) << "Dien thoai"
              << setw(13) << "Bat dau"
              << setw(13) << "Het han"
              << right << setw(16) << "Gia/thang" << ' '
              << left << setw(16) << "Chu ky"
              << setw(14) << "Trang thai" << '\n';
    cout << string(133, '-') << '\n';
}

void SiteRentalContract::display() const {
    cout << left << setw(11) << getId().substr(0, 10)
              << setw(11) << stationId_.substr(0, 10)
              << setw(23) << lessorName_.substr(0, 22)
              << setw(17) << lessorPhone_.substr(0, 16)
              << setw(13) << startDate_
              << setw(13) << endDate_
              << right << setw(16) << fixed
              << setprecision(0) << monthlyRent_ << ' '
              << left << setw(16) << paymentCycle_.substr(0, 15)
              << setw(14) << status_.substr(0, 13) << '\n';
}

string SiteRentalContract::serialize() const {
    ostringstream record;
    record << getId() << '|' << stationId_ << '|' << lessorName_ << '|'
           << lessorPhone_ << '|' << startDate_ << '|' << endDate_ << '|'
           << setprecision(15) << monthlyRent_ << '|' << paymentCycle_
           << '|' << status_;
    return record.str();
}

void SiteRentalContract::deserialize(const string& line) {
    const vector<string> fields = splitRecord(line);
    if (fields.size() != 9) {
        throw AppException("Ban ghi hop dong phai co 9 truong.");
    }
    validateId(fields[0]);
    setId(fields[0]);
    setStationId(fields[1]);
    setLessorName(fields[2]);
    setLessorPhone(fields[3]);
    setStartDate(normalizeDate(fields[4]));
    setEndDate(normalizeDate(fields[5]));
    double rent;
    if (!parseRent(fields[6], rent)) {
        throw AppException("Gia thue trong file phai la so lon hon 0.");
    }
    setMonthlyRent(rent);
    setPaymentCycle(fields[7]);
    setStatus(fields[8]);
}
