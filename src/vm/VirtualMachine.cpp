#include <VirtualMachine.hpp>

namespace nython::vm {

    // Constructor
    VM::VirtualMachine(Reporter* reporter) : Runnable(reporter, RunnableType::COMPILER), gc{new GarbageCollector(this)} {
    }

    // Destructor
    VM::~VirtualMachine() {
    }

}
