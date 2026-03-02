#ifndef __COLLECTABLE__HPP
#define __COLLECTABLE__HPP

#include <string>
#include <Type.hpp>
#include <Runnable.hpp>
#include <Definitions.hpp>


using nython::kernel::Type;
using nython::kernel::Class;
using nython::kernel::Object;

namespace nython::gc {

// Metadata which is stores in every heap Value
class Collectable {

protected:
    Type  type;
    bool marked;   // set by the GC to mark reachable Values
    Runnable* runner;

public:

    Collectable(Type type);
    Collectable(Runnable* runner, Type type);
    virtual ~Collectable();

    virtual void clean();
    virtual std::string toString();
    virtual std::string typeName();

    virtual std::string getName() {
        return "";
    }

    virtual std::string shortName(){
        return "";
    }

    virtual std::string address(){
        return "";
    }

    virtual std::string qualifiedName(){
        return "";
    }

    virtual Type getType();
    virtual void setType(Type type);
    virtual uint64_t id();
    virtual Object* getParent();
    virtual Class* getClass();
    virtual bool equals(Collectable* that);
    virtual bool isClass();

    bool get_gc_mark();
    void set_gc_mark();
    void clear_gc_mark();

    template<typename T>
    T* as();

};
}


#endif // __COLLECTABLE__HPP

