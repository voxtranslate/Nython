#ifndef __VALUE__HPP
#define __VALUE__HPP

#include <vector>
#include <memory>
#include <Type.hpp>
#include <Token.hpp>
#include <bigint.hpp>
#include <Modifier.hpp>
#include <Collectable.hpp>

#define NONE_VALUE  	((Value()))
#define TRUE_VALUE  	((Value(true)))
#define FALSE_VALUE 	((Value(false)))
#define UNDEFINED_VALUE ((Value::Undefined()))


#ifndef __ldouble__
#define __ldouble__
typedef long double ldouble;
#endif // __ldouble__

namespace nython {
namespace interpreter {
struct Interpreter;
}

namespace vm {
struct VirtualMachine;
}
struct Runnable;
}

using nython::Runnable;
using nython::lexer::Token;
using nython::vm::VirtualMachine;
using nython::interpreter::Interpreter;

namespace nython{
namespace gc {
class Collectable;
}

using gc::Collectable;

namespace kernel{

struct Object;
struct Class;
struct Value;

/*
** Type for C functions registered with Nool
*/
#ifndef __cfunction__
#define __cfunction__
typedef Value (*CFunction) (Runnable* runner, std::vector<Value>& args);
#endif //__cfunction__


/*
** Union of all Nool Values
*/
typedef struct TValue {

	Collectable* gc;      /* collectable object */
	void* p;         /* light user data */
	bool b;          /* boolean*/
	bigint i;       /* integer numbers */
	long double d;  /* float numbers */

	explicit TValue():gc{},p{},b{},i{},d{} {
	}

	explicit TValue(bool b):gc{},p{},b{b},i{},d{} {
	}

	explicit TValue(int i):gc{},p{},b{},i{i},d{} {
	}

	explicit TValue(bigint i):gc{},p{},b{},i{i},d{} {
	}

	explicit TValue(uint64_t i):gc{},p{},b{},i{i},d{} {
	}

	explicit TValue(long int i):gc{},p{},b{},i{i},d{} {
	}

	explicit TValue(double d):gc{},p{},b{},i{},d{d} {
	}

	explicit TValue(long double d):gc{},p{},b{},i{},d{d} {
	}

	explicit TValue(void* p):gc{},p{p},b{},i{},d{} {
	}

	explicit TValue(Collectable* gc):gc{gc},p{},b{},i{},d{} {
	}

	TValue(const TValue& that):gc{that.gc},p{that.p},b{that.b},i{that.i},d{that.d} {
	}

	~TValue(){
	}

	TValue& operator=(const TValue& that){
		if(this!=&that){
			gc = that.gc;
			p  = that.p;
			b  = that.b;
			i  = that.i;
			d  = that.d;
		}
		return *this;
	}

}TValue;

enum class ValueType: uint8_t  {
    // Types which are immediate encoded using nan-boxing
    NONE,
    BOOLEAN,
    INTEGER,
    DOUBLE,
    UNDEFINED,
    // Types which are allocated on the heap
    COLLECTABLE,
    // This should never appear anywhere
    USERDATA
};

enum class ValueKind: uint8_t  {
    _CONST,
    VAR,
    LET
};


struct Value {

    int  access;
    ValueKind kind;
	ValueType type;
	TValue    value;
	Token     token;

	explicit Value();
	explicit Value(bool b);
	explicit Value(int num);
	explicit Value(bigint num);
	explicit Value(uint64_t num);
	explicit Value(long int num);
	explicit Value(double num);
	explicit Value(long double num);
	explicit Value(Collectable* c);
	explicit Value(void* p);
	Value(const Value& that);
	~Value();

	Value& operator=(const Value& that){
		if(this!=&that){
			type   = that.type;
			value  = that.value;
			access = that.access;
			kind   = that.kind;
		}
		return *this;
	}

	void SetNone();
	void SetBoolean(bool b);
	void SetModifier(int mod);
	void SetValueKind(ValueKind kind);

	bool isNone() {
		return is(ValueType::NONE);
	}

	bool isFalse() {
		return isNone() || (isBoolean() && !value.b);
	}

	bool isTrue() {
		return isBoolean() && value.b;
	}

	bool isBoolean(){
		return is(ValueType::BOOLEAN);
	}

	bool isCollectable(){
		return is(ValueType::COLLECTABLE) or value.gc!=nullptr;
	}

	bool isObject() {
        return isCollectable();
	}

	bool isUserData(){
		return is(ValueType::USERDATA);
	}

	bool isFloat(){
		return is(ValueType::DOUBLE);
	}

	bool isDouble(){
		return is(ValueType::DOUBLE);
	}

	bool isNull() {
        return is(ValueType::COLLECTABLE) && value.gc==nullptr;
	}

	bool isInteger(){
		return is(ValueType::INTEGER);
	}

	bool isLiteral(){
		return isNumber() || isBoolean() || isString() || isNone();
	}

	bool isNumber() {
        return isInteger() or isDouble();
	}

	bool isUndefined(){
        return is(ValueType::UNDEFINED);
	}

	bool isVariable(){
        return is(ValueKind::VAR);
	}

	bool isConstant(){
        return is(ValueKind::_CONST);
	}

	bool isLet(){
        return is(ValueKind::LET);
	}

	bool isPublic(){
        return Modifier.isPublic(access);
    }

    bool isPrivate() {
        return Modifier.isPrivate(access);
    }

    bool isProtected() {
        return Modifier.isProtected(access);
    }

    bool isAbstract() {
        return Modifier.isAbstract(access);
    }

    bool isStatic() {
        return Modifier.isStatic(access);
    }

    bool isFinal() {
        return Modifier.isFinal(access) || isConstant();
    }

    bool isSynchronized() {
        return Modifier.isSynchronized(access);
    }

    bool isRef() {
        return Modifier.isRef(access);
    }

    bool isImmutable() {
        return Modifier.isImmutable(access);
    }

    bool isMutable() {
        return Modifier.isMutable(access);
    }

    bool isVarArgs(){
        return Modifier.isVarArgs(access);
    }

    bool isAnnotation() {
      return Modifier.isAnnotation(access);
    }

    static const int classModifiers() {
        return Modifier.classModifiers();
    }

    static int accessModifiers() {
        return Modifier.accessModifiers();
    }

    static const int interfaceModifiers() {
        return Modifier.interfaceModifiers();
    }

    static const int constructorModifiers() {
        return Modifier.constructorModifiers();
    }

    static const int methodModifiers() {
        return Modifier.methodModifiers();
    }

    static const int functionModifiers() {
        return Modifier.functionModifiers();
    }

    static const int fieldModifiers() {
        return Modifier.fieldModifiers();
    }

    static const int parameterModifiers() {
        return Modifier.parameterModifiers();
    }

    static Value Undefined();

    bool isEnum();
	bool isString();
	bool isFunction();
	bool isPrimitive();
	bool isInterface();
	bool isLambda();
	bool isClass();
	bool isList();
	bool isTuple();
	bool isArray();
	bool isSet();
	bool isMap();
	bool isComplex();
	bool isRational();
	bool isMethod();

	bool is(ValueType type){
		return this->type == type;
	}

	bool is(ValueKind kind){
		return this->kind == kind;
	}

	Object* operator->();

	Object* toObject();

	template<typename T>
	T* as();

	uint64_t id() const{
        return reinterpret_cast<int64_t>(this);
	}

	bool operator < (const Value& that) {
        return id() < that.id();
	}

	bool operator > (const Value& that) {
        return id() > that.id();
	}

	bool operator <= (const Value& that) {
        return id() <= that.id();
	}

	bool operator >= (const Value& that) {
        return id() >= that.id();
	}

	friend bool operator < (Value a, Value b){
        return a.id() < b.id();
	}

	friend bool operator > (Value a, Value b) {
        return a.id() > b.id();
	}

	friend bool operator <= (Value a, Value b) {
        return a.id() <= b.id();
	}

	friend bool operator >= (Value a, Value b) {
        return a.id() >= b.id();
	}

	operator Collectable*(){
		return value.gc;
	}

	operator std::string(){
		return toString();
	}

	operator bool(){
		return toString().size()?true:false;
	}

    operator int(){
    	if(isInteger()){
			return value.i;
    	}
    	if(isDouble()){
			return value.d;
    	}
		return atoi(toString().c_str());
	}

    operator int64_t(){
		if(isInteger()){
			return value.i;
    	}
    	if(isDouble()){
			return value.d;
    	}
		return atoi(toString().c_str());
	}

    operator bigint(){
		if(isInteger()){
			return value.i;
    	}
    	if(isDouble()){
			return value.d;
    	}
		return atoi(toString().c_str());
	}

    operator float(){
		if(isInteger()){
			return value.i;
    	}
    	if(isDouble()){
			return value.d;
    	}
		return atof(toString().c_str());
	}

    operator double(){
		if(isInteger()){
			return value.i;
    	}
    	if(isDouble()){
			return value.d;
    	}
		return atof(toString().c_str());
	}

    operator ldouble(){
		if(isInteger()){
			return value.i;
    	}
    	if(isDouble()){
			return value.d;
    	}
		return atof(toString().c_str());
	}

    operator char*(){
		return (char*)(toString().c_str());
	}

	bool equals(Value that);

	bool operator==(const Value& that){
		return equals(that);
	}

	bool operator!=(const Value& that){
		return !equals(that);
	}

	const char* typeName() const;
	Value call(std::vector<Value> args);
	Value call(const std::string& name,std::vector<Value> args);
	Value operator+(Value that);
	Value operator-(Value that);
	Value operator*(Value that);
	Value operator/(Value that);
	Value operator%(Value that);
	Value operator~(void);

	Value addNone(Value that);
	Value addBoolean(Value that);
	Value addInteger(Value that);
	Value addDouble(Value that);
	Value addCollectable(Value that);

	Value remNone(Value that);
	Value remBoolean(Value that);
	Value remInteger(Value that);
	Value remDouble(Value that);
	Value remCollectable(Value that);

	Value mulNone(Value that);
	Value mulBoolean(Value that);
	Value mulInteger(Value that);
	Value mulDouble(Value that);
	Value mulCollectable(Value that);

	Value divNone(Value that);
	Value divBoolean(Value that);
	Value divInteger(Value that);
	Value divDouble(Value that);
	Value divCollectable(Value that);

	Value modNone(Value that);
	Value modBoolean(Value that);
	Value modInteger(Value that);
	Value modDouble(Value that);
	Value modCollectable(Value that);

	Value comBoolean();
	Value comInteger();
	Value comDouble();
	Value comCollectable();

	int sizeOf();
	Class* getClass();
	uint64_t hash();
	bigint size();
	std::string getName();
	std::string format();
	std::string modifiers();
	std::string toString();
	std::string toString() const;
	friend std::ostream& operator<<(std::ostream& os,Value& that);
	friend std::ostream& operator<<(std::ostream& os,const Value& that);

};

inline bool operator == (const Value &left, const Value &right){
	if(left.type != right.type) return false;
	switch(left.type){
		case ValueType::NONE      : return true;
		case ValueType::BOOLEAN   : return left.value.b  == right.value.b;
		case ValueType::INTEGER   : return left.value.i  == right.value.i;
		case ValueType::DOUBLE    : return left.value.d  == right.value.d;
		case ValueType::COLLECTABLE    : return left.value.gc == right.value.gc;
		case ValueType::USERDATA  : return left.value.p  == right.value.p;
		default:
		break;
	}
	return false;
}

inline bool operator != (const Value &left, const Value &right){
	return !(left == right);
}

}///kernel
}///nython

namespace std{

static uint32_t hashString(const char* key, int length) {
	uint32_t hash = 2166136261u;
	for(auto i = 0; i < length; i++){
		hash ^= key[i];
		hash *= 16777619;
	}
	return hash;
}

template<>
struct hash<nython::kernel::bigint>	: public unary_function<nython::kernel::Value, size_t>{
	size_t operator () (const nython::kernel::bigint &t) const{
		std::string str = t.toString();
		return hashString(str.c_str(),str.size());
	}
};

uint64_t __hash__(const nython::kernel::Value &t);

template<>
struct hash<nython::kernel::Value> : public unary_function<nython::kernel::Value, size_t>{
	size_t operator () (const nython::kernel::Value &t) const{
		return __hash__(t);
	}
};

}

inline bool is_ptr(nython::kernel::Value v){
    return v.type == nython::kernel::ValueType::COLLECTABLE && v.value.gc != nullptr;
}

template<typename T>
inline T* as(nython::kernel::Value p) {
    return reinterpret_cast<T*>(p.value.gc);
}

inline nython::gc::Collectable* as_collectable(nython::kernel::Value value) {
    return reinterpret_cast<nython::gc::Collectable*>(value.value.gc);
}

inline bool is_dead(nython::kernel::Value value){
    nython::gc::Collectable* gc = as_collectable(value);
    return gc && gc->getType() == Type::DEAD;
}

inline bool is_dead(nython::gc::Collectable* gc){
    return gc && gc->getType() == Type::DEAD;
}

#endif // __VALUE__HPP

