#ifndef ENTITY_H
#define ENTITY_H

#include <string>

class Entity {
public:
    virtual ~Entity() {}

    const std::string& getId() const { return id_; }
    void setId(const std::string& id) { id_ = id; }

    virtual void input() = 0;
    virtual void display() const = 0;
    virtual std::string serialize() const = 0;
    virtual void deserialize(const std::string& line) = 0;

private:
    std::string id_;
};

#endif
