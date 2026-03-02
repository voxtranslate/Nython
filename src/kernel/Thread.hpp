#pragma once

#include <vector>
#include <thread>
#include <Value.hpp>
#include <Object.hpp>

using nython::interpreter::Interpreter;

namespace nython {
namespace kernel {
// Represents a worker thread started by the VM
struct Thread extends Object {

    CFunction cfunc;
    Function* callback;
    Value return_value;
    std::vector<Value> arguments;
    std::thread thread;

    Thread(Runnable* runner,CFunction _cf, const std::vector<Value>& _args, Function* _cb) :Object(runner,name,Type::THREAD), cfunc(_cf), callback(_cb), return_value{}, arguments(_args) {
    }

    ~Thread() {
        if (std::this_thread::get_id() == this->thread.get_id()) {
            this->thread.detach();
        } else {
            this->thread.join();
        }
    }

    void detach() {
        if (std::this_thread::get_id() == this->thread.get_id()) {
            this->thread.detach();
        }
    }

    void join() {
        if (this->thread.joinable()) this->thread.join();
    }

    Value call(Runnable* runner);

    Value operator()(Runnable* runner) {
        return call(runner);
    }

    Value getValue() {
        return this->return_value;
    }
};
}
}
