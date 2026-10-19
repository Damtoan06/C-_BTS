#ifndef TECHNICIAN_H
#define TECHNICIAN_H

#include "../common/Entity.h"

#include <string>

class Technician : public Entity {
public:
    Technician();
    Technician(const std::string& id, const std::string& fullName,
               const std::string& phoneNumber,
               const std::string& specialization,
               const std::string& certLevel, const std::string& status);

    const std::string& getFullName() const;
    const std::string& getPhoneNumber() const;
    const std::string& getSpecialization() const;
    const std::string& getCertLevel() const;
    const std::string& getStatus() const;

    void setFullName(const std::string& value);
    void setPhoneNumber(const std::string& value);
    void setSpecialization(const std::string& value);
    void setCertLevel(const std::string& value);
    void setStatus(const std::string& value);

    void input();
    void display() const;
    std::string serialize() const;
    void deserialize(const std::string& line);

    static void displayHeader();

private:
    std::string fullName_;
    std::string phoneNumber_;
    std::string specialization_;
    std::string certLevel_;
    std::string status_;

    static void validateField(const std::string& value,
                              const std::string& fieldName);
    static void validateId(const std::string& id);
};

#endif
