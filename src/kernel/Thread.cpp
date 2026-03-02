#include <Thread.hpp>
#include <Interpreter.hpp>



namespace nython {
namespace kernel {
Value Thread::call(Runnable* runner) {
    this->thread = std::thread([this, &runner]() {
        // Invoke c function
        this->return_value = this->cfunc(runner, this->arguments);
        // Return our value to the VM
    });
    return this->return_value;
}
}
}
