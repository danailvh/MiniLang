#pragma once
#include "Visitor.h"
#include <unordered_map>
#include <stack>

class Parser
{
	//get tokens vector & identation vector 
	//return Expression pointer to the root of the program
	//that tree has a node for every expression in the program
	std::vector<Token> tokens;
	std::vector<unsigned> identations;
	std::unordered_map<std::string, char> returnTypes;
	std::stack<std::unordered_map<std::string, char>> types;
	unsigned current;

	bool notEnd();
	TokenType peek();
	bool currType(TokenType t);
	bool typeIn(const std::vector<TokenType>& vec);
	std::shared_ptr<Expression> retNullptr(int startpos);
	std::vector<std::shared_ptr<Expression>> params();
	std::vector<std::shared_ptr<Expression>> args();

	std::shared_ptr<Expression> topStmts();
	std::shared_ptr<Expression> funcDef();
	std::shared_ptr<Expression> statement(); 
	std::shared_ptr<Expression> block();
	std::shared_ptr<Expression> identifier();
	std::shared_ptr<Expression> newIdentifier(std::string type);
	std::shared_ptr<Expression> funcCall();
	std::shared_ptr<Expression> whileStmt();
	std::shared_ptr<Expression> ifCond(); //add this to the grammar
	std::shared_ptr<Expression> ifStmt();
	std::shared_ptr<Expression> elifStmt();
	std::shared_ptr<Expression> elseStmt();
	std::shared_ptr<Expression> assignementExpr(); 
	std::shared_ptr<Expression> literals();
	std::shared_ptr<Expression> breakCont();
	std::shared_ptr<Expression> unaryExpr();
	std::shared_ptr<Expression> binaryExpr();
	std::shared_ptr<Expression> disj();
	std::shared_ptr<Expression> conj();
	std::shared_ptr<Expression> inversion();
	std::shared_ptr<Expression> comparison();
	std::shared_ptr<Expression> expr();
	std::shared_ptr<Expression> term();
	std::shared_ptr<Expression> factor();
	std::shared_ptr<Expression> power();
	std::shared_ptr<Expression> atom();
	std::shared_ptr<Expression> varDef();
public:
	Parser() : current(0) 
	{
		std::unordered_map<std::string, char> t;
		types.push(t);
	};
	std::shared_ptr<Expression> parse(const std::vector<Token>& tokens, const std::vector<unsigned>& ident);
};

