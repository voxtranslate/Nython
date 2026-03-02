#ifndef __TOKEN__HPP
#define __TOKEN__HPP

#include <string>
#include <sstream>
#include <iostream>
#include <IToken.hpp>
#include <fmt/format.h>

namespace nython::utils {
    class PrettyPrinter;
}

using nython::utils::PrettyPrinter;

namespace nython::lexer  {

template< typename ValueType, size_t InitSizeDiff = 0>
class ConstArray {

    private:
		ValueType * const data_;
		std::size_t size_;
    public:
        template<std::size_t N>
        ConstArray(ValueType(&data)[N]):data_(data),size_(N-InitSizeDiff) {
        }

        ConstArray(const ConstArray& that):data_{that.data_},size_{that.size_}{
        }

        virtual ~ConstArray() {
        }

        ConstArray& operator=(const ConstArray& that) {
            if(this!=&that) {
                data_ = that.data;
                size_ = that.size_;
            }
            return this;
        }

        char operator[](std::size_t index) const{
            return index < size_ ? data_[index]:throw std::out_of_range("");
        }

        std::size_t size() const{
            return size_;
        }

        ValueType const * data() const{
            return data_;
        }

        bool empty() const{
            return size_ == 0;
        }

        ValueType const * begin() const{
            return data();
        }

        ValueType const * end() const{
            return data() + size();
        }
};

struct TokenIdent {

    private:

        TokenType id_;
        TokenKind kind_;
        TokenClass cls_;

    public:

        TokenIdent(TokenType id, TokenKind kind, TokenClass cls): id_(id),kind_(kind),cls_(cls) {
        }

        TokenIdent(): TokenIdent(TokenType::Invalid, TokenKind::Error, TokenClass::Default) {
        }

        TokenType id() const{
            return id_;
        }

        void id(TokenType id) {
            id_ = id;
        }

        TokenKind kind() const{
            return kind_;
        }

        TokenClass cls() const{
            return cls_;
        }

        void kind(TokenKind kind) {
            kind_ = kind;
        }

        void cls(TokenClass cls) {
            cls_ = cls;
        }

        bool operator==(const TokenIdent& that) {
            return that.kind_==kind_ && that.cls_==cls_ && that.id_==id_;
        }
};

struct TokenDef {

	TokenIdent ident;
	std::string value;

	TokenDef(TokenType id,std::string value,TokenKind kind,TokenClass cls):ident(id,kind,cls),value(value) {
	}

	inline bool match4(char c0,char c1,char c2,char c3) const{
		return (value.size() == 4 && (c0 == value[0] && c1 == value[1] && c2 == value[2] && c3 == value[3]))
		|| (value.size() == 3 && (c0 == value[0] && c1 == value[1] && c2 == value[2]))
		|| (value.size() == 2 && (c0 == value[0]) && c1 == value[1])
		|| (value.size() == 1 && (c0 == value[0]));
	}

	inline bool match3(char c0,char c1,char c2) const{
		return (value.size() == 3 && (c0 == value[0] && c1 == value[1] && c2 == value[2]))
		|| (value.size() == 2 && (c0 == value[0] && c1 == value[1]))
		|| (value.size() == 1 && (c0 == value[0]));
	}

	inline bool match2(char c0,char c1) const{
		return (value.size() == 2 && (c0 == value[0] && c1 == value[1]))
		|| (value.size() == 1 && (c0 == value[0]));
	}

	friend std::ostream& operator<<(std::ostream& os,TokenDef& obj) {
		os<<"["<<obj.value.data()<<", "<<int(obj.ident.id())<<"]";
		return os;
	}
};


struct Token extends IToken {

    Location _location;
    TokenIdent ident  = {TokenType::End, TokenKind::End, TokenClass::Eof};
    std::string value = "End";

    Token() {
    }

    Token(TokenIdent t) : ident(t) {
    }

    Token(TokenIdent t, const std::string& v) : _location{}, ident(t), value(v) {
    }

    Token(TokenIdent t, const std::string& v, const Location& l) : _location(l), ident(t), value(v) {
    }

    virtual ~ Token() {
    }

    TokenType type() {
        return ident.id();
    }

    void type(TokenType type) {
        return ident.id(type);
    }

    TokenKind kind() {
        return ident.kind();
    }

    TokenClass clazz() {
        return ident.cls();
    }

    Token& operator=(const Token& token) {
        if(this!=&token) {
            ident     = token.ident;
            _location = token._location;
            value     = token.value;
        }
        return *this;
    }

    void writeToStdOut(PrettyPrinter p);
    std::string writeToString(PrettyPrinter p);
    std::string toString();

    bool isKeyWord();
    bool isAssignment();

    inline Location location() {
        return _location;
    }

    inline std::string fileName() {
        return _location.filename;
    }

    inline int line() {
        return _location.row;
    }

    inline int column(){
        return _location.column;
    }

    bool operator==(const Token& that);

    friend std::ostream& operator<<(std::ostream& os,Token token) {
        os << token.toString();
        return os;
    }
};

}

namespace fmt
{
  using namespace nython;
  using namespace nython::lexer;

  // Class that defines a formatter for a token kind
  template <>
  struct formatter<lexer::TokenType> :formatter<std::string_view>
  {
    inline std::string stringify(lexer::TokenType type) {
      switch(type) {
        // Lexer-specific
        case TokenType::Invalid: return "invalid token";
        case TokenType::End: return "end of tokens";
        case TokenType::NewLine: return "new line";
        case TokenType::Regex: return "Regex literal";
        case TokenType::Indent: return "indent";
        case TokenType::Dedent: return "dedent";
        case TokenType::Block: return "block statement";
        case TokenType::NotMultipleOfFourIndentError: return "not multiple of four indent error";
        case TokenType::MixedIndentError: return "mixed indent error";
        case TokenType::IndentationError: return "indentation error";
        case TokenType::DedentationError: return "dedentation error";
        case TokenType::LineContinuationError: return "line continuation error";
        case TokenType::UnterminatedStringError: return "unterminated string error";
        case TokenType::Comment: return "comment";/// for class and function documentation
        case TokenType::Whitespace: return "white space";
        case TokenType::BackQuote: return "back quote";/// '`'
        case TokenType::Identifier: return "identifier";
        case TokenType::Float: return "real literal";
        case TokenType::Integer: return "integer literal";
        case TokenType::Hex: return "hexadecimal literal";
        case TokenType::Oct: return "octal literal";
        case TokenType::Binary: return "binary literal";
        case TokenType::Complex: return "complex literal";
        case TokenType::String: return "string literal";
        case TokenType::True: return "true";
        case TokenType::False: return "false";
        case TokenType::None: return "none";
        case TokenType::Undefined: return "undefined keyword";
        case TokenType::Class: return "class";
        case TokenType::Abstract: return "abstract";
        case TokenType::Continue: return "continue";
        case TokenType::Ref: return "ref";
        case TokenType::If: return "if";
        case TokenType::Else: return "else";
        case TokenType::ElseIf: return "elseif";
        case TokenType::Default: return "default";
        case TokenType::For: return "for";
        case TokenType::Do: return "do";
        case TokenType::Extends: return "extends";
        case TokenType::Finally: return "finally";
        case TokenType::Final: return "final";
        case TokenType::Implements: return "implements";
        case TokenType::Import: return "import";
        case TokenType::In: return "in";
        case TokenType::Pass: return "pass";
        case TokenType::Instanceof: return "instance of";
        case TokenType::Subclassof: return "subclass of";
        case TokenType::Parentof: return "parent of";
        case TokenType::Interface: return "interface";
        case TokenType::Package: return "package";
        case TokenType::NameSpace: return "namespace";
        case TokenType::Private: return "private";
        case TokenType::Protected: return "protected";
        case TokenType::Public: return "public";
        case TokenType::Return: return "return";
        case TokenType::Static: return "static";
        case TokenType::Super: return "super";
        case TokenType::Switch: return "switch";
        case TokenType::This: return "this";
        case TokenType::Self: return "self";
        case TokenType::Raise: return "raise";
        case TokenType::Try: return "try";
        case TokenType::While: return "while";
        case TokenType::Def: return "def";
        case TokenType::Function: return "function";
        case TokenType::Fun: return "fun";
        case TokenType::Fn: return "fn";
        case TokenType::EndBlock: return "end of block";
        case TokenType::Break: return "break";
        case TokenType::With: return "with";
        case TokenType::As: return "as";
        case TokenType::Assert: return "assert";
        case TokenType::Lambda: return "lambda";
        case TokenType::Case: return "case";
        case TokenType::Use: return "use";
        case TokenType::Enum: return "enum";
        case TokenType::Except: return "except";
        case TokenType::Print: return "print";
        case TokenType::Yield: return "yield";
        case TokenType::Is: return "is";
        case TokenType::From: return "from";
        case TokenType::Unless: return "unless";
        case TokenType::Repeat: return "repeat";
        case TokenType::Until: return "until";
        case TokenType::Loop: return "loop";
        case TokenType::Delete: return "delete";
        case TokenType::Sizeof: return "size of";
        case TokenType::Typeof: return "type of";
        case TokenType::Global: return "global";
        case TokenType::Execute: return "execute";
        case TokenType::Var: return "var";
        case TokenType::Const: return "const";
        case TokenType::Let: return "let";
        case TokenType::Ellipsis: return "...";/// ...
        case TokenType::Add: return "addition operator";
        case TokenType::Sub: return "subtraction operator";
        case TokenType::Mul: return "multiplication operator";
        case TokenType::Div: return "division operator";
        case TokenType::RevDiv: return "reverse division operator";/// '\'
        case TokenType::Exp: return "exponent operator"; /// **
        case TokenType::Mod: return "modulus operator"; /// %
        case TokenType::DoubleAdd: return "++ operator";/// ++
        case TokenType::DoubleSub: return "-- operator";/// --
        case TokenType::ColonIn: return ":>"; /// ':>'
        case TokenType::InColon: return "<:"; /// '<:'
        case TokenType::Not: return "!";/// '!'
        case TokenType::And: return "and operator";/// '&&'
        case TokenType::Or: return "or operator";/// '||'
        case TokenType::Xor: return "xor operator";/// '^^'
        case TokenType::BinOr: return "binary or operator";/// '|'
        case TokenType::BinXor: return "binary xor operator";/// '^'
        case TokenType::BinAnd: return "binary and operator";/// '&'
        case TokenType::Complement: return "complement operator";/// '~'
        case TokenType::Shift: return "shift operator";/// '>>>'
        case TokenType::ShiftLeft: return "left shift operator";
        case TokenType::ShiftRight: return "right shift operator";
        case TokenType::Equal: return "equal operator";
        case TokenType::Equals: return "equals";
        case TokenType::NotEqual: return "not equal operator";
        case TokenType::DeepEqual: return "deep equal operator";
        case TokenType::NotDeepEqual: return "not deep equal operator";
        case TokenType::GreatEqual: return "great or equal than operator";
        case TokenType::LessEqual: return "less or equal than operator";
        case TokenType::Great: return "greater than operator";
        case TokenType::Less: return "less than operator";
        case TokenType::Interval: return "..";/// ..
        case TokenType::Assign: return "assign operator";
        case TokenType::AddAssign: return "addition & assign operator";
        case TokenType::SubAssign: return "subtraction & assign operator";
        case TokenType::MulAssign: return "multiplication assign operator";
        case TokenType::DivAssign: return "division assign operator";
        case TokenType::ExpAssign: return "exponent assign operator";
        case TokenType::ModAssign: return "modulus assign operator";
        case TokenType::RevDivAssign: return "reverse division assign operator";/// '\='
        case TokenType::AndAssign: return "and assign operator";/// '&&='
        case TokenType::OrAssign: return "or assign operator";/// '||='
        case TokenType::XorAssign: return "xor assign operator";/// '^^='
        case TokenType::BinOrAssign: return "binary or assign operator";/// '|='
        case TokenType::BinXorAssign: return "binary xor assign operator";/// '^='
        case TokenType::BinAndAssign: return "binary and assign operator";/// '&='
        case TokenType::ComplementAssign: return "complement assign operator";/// '~='
        case TokenType::ShiftAssign: return "shift assign operator";/// '>>>='
        case TokenType::ShiftLeftAssign: return "left shift assign operator";
        case TokenType::ShiftRightAssign: return "right shift assign operator";
        case TokenType::BraceOpen: return "{";/// '{'
        case TokenType::BraceClose: return "}";/// '}'
        case TokenType::Comma: return ",";
        case TokenType::Colon: return ":";/// ':'
        case TokenType::Dot: return ".";
        case TokenType::SemiColon: return ";";
        case TokenType::BracketOpen: return "[";/// '['
        case TokenType::BracketClose: return "]";/// ']'
        case TokenType::ParenOpen: return "(";
        case TokenType::ParenClose: return ")";
        case TokenType::QuestionMark: return "?";
        case TokenType::At: return "@";/// '@'
        case TokenType::RightArrow: return "->";/// '->'
        case TokenType::LeftArrow: return "<-"; /// '<-'
        case TokenType::New: return "new"; /// '<-'
        default: return "<undefined>";
      }
    }

    template <typename FormatContext>
    auto format(lexer::TokenType type, FormatContext& ctx)
    {
      return formatter<std::string_view>::format(stringify(type), ctx);
    }
  };

  // Class that defines a formatter for a token
  template <>
  struct formatter<Token> : formatter<std::string_view>
  {
    inline std::string stringify(Token token)
    {
      if (!token.value.empty())
        return fmt::format("{} \"{}\" at {}", token.type(), token.value, token.location());
      else
        return fmt::format("{} at {}", token.type(), token.location());
    }

    template <typename FormatContext>
    auto format(Token token, FormatContext& ctx)
    {
      return formatter<std::string_view>::format(stringify(token), ctx);
    }
  };
}


#endif // __TOKEN__HPP

