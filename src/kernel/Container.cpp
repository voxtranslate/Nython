#include <sstream>
#include <Object.hpp>
#include <Container.hpp>
#include <Collectable.hpp>

namespace nython {
namespace kernel {

Container::Container(Type type, uint32_t initial_capacity): Collectable(type) {
    this->container = new ContainerType();
    this->container->reserve(initial_capacity);
}

Container::Container(Runnable* runner, Type type, uint32_t initial_capacity): Collectable(runner,type) {
    this->container = new ContainerType();
    this->container->reserve(initial_capacity);
}

Container::~Container() {
}

std::string Container::toString() {
    std::stringstream ret;
    std::hex(ret);
    ret << this;
    return std::string("< container object at ").append(ret.str()).append(" of type ").append(TypeToString(type)).append(">");
}

void Container::copy_container_from(const Container* other) {
  if(!this->container) return;
  this->container->insert(other->container->begin(), other->container->end());
}

bool Container::read(Value key, Value* result) {
  if(!this->container) {
     result = nullptr;
     return false;
  }
  if(this->container->count(key) == 0) return false;
  *result = (* this->container)[key];
  return true;
}

Value Container::read_or(Value key, Value fallback) {
  if(!this->container) return fallback;
  if (this->container->count(key) == 1) {
    return (* this->container)[key];
  }
  return fallback;
}

bool Container::contains(Value key) {
  if(!this->container) return false;
  return this->container->count(key);
}

int Container::size() {
  if(!this->container) return -1;
  return this->container->size();
}

bool Container::erase(Value key) {
  if(!this->container) return false;
  return this->container->erase(key) > 0;
}

void Container::write(Value key, Value value) {
  if(!this->container) return;
  (* this->container)[key] = value;
}

bool Container::assign(Value key, Value value) {
  if(!this->container) return false;
  if(this->container->count(key) == 0) return false;
  this->container->insert({key, value});
  return true;
}

void Container::access_container(std::function<void(ContainerType*)> cb) {
  if(!this->container) return;
  cb(this->container);
}

void Container::access_container_shared(std::function<void(ContainerType*)> cb) {
  if(!this->container) return;
  cb(this->container);
}

void Container::clean() {
  Collectable::clean();
  if(this->container) {
    for(auto& it: *container) {
        Value second = it.second;
        if(second.isCollectable()) {
            second->clean();
        }
    }
    delete this->container;
    this->container = nullptr;
  }
}

ContainerType::iterator Container::find(const Value& value, bool* ok) {
    const std::string name = Value(value).getName();
    ContainerType::iterator it = this->container->find(name);
    if(it!=this->container->end()) {
        *ok = true;
    } else *ok = false;
    return it;
}

}
}

