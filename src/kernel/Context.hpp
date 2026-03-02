#ifndef __CONTEXT__HPP
#define __CONTEXT__HPP

#include <memory>
#include <Value.hpp>
#include <Object.hpp>
#include <Runnable.hpp>
#include <Container.hpp>

using nython::Runnable;

namespace nython{
namespace kernel{

struct Context extends Container {

    std::string name;
	Collectable* self;
	Collectable* klass;
	Context* parent;

	/// are we in a function or in a class or in a namespace or in a block or in module
    bool inFunction  = false;
    bool inClass     = false;
    bool inNameSpace = false;
    bool inBlock     = false;
    bool inModule    = false;

	Context(Runnable* runner,const std::string& name, Collectable* self = nullptr, Collectable* klass = nullptr, Context* parent = nullptr);
	~Context();

	Context& operator=(Context&& that);

	Context* makeChildContext(const std::string& name);

	Value get(const Value& name);
	void set(const Value& name,Value value);
	void define(const Value& name,Value value);
	Value getAt(int distance, const Value& name);
	void setAt(int distance, const Value &name, Value value);

    bool has(const Value& name);
    bool has(const Value& name,Value* obj);
    bool remove(const Value& name);

    Object* getParent();
    void setParent(Object* parent);


    Context& ancestor(int distance);

    bigint size(){
        return container->size();
    }

    std::string toString(){
        std::stringstream s;
        s << "<Context name = " << name << ">\n";
        s << "</Context>";
        return s.str();
    }

	Value eval(const std::string& code);

	DISALLOW_COPY_AND_ASSIGN(Context);

};

}
}

#endif // __CONTEXT__HPP


