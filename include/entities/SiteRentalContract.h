#ifndef SITE_RENTAL_CONTRACT_H
#define SITE_RENTAL_CONTRACT_H

#include "../common/Entity.h"

#include <string>

class SiteRentalContract : public Entity {
public:
    SiteRentalContract();
    SiteRentalContract(const std::string& id, const std::string& stationId,
                       const std::string& lessorName,
                       const std::string& lessorPhone,
                       const std::string& startDate,
                       const std::string& endDate, double monthlyRent,
                       const std::string& paymentCycle,
                       const std::string& status);

    const std::string& getStationId() const;
    const std::string& getLessorName() const;
    const std::string& getLessorPhone() const;
    const std::string& getStartDate() const;
    const std::string& getEndDate() const;
    double getMonthlyRent() const;
    const std::string& getPaymentCycle() const;
    const std::string& getStatus() const;

    void setStationId(const std::string& value);
    void setLessorName(const std::string& value);
    void setLessorPhone(const std::string& value);
    void setStartDate(const std::string& value);
    void setEndDate(const std::string& value);
    void setMonthlyRent(double value);
    void setPaymentCycle(const std::string& value);
    void setStatus(const std::string& value);

    void input();
    void display() const;
    std::string serialize() const;
    void deserialize(const std::string& line);

    static void displayHeader();

private:
    std::string stationId_;
    std::string lessorName_;
    std::string lessorPhone_;
    std::string startDate_;
    std::string endDate_;
    double monthlyRent_;
    std::string paymentCycle_;
    std::string status_;

    static void validateField(const std::string& value,
                              const std::string& fieldName);
    static void validateId(const std::string& id);
    static void validateStationId(const std::string& id);
};

#endif
