#ifndef __CONTAINER__HPP
#define __CONTAINER__HPP

#include <Type.hpp>
#include <Value.hpp>
#include <functional>
#include <unordered_map>
#include <Location.hpp>
#include <Collectable.hpp>

namespace nython::kernel {

using gc::Collectable;
using lexer::Location;

// Underlying type of every Value which has its own
// Value container (e.g. Object, Function, Class)
using ContainerType = std::unordered_map<std::string, Value>;
class Container: public Collectable {

public:
    Location location;
    ContainerType* container;

public:

    Container(Type type, uint32_t initial_capacity = 4);
    Container(Runnable* runner, Type type, uint32_t initial_capacity = 4);
    ~Container();
    void clean();
    void copy_container_from(const Container* other);

    std::string toString();
    bool read(Value key, Value* result);              // reads a Value from the container
    Value read_or(Value key, Value fallback = NONE_VALUE); // reads a Value from the container or returns fall-back
    bool contains(Value key);                         // check whether the container contains some key
    int size();                                       // returns amount of keys inside container
    bool erase(Value key);                            // erases a key from the container, returns true on success
    void write(Value key, Value value);               // insert or assign to some key
    bool assign(Value key, Value value);              // assign to some key, returns false if key did not exist
    ContainerType::iterator find(const Value& value, bool* ok); // find if value is inside the container or not

    // Access the internal container data structure via a callback function
    void access_container(std::function<void(ContainerType*)> cb);
    void access_container_shared(std::function<void(ContainerType*)> cb);

};

}///nython

#endif // __CONTAINER__HPP


