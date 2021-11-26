#pragma once
#include <string>
#include <vector>

enum TokenType
{
	Plus,
	Minus,
	Star,
	DoubleStar, 
	Slash,
	Mod,
	LessThan,
	MoreThan,
	LessEqual,
	MoreEqual,
	Equal,
	EqualEqual,
	NotEqual,
	LeftPar,
	RightPar,
	Coma,
	Colon,
	Dot,
	SemiColon,
	PlusEqual,
	MinusEqual,
	TimesEqual,
	DivEqual,

	SingleQuote,
	DoubleQuote,
	Hash, 
	Backslash,

	//Keywords
	//"if","elif","else","break","continue","return","while","and","or","not","def","int","float","string","bool","print"
	IntD,
	FloatD,
	StringD,
	BoolD,
	If,
	Elif,
	Else,
	While,
	Break,
	Continue,
	Def,
	Return,
	And,
	Or,
	Not,
	Print,

	Identifier,
	String,
	Integer,
	Float,
	True,
	False,

	Newline,  
	Invalid

};

struct Token
{
	TokenType type;
	unsigned line;
	unsigned col;
	std::string lexeme;
	Token(TokenType t, unsigned l, unsigned c, const std::string& lex) : type(t), line(l), col(c), lexeme(lex) {};
};

class Lexer
{
	std::vector<Token> tokens;
	std::vector<unsigned> identation;
	std::string code;
	unsigned position;
	unsigned line;
	unsigned col;
	//! bool ErrorOcurred and check 

	void addToken(TokenType type, unsigned size = 1, const std::string & lex = "");
	void addComment();
	void addNewline(unsigned& ident, bool& isNl);
	void addIdentifier();
	void addNumber();
	void addQuote(TokenType type);
	char peek();
	void Tokenize();
public:
	Lexer(const std::string& c = "") : code(c), position(0), line(0), col(0) {};
	std::vector<Token> getTokens(const std::string& c = ""); 
	std::vector<unsigned> getIdentation();
	void print();

};

