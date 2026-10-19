#include "common/AppException.h"
#include "common/InputHelper.h"

#include <exception>
#include <iostream>

using namespace std;

void runTechnicianMenu();
void runContractMenu();

int main() {
    try {
        while (true) {
            cout << "\n=== QUAN LY HA TANG TRAM BTS ===\n"
                      << "1. Quan ly ky thuat vien\n"
                      << "2. Quan ly hop dong thue vi tri\n"
                      << "0. Thoat\n";
            const int choice = InputHelper::readChoice("Chon: ", 0, 2);
            if (choice == 0) {
                return 0;
            }
            try {
                if (choice == 1) {
                    runTechnicianMenu();
                } else {
                    runContractMenu();
                }
            } catch (const exception& error) {
                cerr << "Khong the mo phan he: " << error.what() << '\n';
            }
        }
    } catch (const exception& error) {
        cerr << "Chuong trinh ket thuc: " << error.what() << '\n';
        return 1;
    }
}
