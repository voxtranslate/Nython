#ifndef __PARSER__HPP
#define __PARSER__HPP

#include <map>
#include <vector>
#include <IParser.hpp>

using nython::node::Node;
using nython::lexer::Token;
using nython::lexer::Lexer;
using nython::node::node_ptr;
using nython::lexer::TokenKind;
using nython::lexer::TokenType;
using nython::lexer::token_list;
using nython::exception::SyntaxError;


namespace nython {
namespace lexer{
extern std::map<TokenType,std::string> TokenTypeNames;
}
}

namespace nython::parser {

using nython::lexer::TokenTypeNames;

/// class that defines a parser that converts a list of tokens into a list of nodes
class Parser extends IParser {

private:

    /// Reference to the virtual machine or the interpreter
    Runnable* runner;

    /** The lexical analyzer with which tokens are scanned. */
	Lexer* scanner;

	inline Token next(){
        return scanner->next();
    }

    inline Token prev(){
        return scanner->prev();
    }

    inline Token peek(int pos = 1){
        return scanner->peek(pos);
    }

    inline Token token(){
        return scanner->curr();
    }

    inline int line(){
		return token().line();
    }

    inline int column(){
		return token().column();
    }

    inline std::string value(){
		return token().value;
    }

    inline std::string fileName(){
		return token().fileName();
    }

    inline std::string code(){
        return scanner->code();
    }

    inline bool atEnd() {
        return scanner->atEnd() or next().type() == TokenType::End;
    }

    inline bool match(TokenType type) {
        return have(type);
    }

    inline bool match(const std::initializer_list<TokenType> types) {
        for (auto type : types) {
          if (see(type)) {
            next();
            return true;
          }
        }
        return false;
    }

    inline bool see(TokenType type){
        return token().type() == type;
    }

    inline bool have(TokenType type){
        if(see(type)){
            next();
            return true;
        }
        return false;
    }

    /**
     *
     * @param saw the token we're looking for.
	*/
    inline void mustBe(TokenType type, std::string context = ""){
        if(see(type)){
            next();
            return;
        } else if (!context.empty())
            throw report<SyntaxError>(next().location(), fmt::format("Expected {} {}, but found {}", type, context, next().type()));
        else
            throw report<SyntaxError>(next().location(), fmt::format("Expected {}, but found {}", type, next().type()));
    }

    inline Token consume(TokenType type, std::string context = ""){
        if(see(type)){
            return next();
        } else if (!context.empty())
            throw report<SyntaxError>(next().location(), fmt::format("Expected {} {}, but found {}", type, context, next().type()));
        else
            throw report<SyntaxError>(next().location(), fmt::format("Expected {}, but found {}", type, next().type()));
    }

    // Synchronize the parser after an error
    inline void synchronize() {
        // Advance the parser until it reaches an expression boundary
        next();
        while (!atEnd()) {
            // Check for a newline token
            if (token().type() == TokenType::NewLine) break;
            // Advance the parser
            next();
        }
    }

    std::string identifier();
    std::string dottedName();

    bool isCompoundStatement();
    bool isFlowStatement();
    bool isImportStatement();
    bool isAugAssign();
    bool isOperator();
    bool isClosing();
    bool isOperatorCall();
    bool isComparison();
    bool isTrailer();
    bool isYield();
    bool isModifier();
    bool isKeyWord();

    /// The script it self
    node_ptr script();
    node_ptr stmt();

public:

    // Constructor
    Parser(Reporter* reporter, Runnable* runner, Lexer* lexer);
    node_ptr parse();

};

}



#endif // __PARSER__HPP
