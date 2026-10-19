#ifndef STATION_H
#define STATION_H

#include "../common/Entity.h"

#include <string>

class Station : public Entity {
public:
    Station();
    Station(const std::string& id, const std::string& name,
            const std::string& location, const std::string& status);

    const std::string& getName() const;
    const std::string& getLocation() const;
    const std::string& getStatus() const;

    void setName(const std::string& value);
    void setLocation(const std::string& value);
    void setStatus(const std::string& value);

    void input();
    void display() const;
    std::string serialize() const;
    void deserialize(const std::string& line);

private:
    std::string name_;
    std::string location_;
    std::string status_;

    static void validateId(const std::string& id);
    static void validateField(const std::string& value,
                              const std::string& fieldName);
};

#endif
