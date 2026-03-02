#ifndef __VIRTUAL_MACHINE__HPP
#define __VIRTUAL_MACHINE__HPP

#include <Value.hpp>
#include <Object.hpp>
#include <Runnable.hpp>
#include <GarbageCollector.hpp>

#include <new>
#include <forward_list>


using nython::Runnable;
using nython::kernel::Value;
using nython::kernel::Object;
using nython::gc::GarbageCollector;

namespace nython::vm {

// Enum that defines the result of executing code in the virtual machine
enum class VMResult {
    SUCCESS,        // Exit the VM with success
    COMPILE_ERROR,  // Exit the VM with a compile error
    RUNTIME_ERROR,  // Exit the VM with a runtime error
};


class VirtualMachine extends Runnable {

    friend class gc::GarbageCollector;

    using gc_ptr = std::shared_ptr<GarbageCollector>;

private:
    // The code being executed by the virtual machine
    /// Code* code_;

    // The instruction pointer of the code being executed
    size_t ip_;

    /// Garbage collector to collect and allocate objects
    gc_ptr gc;

public:

    // Constructor
    VirtualMachine(Reporter* reporter);

    // Descructor
    ~VirtualMachine();

    template<typename T>
    T* create() {
        auto cell = gc->allocate();
        T* object = nullptr;
        try{
            object = new T(this);
        }catch(...) {
            gc->collect();
            try{
                object = new T();
            }catch(std::bad_alloc& e){
                throw e;
            }
        }
        cell->value = object;
        return object;
    }

    template<class T,class First,class...Args>
    T* create(First&& first, Args&&... args) {
        auto cell = gc->allocate();
        T* object = nullptr;
        try{
            object = new T(this,std::forward<First&&>(first),std::forward<Args&&>(args)...);
        }catch(...){
            gc->collect();
            try{
                object = new T(this,std::forward<First&&>(first),std::forward<Args&&>(args)...);
            }catch(std::bad_alloc& e){
                throw e;
            }
        }
        cell->value = object;
        return object;
    }

};

using VM = VirtualMachine;

}


#endif // __VIRTUAL_MACHINE__HPP

