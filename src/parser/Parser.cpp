#include <Parser.hpp>

#include <Script.hpp>

using nython::node::Script;


namespace nython::parser {

std::vector<std::string> operators = {
    ">>>=",">>>","===","!==","**=","&&=","||=","^^=","<<=",">>=","**","++","--","&&","||","^^",
    "<<",">>",">=","<=","==","!=","+=","-=","*=","/=","\\=","%=","~=","&=","|=","^=",">","<","&",
    "|","^","~","%","+","-","*","/","\\","!","=",
    "and","or","xor","not","sizeof","typeof","not in","in","is","is not","print"
};

std::vector<std::string> binaries = {
    ">>>","===","!==","**","&&","||","^^",
    "<<",">>",">=","<=","==","!=",">","<","&",
    "|","^","~","%","+","-","*","/","\\",
    "and","or","xor","not","not in","in","is","is not"
};


Parser::Parser(Reporter* reporter, Runnable* runner, Lexer* lexer): IParser(reporter), runner{runner}, scanner{lexer} {
}


node_ptr Parser::parse() {
    // Parse the script
    try {
        return script();
    } catch (SyntaxError& ex) {
        throw ex;
    }
}

std::string Parser::dottedName(){
	mustBe(TokenType::Identifier);
	std::string name = prev().value;
	while(have(TokenType::Dot)){
		mustBe(TokenType::Identifier);
		name += "."+prev().value;
	}
	return name;
}

std::string Parser::identifier(){
    mustBe(TokenType::Identifier);
    return prev().value;
}


/**
* break_stmt | continue_stmt | return_stmt | raise_stmt | yield_stmt
**/
bool Parser::isFlowStatement(){
    return see(TokenType::Break)||see(TokenType::Continue)||see(TokenType::Return)||see(TokenType::Yield)||see(TokenType::Raise);
}

bool Parser::isYield(){
    return see(TokenType::Yield);
}

bool Parser::isCompoundStatement(){
    bool ok = false;
    if(see(TokenType::Colon))          ok = true;
    else if(see(TokenType::Block))     ok = true;
    else if(see(TokenType::If))        ok = true;
    else if(see(TokenType::While))     ok = true;
    else if(see(TokenType::For))       ok = true;
    else if(see(TokenType::Try))       ok = true;
    else if(see(TokenType::With))      ok = true;
    else if(see(TokenType::Def))       ok = true;
    else if(see(TokenType::Function))  ok = true;
    else if(see(TokenType::Var))       ok = true;
    else if(see(TokenType::Let))       ok = true;
    else if(see(TokenType::Const))     ok = true;
    else if(see(TokenType::Class))     ok = true;
    else if(see(TokenType::Unless))    ok = true;
    else if(see(TokenType::Interface)) ok = true;
    else if(see(TokenType::Enum))      ok = true;
    else if(see(TokenType::Switch))    ok = true;
    else if(see(TokenType::NameSpace)) ok = true;
    else if(see(TokenType::Repeat))    ok = true;
    return ok;
}

bool Parser::isImportStatement(){
    return see(TokenType::From)||see(TokenType::Import);
}

///('+=' | '-=' | '*=' | '/=' | '%=' | '&=' | '|=' | '^=' | '<<=' | '>>=' | '**=' | '\=' | '&&=' | '^^=' | '||=' | '>>>=')
bool Parser::isAugAssign(){
    return see(TokenType::AddAssign)||see(TokenType::SubAssign)
           || see(TokenType::MulAssign) || see(TokenType::DivAssign)
           || see(TokenType::AndAssign) || see(TokenType::OrAssign) || see(TokenType::XorAssign)
           || see(TokenType::BinAndAssign) || see(TokenType::BinOrAssign) || see(TokenType::BinXorAssign)
           || see(TokenType::ModAssign) || see(TokenType::RevDivAssign) || see(TokenType::ExpAssign)
           || see(TokenType::ShiftLeftAssign) || see(TokenType::ShiftRightAssign)
           || see(TokenType::ShiftAssign);
}

bool Parser::isOperator(){
    bool ok = contains(value(),operators);
    return ok&&((peek().type()==TokenType::NewLine) || (peek().type()==TokenType::SemiColon) || (peek().type()==TokenType::Comma) || (peek().type()==TokenType::End || (peek().type()==TokenType::ParenClose)));
}

bool Parser::isClosing(){
    return (see(TokenType::ParenClose) || see(TokenType::BraceClose) || see(TokenType::BracketClose) );
}

bool Parser::isOperatorCall(){
    bool ok = contains(value(),operators);
    return ok&&(peek().type()==TokenType::ParenOpen);
}

/// '<'|'>'|'=='|'>='|'<='|'!='|'in'|'not' 'in'|'is'|'is' 'not'|'!=='|'==='|'instanceof'||'parentof'|'subclassof'|'equals'
bool Parser::isComparison(){
    return see(TokenType::GreatEqual) || see(TokenType::LessEqual)
           || see(TokenType::Less)    || see(TokenType::Great)
           || see(TokenType::Equal)   || see(TokenType::NotEqual)
           || see(TokenType::In)      || see(TokenType::Is)
           || (see(TokenType::Not)&&(peek().type()==TokenType::In))
           || (see(TokenType::Is) &&(peek().type()==TokenType::Not))
           || see(TokenType::DeepEqual)  || see(TokenType::NotDeepEqual) || see(TokenType::Equals)
           || see(TokenType::Instanceof) || see(TokenType::Parentof) || see(TokenType::Subclassof);
}

/// '(' [arglist] ')' | '[' subscriptlist ']' | '.' NAME
bool Parser::isTrailer(){
    return see(TokenType::ParenOpen) || see(TokenType::BracketOpen) || see(TokenType::Dot);
}

bool Parser::isModifier(){
    return see(TokenType::Public)||see(TokenType::Protected)||see(TokenType::Private)||see(TokenType::Static)||see(TokenType::Abstract);
}

bool Parser::isKeyWord(){
    return (!see(TokenType::Self)&&!see(TokenType::This)&&!see(TokenType::Super)&&peek().type()!=TokenType::Dot)
    && token().clazz()==TokenClass::Keyword;
}

/**
* script  : (NEWLINE | stmt)* END
*/
node_ptr Parser::script(){
    node_ptr node  = nullptr;
    Token token = this->token();
    if(have(TokenType::Package)){
        token.value = dottedName();
        have(TokenType::SemiColon);
    }else{
        token.value = "nython";
    }
    node = node_ptr(new Script(token));
    while(!see(TokenType::End)){
        if(have(TokenType::NewLine));
        else node->add(stmt());
    }
    mustBe(TokenType::End);
    return node;
}

node_ptr Parser::stmt() {
    node_ptr node = nullptr;
    return node;
}

}
