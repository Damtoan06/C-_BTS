#include "../../include/common/DateUtils.h"
#include "../../include/common/InputHelper.h"
#include "../../include/common/Repository.h"
#include "../../include/entities/SiteRentalContract.h"
#include "../../include/entities/Station.h"
#include "../../include/common/AppException.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

namespace {

void showContracts(const vector<SiteRentalContract>& contracts) {
    if (contracts.empty()) {
        cout << "Danh sach rong.\n";
        return;
    }
    SiteRentalContract::displayHeader();
    for (vector<SiteRentalContract>::const_iterator it = contracts.begin();
         it != contracts.end(); ++it) {
        it->display();
    }
}

void showContractDetails(const SiteRentalContract& contract) {
    cout << "Ma hop dong: " << contract.getId() << '\n'
              << "Ma tram: " << contract.getStationId() << '\n'
              << "Ben cho thue: " << contract.getLessorName() << '\n'
              << "Dien thoai: " << contract.getLessorPhone() << '\n'
              << "Ngay bat dau: " << contract.getStartDate() << '\n'
              << "Ngay het han: " << contract.getEndDate() << '\n'
              << "Gia thue/thang: " << contract.getMonthlyRent() << '\n'
              << "Chu ky thanh toan: " << contract.getPaymentCycle() << '\n'
              << "Trang thai: " << contract.getStatus() << '\n';
}

void editContract(SiteRentalContract& contract) {
    string value = InputHelper::readLine(
        "Ma tram [" + contract.getStationId() + "]: ");
    if (!value.empty()) contract.setStationId(value);
    value = InputHelper::readLine(
        "Ben cho thue [" + contract.getLessorName() + "]: ");
    if (!value.empty()) contract.setLessorName(value);
    value = InputHelper::readLine(
        "Dien thoai [" + contract.getLessorPhone() + "]: ");
    if (!value.empty()) contract.setLessorPhone(value);
    const string newStartDate = InputHelper::readLine(
        "Ngay bat dau [" + contract.getStartDate() + "]: ");
    const string newEndDate = InputHelper::readLine(
        "Ngay het han [" + contract.getEndDate() + "]: ");
    const string startDate =
        newStartDate.empty() ? contract.getStartDate() : newStartDate;
    const string endDate =
        newEndDate.empty() ? contract.getEndDate() : newEndDate;
    if (!DateUtils::isValidDate(startDate) ||
        !DateUtils::isValidDate(endDate) ||
        !DateUtils::isDateBefore(startDate, endDate)) {
        throw InvalidDateException("Khoang ngay hop dong khong hop le.");
    }
    if (!newStartDate.empty() && !newEndDate.empty() &&
        !DateUtils::isDateBefore(startDate, contract.getEndDate())) {
        contract.setEndDate(endDate);
        contract.setStartDate(startDate);
    } else {
        if (!newStartDate.empty()) contract.setStartDate(startDate);
        if (!newEndDate.empty()) contract.setEndDate(endDate);
    }
    ostringstream currentRent;
    currentRent << contract.getMonthlyRent();
    value = InputHelper::readLine("Gia thue/thang [" + currentRent.str() + "]: ");
    if (!value.empty()) {
        istringstream parser(value);
        double rent;
        char extra;
        if (!(parser >> rent) || (parser >> extra)) {
            throw AppException("Gia thue khong hop le.");
        }
        contract.setMonthlyRent(rent);
    }
    value = InputHelper::readLine(
        "Chu ky thanh toan [" + contract.getPaymentCycle() + "]: ");
    if (!value.empty()) contract.setPaymentCycle(value);
    value = InputHelper::readLine(
        "Trang thai [" + contract.getStatus() + "]: ");
    if (!value.empty()) contract.setStatus(value);
}

}

void runContractMenu() {
    Repository<Station> stations("data/stations.txt");
    Repository<SiteRentalContract> repository(
        "data/contracts.txt",
        [&stations](const SiteRentalContract& contract) {
            if (stations.findById(contract.getStationId()) == 0) {
                throw ReferenceIntegrityException(
                    "Ma tram khong ton tai: " + contract.getStationId());
            }
        });
    while (true) {
        cout << "\n=== QUAN LY HOP DONG THUE VI TRI ===\n"
                  << "1. Them moi\n2. Xem danh sach\n"
                  << "3. Tim kiem (ma/tram/ben cho thue)\n"
                  << "4. Sap xep theo gia thue\n5. Cap nhat\n6. Xoa\n0. Quay lai\n";
        const int choice = InputHelper::readChoice("Chon: ", 0, 6);
        try {
            if (choice == 0) return;
            if (choice == 1) {
                SiteRentalContract contract;
                contract.input();
                repository.add(contract);
                cout << "Da them va luu hop dong.\n";
            } else if (choice == 2) {
                showContracts(repository.all());
            } else if (choice == 3) {
                const string rawQuery = InputHelper::readNonEmpty("Nhap tu khoa: ");
                const SiteRentalContract* exact = repository.findById(rawQuery);
                if (exact != 0) {
                    showContractDetails(*exact);
                    continue;
                }
                const string query = InputHelper::lower(rawQuery);
                vector<SiteRentalContract> matches;
                const vector<SiteRentalContract>& all = repository.all();
                for (vector<SiteRentalContract>::const_iterator it = all.begin();
                     it != all.end(); ++it) {
                    if (InputHelper::lower(it->getId()).find(query) != string::npos ||
                        InputHelper::lower(it->getStationId()).find(query) != string::npos ||
                        InputHelper::lower(it->getLessorName()).find(query) != string::npos) {
                        matches.push_back(*it);
                    }
                }
                showContracts(matches);
            } else if (choice == 4) {
                showContracts(repository.sorted(
                    [](const SiteRentalContract& left,
                       const SiteRentalContract& right) {
                        return left.getMonthlyRent() < right.getMonthlyRent();
                    }));
            } else if (choice == 5) {
                const string id = InputHelper::readNonEmpty("Ma HD can sua: ");
                const SiteRentalContract* current = repository.findById(id);
                if (current == 0) {
                    cout << "Khong tim thay ma " << id << ".\n";
                    continue;
                }
                current->display();
                repository.update(id, editContract);
                cout << "Da cap nhat va luu file.\n";
            } else {
                const string id = InputHelper::readNonEmpty("Ma HD can xoa: ");
                const SiteRentalContract* current = repository.findById(id);
                if (current == 0) {
                    cout << "Khong tim thay ma " << id << ".\n";
                    continue;
                }
                current->display();
                if (InputHelper::isConfirmed("Xac nhan xoa? (y/n): ")) {
                    repository.remove(id);
                    cout << "Da xoa va cap nhat file.\n";
                } else {
                    cout << "Da huy thao tac xoa.\n";
                }
            }
        } catch (const exception& error) {
            cout << "Loi: " << error.what() << '\n';
        }
    }
}
