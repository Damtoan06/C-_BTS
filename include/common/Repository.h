#ifndef REPOSITORY_H
#define REPOSITORY_H

#include "AppException.h"
#include "Entity.h"

#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

template <typename T>
class Repository {
public:
    typedef std::function<void(const T&)> Validator;

    explicit Repository(const std::string& filePath,
                        const Validator& validator = Validator())
        : filePath_(filePath), validator_(validator) {
        ensureFileExists();
        load();
    }

    const std::vector<T>& all() const { return items_; }

    const T* findById(const std::string& id) const {
        typename std::vector<T>::const_iterator it = std::find_if(
            items_.begin(), items_.end(),
            [&id](const T& item) { return item.getId() == id; });
        return it == items_.end() ? 0 : &(*it);
    }

    void add(const T& item) {
        if (item.getId().empty()) {
            throw AppException("Ma dinh danh khong duoc de trong.");
        }
        if (findById(item.getId()) != 0) {
            throw DuplicateIdException(item.getId());
        }
        validate(item);
        items_.push_back(item);
        try {
            save();
        } catch (...) {
            items_.pop_back();
            throw;
        }
    }

    void update(const std::string& id,
                const std::function<void(T&)>& editFunction) {
        typename std::vector<T>::iterator it = std::find_if(
            items_.begin(), items_.end(),
            [&id](const T& item) { return item.getId() == id; });
        if (it == items_.end()) {
            throw AppException("Khong tim thay ma: " + id);
        }

        T updated = *it;
        editFunction(updated);
        if (updated.getId() != id) {
            throw AppException("Khong duoc thay doi ma dinh danh.");
        }
        validate(updated);

        T previous = *it;
        *it = updated;
        try {
            save();
        } catch (...) {
            *it = previous;
            throw;
        }
    }

    void remove(const std::string& id) {
        typename std::vector<T>::iterator it = std::find_if(
            items_.begin(), items_.end(),
            [&id](const T& item) { return item.getId() == id; });
        if (it == items_.end()) {
            throw AppException("Khong tim thay ma: " + id);
        }

        T previous = *it;
        const std::size_t index = static_cast<std::size_t>(it - items_.begin());
        items_.erase(it);
        try {
            save();
        } catch (...) {
            items_.insert(items_.begin() + index, previous);
            throw;
        }
    }

    template <typename Compare>
    std::vector<T> sorted(Compare compare) const {
        std::vector<T> result(items_);
        std::sort(result.begin(), result.end(), compare);
        return result;
    }

private:
    std::string filePath_;
    std::vector<T> items_;
    Validator validator_;

    void validate(const T& item) const {
        if (validator_) {
            validator_(item);
        }
    }

    void ensureFileExists() const {
        std::ifstream input(filePath_.c_str());
        if (input.good()) {
            return;
        }
        std::ofstream output(filePath_.c_str(), std::ios::out);
        if (!output) {
            throw AppException("Khong the tao file du lieu: " + filePath_);
        }
    }

    void load() {
        std::ifstream input(filePath_.c_str());
        if (!input) {
            throw AppException("Khong the mo file du lieu: " + filePath_);
        }

        std::string line;
        std::size_t lineNumber = 0;
        while (std::getline(input, line)) {
            ++lineNumber;
            if (line.empty() || line[0] == '#') {
                continue;
            }
            try {
                T item;
                item.deserialize(line);
                if (item.getId().empty() || findById(item.getId()) != 0) {
                    throw AppException("Ma dinh danh rong hoac bi trung.");
                }
                validate(item);
                items_.push_back(item);
            } catch (const std::exception& error) {
                std::cerr << "Canh bao: bo qua dong " << lineNumber
                          << " trong " << filePath_ << ": " << error.what()
                          << '\n';
            }
        }
        if (input.bad()) {
            throw AppException("Loi khi doc file du lieu: " + filePath_);
        }
    }

    void save() const {
        std::ofstream output(filePath_.c_str(), std::ios::out | std::ios::trunc);
        if (!output) {
            throw AppException("Khong the ghi file du lieu: " + filePath_);
        }
        for (typename std::vector<T>::const_iterator it = items_.begin();
             it != items_.end(); ++it) {
            output << it->serialize() << '\n';
        }
        output.flush();
        if (!output) {
            throw AppException("Loi khi ghi file du lieu: " + filePath_);
        }
    }
};

#endif
