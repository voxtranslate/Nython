#include <sstream>
#include <Value.hpp>
#include <Class.hpp>
#include <except.hpp>
#include <Collectable.hpp>
#include <Collectable.hpp>


using nython::gc::Collectable;

namespace nython{
namespace kernel{

Value::Value() : type{ValueType::NONE},value{} {
}

Value::Value(bool b) : type{ValueType::BOOLEAN},value{b} {
}

Value::Value(int num) : Value(bigint(num)) {
}

Value::Value(uint64_t num) : Value(bigint(num)) {
}

Value::Value(long int num) : Value(bigint(num)) {
}

Value::Value(bigint num) : type{ValueType::INTEGER},value{num} {
}

Value::Value(double num) : Value(ldouble(num)) {
}

Value::Value(long double num) : type{ValueType::DOUBLE},value{num} {
}

Value::Value(Collectable* obj) : type{ValueType::COLLECTABLE},value{obj} {
}

Value::Value(void* p) : type{ValueType::USERDATA},value{p} {
}

Value::Value(const Value& that):type{that.type},value{that.value} {
}

Value::~Value(){
}

Value Value::Undefined(){
	Value v;
	///v.value.gc = state->undefinedObject;
	v.type     = ValueType::UNDEFINED;
	return v;
}

Object* Value::operator->() {
	if(value.gc==nullptr) return (Object*)getClass();
	else if(!value.gc->isClass()){
        return value.gc->getClass();
	}
	return (Object*)value.gc;
}

void Value::SetNone(){
	value.gc = nullptr;
	type     = ValueType::NONE;
}

void Value::SetBoolean(bool b){
	value.b = b;
	type    = ValueType::BOOLEAN;
}

Class* Value::getClass(){
	if(isCollectable()) return value.gc->getClass();
	Class* clazz = nullptr;
	/*switch(type){
		case ValueType::NONE:
			clazz = state->noneObject->data->clazz;
		break;
		case ValueType::DOUBLE:
			clazz = state->getClass("Float");
		break;
		case ValueType::BOOLEAN:
			if(value.b) clazz = state->trueObject->data->clazz;
			else clazz = state->falseObject->data->clazz;
		break;
		case ValueType::INTEGER:
			clazz = state->getClass("Integer");
		break;
		case ValueType::UNDEFINED:
			clazz = state->undefinedObject->data->clazz;
		break;
		default:
		break;
	}    */
	return clazz;
}

Value Value::call(std::vector<Value> args){
	return NONE_VALUE;
}

Value Value::call(const std::string& name,std::vector<Value> args){
	return NONE_VALUE;
}

uint64_t Value::hash(){
	if(isCollectable()) return this->hash();
	return (uint64_t)std::__hash__(*this);
}

bigint Value::size(){
	if(isCollectable()) return this->size();
	bigint val;
	switch(type){
		case ValueType::DOUBLE:
			val = sizeof(value.d);
		break;
		case ValueType::BOOLEAN:
			val = sizeof(value.b);
		break;
		case ValueType::INTEGER:
			val = sizeof(value.i);
		break;
		case ValueType::COLLECTABLE:
		case ValueType::NONE:
			val = sizeof(value.gc);
		break;
		default:
			val = sizeof(value.p);
		break;
	}
	return val;
}

std::string Value::getName(){
	if(isCollectable()) return value.gc->toString();
	std::string name;
	switch(type){
		case ValueType::NONE:
			name = "none";
		break;
		case ValueType::DOUBLE:
			name = std::to_string(value.d);
		break;
		case ValueType::BOOLEAN:
			if(value.b) name = "true";
			else name = "false";
		break;
		case ValueType::INTEGER:
			name = value.i.toString();
		break;
		default:
		break;
	}
	return name;
}

int Value::sizeOf(){
	if(isCollectable()) return this->sizeOf();
	int sz = 1;
	switch(type){
		case ValueType::NONE:
			sz = sizeof(Collectable);
		break;
		case ValueType::DOUBLE:
			sz = sizeof(value.d);
		break;
		case ValueType::BOOLEAN:
			sz = sizeof(value.b);
		break;
		case ValueType::INTEGER:
			sz = sizeof(value.i);
		break;
		default:
			sz = sizeof(value.p);
		break;
	}
	return sz;
}

bool Value::isString(){
	return isCollectable() && value.gc->getType()==Type::STRING;
}

bool Value::isFunction(){
	return isCollectable() && value.gc->getType()==Type::FUNCTION;
}

bool Value::isPrimitive(){
	return isCollectable() && value.gc->getType()==Type::NATIVE;
}

bool Value::isMethod(){
	return isCollectable() && value.gc->getType()==Type::METHOD;
}

bool Value::isClass(){
	return isCollectable() && value.gc->getType()==Type::CLASS;
}

bool Value::isList(){
	return isCollectable() && value.gc->getType()==Type::LIST;
}

bool Value::isTuple(){
	return isCollectable() && value.gc->getType()==Type::TUPLE;
}

bool Value::isArray(){
	return isCollectable() && value.gc->getType()==Type::ARRAY;
}

bool Value::isSet(){
	return isCollectable() && value.gc->getType()==Type::SET;
}

bool Value::isMap(){
	return isCollectable() && value.gc->getType()==Type::MAP;
}

bool Value::isComplex(){
	return isCollectable() && value.gc->getType()==Type::COMPLEX;
}

bool Value::isRational(){
	return isCollectable() && value.gc->getType()==Type::RATIONAL;
}

Object* Value::toObject() {
    return (Object*)value.gc;
}

static const char* _typeName(Value v){
    if(v.isCollectable()){
        return v.value.gc->typeName().c_str();
	}else switch(v.type){
		case ValueType::NONE: return "none";
		case ValueType::BOOLEAN: return "boolean";
		case ValueType::DOUBLE: return "double";
		case ValueType::INTEGER: return "integer";
		case ValueType::USERDATA:  return "user-data";
		default: return "unknown type";
	}
	return "unknown type";
}


const char* Value::typeName() const{
	return _typeName(*this);
}

template<class T>
inline std::string _format(T value){
    std::stringstream str;
    str<<value;
    return str.str();
}

std::string Value::format(){
	switch(type){
		case ValueType::NONE: return "none";
		case ValueType::USERDATA:  return "user-data";
		case ValueType::BOOLEAN: return value.b?"true":"false";
		case ValueType::DOUBLE:  return _format(value.d);
		case ValueType::INTEGER: return _format(value.i);
		case ValueType::COLLECTABLE:  return value.gc->toString();
		case ValueType::UNDEFINED:  return "undefined";
		default: return "unknown";
	}
	return "unknown";
}

std::string _toString(Value* that){
	switch(that->type){
		case ValueType::NONE: return "none";
		case ValueType::USERDATA:  return "user-data";
		case ValueType::BOOLEAN: return that->value.b?"true":"false";
		case ValueType::DOUBLE: return _format(that->value.d);
		case ValueType::INTEGER: return _format(that->value.i);
		case ValueType::UNDEFINED: return "undefined";
		case ValueType::COLLECTABLE:
			if(that->value.gc==nullptr) return "none";
		return that->value.gc->toString();
		default: return "unknown";
	}
	return "unknown";
}

bool Value::equals(Value that){
	if(type!=that.type){
		if(type==ValueType::DOUBLE&&that.type==ValueType::INTEGER){
			return value.d  == that.value.i;
		}else if(that.type==ValueType::DOUBLE&&type==ValueType::INTEGER){
			return that.value.d  == value.i;
		}else if(that.type==ValueType::BOOLEAN&&type==ValueType::INTEGER){
			return that.value.b  == value.i;
		}else if(type==ValueType::BOOLEAN&&that.type==ValueType::INTEGER){
			return value.b  == that.value.i;
		}else if(type==ValueType::BOOLEAN&&that.type==ValueType::DOUBLE){
			return value.b  == that.value.d;
		}else if(that.type==ValueType::BOOLEAN&&type==ValueType::DOUBLE){
			return that.value.b  == value.d;
		}else if(that.type==ValueType::COLLECTABLE&&type==ValueType::DOUBLE){
			return that.value.gc->toString() == _format(value.d);
		}else if(type==ValueType::COLLECTABLE&&that.type==ValueType::DOUBLE){
			return value.gc->toString() == _format(that.value.d);
		}else if(that.type==ValueType::COLLECTABLE&&type==ValueType::INTEGER){
			return that.value.gc->toString() == _format(value.i);
		}else if(type==ValueType::COLLECTABLE&&that.type==ValueType::INTEGER){
			return value.gc->toString() == _format(that.value.i);
		}else if(that.type==ValueType::COLLECTABLE&&type==ValueType::BOOLEAN){
			return that.value.gc->toString() == _format(value.b);
		}else if(type==ValueType::COLLECTABLE&&that.type==ValueType::BOOLEAN){
			return value.gc->toString() == _format(that.value.b);
		}
	}
	if(type != that.type) return false;
    switch(type){
		case ValueType::NONE:
		case ValueType::BOOLEAN:     return value.b  == that.value.b;
		case ValueType::DOUBLE:      return value.d  == that.value.d;
		case ValueType::INTEGER:     return value.i  == that.value.i;
		case ValueType::USERDATA:    return value.p  == that.value.p;
		case ValueType::UNDEFINED:   return value.gc  == that.value.gc;
		case ValueType::COLLECTABLE: return (value.gc == that.value.gc) or (value.gc!=nullptr && value.gc->equals(that.value.gc));
		default:                     return false;
    }
	return false;
}

std::string Value::toString(){
	return _toString(this);
}

std::string Value::toString() const{
	return _toString((Value*)this);
}

std::ostream& operator<<(std::ostream& os,Value& that){
    os<<that.toString();
    return os;
}

std::ostream& operator<<(std::ostream& os,const Value& that){
    os<<that.toString();
    return os;
}

}
} // namespace nython

namespace std {

uint64_t __hash__(const nython::kernel::Value &t){
	switch(t.type){
		case nython::kernel::ValueType::NONE:
		return std::hash<int>()(0);
		case nython::kernel::ValueType::BOOLEAN:
		return std::hash<bool>()(t.value.b);
		case nython::kernel::ValueType::INTEGER:
		return std::hash<nython::kernel::bigint>()(t.value.i);
		case nython::kernel::ValueType::DOUBLE:
		return std::hash<long double>()(t.value.d);
		case nython::kernel::ValueType::UNDEFINED:
		case nython::kernel::ValueType::COLLECTABLE:
		return std::hash<void*>()(t.value.gc);
		case nython::kernel::ValueType::USERDATA:
		default:
		return std::hash<void*>()(t.value.p);
	}
	return std::hash<int>()(-1);
}

}

