#include "Lexer.h"
#include <assert.h>
#include <unordered_map>
#include <iostream>

bool isNum(const char n)
{
	return n >= '0' && n <= '9';
}

bool isIdentFirst(const char n)
{
	return (n >= 'a' && n <= 'z') || (n >= 'A' && n <= 'Z') || (n == '_');
}

bool isIdent(const char n)
{
	return isIdentFirst(n) || isNum(n);
}

char Lexer::peek()
{
	if (position + 1 < code.size())
		return code[position + 1];
	//this is fine because ? is unused - it won't match anything 
	return '?'; 
}

void Lexer::addIdentifier()
{
	std::string lex = "";
	unsigned current = position;
	//first is already checked
	lex += code[current];
	current++;
	while (isIdent(code[current]))
	{
		lex += code[current];
		current++;
	}

	// identifier or keyword 
	std::unordered_map<std::string, TokenType> keywords{ {"if",If}, {"elif",Elif}, {"else",Else}, {"break",Break}, {"continue",Continue},
		{"return",Return}, {"while",While}, {"and",And}, {"or",Or}, {"not",Not}, {"def",Def}, {"int",IntD}, {"float",FloatD}, {"string",StringD},
		{"bool",BoolD}, {"True",True}, {"False",False}, {"print",Print}};

	if (keywords.count(lex))
	{
		addToken(keywords[lex], lex.size(), lex);
		return;
	}
	addToken(Identifier, lex.size(), lex);

	
}
void Lexer::addNumber()
{
	std::string number = "";
	unsigned current = position;

	//part before '.'
	while (current < code.size() && isNum(code[current]))
	{
		number += code[current];
		current++;
	}
		
	//if it isn't a float we are done
	if (current >= code.size() || code[current] != '.')
	{
		addToken(Integer, number.size(), number);
		return;
	}

	//second part
	number += code[current];
	current++;
	while (current < code.size() && isNum(code[current]))
	{
		number += code[current];
		current++;
	}
	if (current < code.size() && current == '.')
	{
		assert("unexpected '.' " && 0);
		return;
	}
	addToken(Float, number.size(), number);
		
}

void Lexer::addComment()
{
	// drop every symbol until newline
	while (position < code.size() && code[position] != '\n')
		position++;
}

void Lexer::addNewline(unsigned &ident,bool& isNl)
{
	identation.push_back(ident);
	addToken(Newline);
	line++;
	col = 0;
	isNl = true;
	ident = 0;
}

void Lexer::addQuote(TokenType type)
{
	bool isSingleQuote = type == SingleQuote ? true : false;
	std::string lex = "";
	position++;
	unsigned current = position;
	while (current < code.size() && ((isSingleQuote && code[current] != '\'') || (!isSingleQuote && code[current] != '"')))
	{
		if (code[current] == '\\')
			current++;

		lex += code[current];
		current++;
	}
	
	if (current < code.size() && ((isSingleQuote && code[current] == '\'') || (!isSingleQuote && code[current] == '"')))
	{
		addToken(String, lex.size(), lex);
		position++;
		return;
	}
	assert("expected closing quote" && 0);
}

void Lexer::addToken(TokenType type, unsigned size,const std::string& lex)
{
	Token t(type, line, col, lex);
	position += size;
	col += size;
	tokens.push_back(t);
}

void Lexer::print()
{
	std::vector<std::string> enumnames({
		"Plus", "Minus", "Star", "DoubleStar", "Slash", "Mod", "LessThan", "MoreThan", "LessEqual", "MoreEqual", "Equal", "EqualEqual", "NotEqual", "LeftPar",
		"RightPar", "Coma", "Colon", "Dot", "SemiColon", "PlusEqual", "MinusEqual", "TimesEqual", "DivEqual", "SingleQuote", "DoubleQuote", "Hash", "Backslash",
		"IntD", "FloatD", "StringD", "BoolD", "If", "Elif" , "Else", "While", "Break", "Continue", "Def", "Return", "And", "Or", "Not", "Print", "Identifier", "String", "Integer", "Float", "True", "False", 
		"Newline", "Invalid"});
	for (auto& i : tokens)
	{
		std::cout << enumnames[i.type] << '\n';
	}
}

void Lexer::Tokenize()
{
	unsigned currIdentation = 0;
	bool isNewLine = true;

	while (position < code.size())
	{
		//remove whitespace and get identation if it is beggining of a line
		while (position < code.size() && (code[position] == ' ' || code[position] == '\t'))
		{
			//tab becomes 4 spaces
			if (code[position] == '\t')
			{
				col += 3;
				if (isNewLine)
					currIdentation += 3;
			}
			position++;
			col++;
			if (isNewLine)
				currIdentation++;
		}
		isNewLine = false;

		switch (code[position])
		{
		case '+': peek() == '=' ? addToken(PlusEqual, 2, "PlusEqual") : addToken(Plus, 1, "Plus");
			continue;
		case '-': peek() == '=' ? addToken(MinusEqual, 2, "MinusEqual") : addToken(Minus, 1, "Minus");
			continue;
		case '*': peek() == '=' ? addToken(TimesEqual, 2, "TimesEqual") : peek() == '*' ? addToken(DoubleStar, 2, "Power") : addToken(Star, 1, "Times");
			continue;
		case '/': peek() == '=' ? addToken(DivEqual, 2, "DivideEqual") : addToken(Slash, 1, "Divide");
			continue;
		case '%': addToken(Mod, 1, "Mod");
			continue;
		case '<': peek() == '=' ? addToken(LessEqual, 2, "LessEqual") : addToken(LessThan, 1, "LessThan");
			continue;
		case '>': peek() == '=' ? addToken(MoreEqual, 2, "MoreEqual") : addToken(MoreThan, 1, "MoreThan");
			continue;
		case '=': peek() == '=' ? addToken(EqualEqual, 2, "EqualEqual") : addToken(Equal, 1, "Equal");
			continue;
		case '!': if (peek() == '=') addToken(NotEqual, 2, "NotEqual");
			continue;
		case '(': addToken(LeftPar);
			continue;
		case ')': addToken(RightPar);
			continue;
		case ',': addToken(Coma);
			continue;
		case ':': addToken(Colon);
			continue;
		case ';': addToken(SemiColon);
			continue;
		case '.': addToken(Dot);
			continue;
		case '\'': addQuote(SingleQuote);
			continue;
		case '"': addQuote(DoubleQuote);
			continue;
		case '#': addComment();
			continue;
		case '\\': addToken(Backslash);
			continue;
		case '\n': addNewline(currIdentation,isNewLine);
			continue;
		default:
			break;
		}

		// int or float
		if (isNum(code[position]))
		{
			if (code[position] == '0' && isNum(peek()))
				assert("number error" && 0);
			addNumber();
			continue;
		}

		// keyword and identifier check
		if (isIdentFirst(code[position]))
		{
			addIdentifier();
			continue;
		}

		assert("Invalid token" && 0);
	}
	//push identation of last row;
	identation.push_back(currIdentation);
}

std::vector<Token> Lexer::getTokens(const std::string& c)
{
	if (c == "")
		return tokens;
	else
	{
		code = c;
		Tokenize();
		return tokens;
	}
}

std::vector<unsigned> Lexer::getIdentation()
{
	return identation;
}