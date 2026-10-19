#include "../../include/common/InputHelper.h"
#include "../../include/common/Repository.h"
#include "../../include/entities/Technician.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

namespace {

void showTechnicians(const vector<Technician>& technicians) {
    if (technicians.empty()) {
        cout << "Danh sach rong.\n";
        return;
    }
    Technician::displayHeader();
    for (vector<Technician>::const_iterator it = technicians.begin();
         it != technicians.end(); ++it) {
        it->display();
    }
}

void showTechnicianDetails(const Technician& technician) {
    cout << "Ma KTV: " << technician.getId() << '\n'
              << "Ho va ten: " << technician.getFullName() << '\n'
              << "So dien thoai: " << technician.getPhoneNumber() << '\n'
              << "Chuyen mon: " << technician.getSpecialization() << '\n'
              << "Cap chung chi: " << technician.getCertLevel() << '\n'
              << "Trang thai: " << technician.getStatus() << '\n';
}

void editTechnician(Technician& technician) {
    string value = InputHelper::readLine(
        "Ho ten [" + technician.getFullName() + "]: ");
    if (!value.empty()) technician.setFullName(value);
    value = InputHelper::readLine(
        "So dien thoai [" + technician.getPhoneNumber() + "]: ");
    if (!value.empty()) technician.setPhoneNumber(value);
    value = InputHelper::readLine(
        "Chuyen mon [" + technician.getSpecialization() + "]: ");
    if (!value.empty()) technician.setSpecialization(value);
    value = InputHelper::readLine(
        "Cap chung chi [" + technician.getCertLevel() + "]: ");
    if (!value.empty()) technician.setCertLevel(value);
    value = InputHelper::readLine(
        "Trang thai [" + technician.getStatus() + "]: ");
    if (!value.empty()) technician.setStatus(value);
}

}

void runTechnicianMenu() {
    Repository<Technician> repository("data/technicians.txt");
    while (true) {
        cout << "\n=== QUAN LY KY THUAT VIEN ===\n"
                  << "1. Them moi\n2. Xem danh sach\n"
                  << "3. Tim kiem (ma/ten/SDT/chuyen mon)\n"
                  << "4. Sap xep theo ten\n5. Cap nhat\n6. Xoa\n0. Quay lai\n";
        const int choice = InputHelper::readChoice("Chon: ", 0, 6);
        try {
            if (choice == 0) return;
            if (choice == 1) {
                Technician technician;
                technician.input();
                repository.add(technician);
                cout << "Da them va luu ky thuat vien.\n";
            } else if (choice == 2) {
                showTechnicians(repository.all());
            } else if (choice == 3) {
                const string rawQuery = InputHelper::readNonEmpty("Nhap tu khoa: ");
                const Technician* exact = repository.findById(rawQuery);
                if (exact != 0) {
                    showTechnicianDetails(*exact);
                    continue;
                }
                const string query = InputHelper::lower(rawQuery);
                vector<Technician> matches;
                const vector<Technician>& all = repository.all();
                for (vector<Technician>::const_iterator it = all.begin();
                     it != all.end(); ++it) {
                    if (InputHelper::lower(it->getId()).find(query) != string::npos ||
                        InputHelper::lower(it->getFullName()).find(query) != string::npos ||
                        InputHelper::lower(it->getPhoneNumber()).find(query) != string::npos ||
                        InputHelper::lower(it->getSpecialization()).find(query) != string::npos) {
                        matches.push_back(*it);
                    }
                }
                showTechnicians(matches);
            } else if (choice == 4) {
                showTechnicians(repository.sorted(
                    [](const Technician& left, const Technician& right) {
                        return InputHelper::lower(left.getFullName()) <
                               InputHelper::lower(right.getFullName());
                    }));
            } else if (choice == 5) {
                const string id = InputHelper::readNonEmpty("Ma KTV can sua: ");
                const Technician* current = repository.findById(id);
                if (current == 0) {
                    cout << "Khong tim thay ma " << id << ".\n";
                    continue;
                }
                current->display();
                repository.update(id, editTechnician);
                cout << "Da cap nhat va luu file.\n";
            } else {
                const string id = InputHelper::readNonEmpty("Ma KTV can xoa: ");
                const Technician* current = repository.findById(id);
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
