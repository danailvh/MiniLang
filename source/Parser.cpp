#include "Parser.h"

bool Parser::notEnd()
{
	return current < tokens.size(); 
}

TokenType Parser::peek()
{
	if (current + 1 < tokens.size())
		return tokens[current + 1].type;
	else
		return Invalid; 
}

bool Parser::currType(TokenType t)
{
	return current < tokens.size() && (tokens[current].type == t);
}

bool Parser::typeIn(const std::vector<TokenType>& vec) 
{
	for (auto& i : vec)
	{
		if (currType(i))
			return true;
	}
	return false;
}

std::shared_ptr<Expression> Parser::retNullptr(int startpos)
{
	current = startpos;
	return nullptr;
}

std::shared_ptr<Expression> Parser::identifier()
{
	if (currType(Identifier) && ++current)		
		return std::make_shared<IdentifierExpression>(tokens[current - 1].lexeme,types.top()[tokens[current - 1].lexeme]);

	return nullptr;
}

std::shared_ptr<Expression> Parser::newIdentifier(std::string type)
{
	char t = 0;
	if (currType(Identifier) && ++current)
	{
		if (types.top().count(tokens[current - 1].lexeme) != 0)
			assert("Variable redefinition" && 0);
		if (type == "int")
			t = INT;
		else if (type == "float")
			t = FLOAT;
		else if (type == "string")
			t = STRING;
		else if (type == "bool")
			t = BOOL;
		else
			assert("nonexistent type" && 0);
		types.top()[tokens[current - 1].lexeme] = t;
		return std::make_shared<IdentifierExpression>(tokens[current - 1].lexeme,t);
	}
	return nullptr;
}

std::shared_ptr<Expression> Parser::literals()
{
	// string,boolean,integer,float
	if (currType(String) && ++current)
		return std::make_shared<LiteralString>(tokens[current - 1].lexeme);
	if (currType(Integer) && ++current)
		return std::make_shared<LiteralInteger>(stoi(tokens[current - 1].lexeme));
	if (currType(Float) && ++current)
		return std::make_shared<LiteralFloat>(stod(tokens[current - 1].lexeme));
	if (currType(True) && ++current)
		return std::make_shared<LiteralBoolean>(true);
	if (currType(False) && ++current)
		return std::make_shared<LiteralBoolean>(false);

	return nullptr;
}

std::shared_ptr<Expression> Parser::atom()
{
	if (currType(LeftPar))
	{
		int startPos = current;
		current++;
		if (auto atom = binaryExpr())
			if (currType(RightPar) && ++current)
				return atom;
		return retNullptr(startPos);
	}
	// idetifier | literal | non-void FunctionCall 
	if (currType(Identifier) && peek() == LeftPar && returnTypes[tokens[current].lexeme] != 0)
	{
		if (auto atom = funcCall())
			return atom;
	}
	if (auto atom = literals())
		return atom;
	if (auto atom = identifier())
		return atom;
	return nullptr;
}

std::shared_ptr<Expression> Parser::power()
{
	//atom ['**' factor]
	unsigned startpos = current;
	auto lhs = atom();
	if (!lhs)
		return retNullptr(startpos);
	if (currType(DoubleStar))
	{
		current++;
		if (auto rhs = factor())
			return std::make_shared<BinaryExpression>("Power", lhs, rhs, lhs->getType()); 
		else
			return retNullptr(startpos);
	}
	return lhs;
}

std::shared_ptr<Expression> Parser::factor()
{
	// ['-'] (factor | power) 
	unsigned startpos = current;
	std::shared_ptr<Expression> pow;
	if (currType(Minus))
	{
		current++;
		if (auto body = factor())
			return std::make_shared<UnaryExpression>("Minus", body, body->getType());
		else
			return retNullptr(startpos);
	}
	else
		pow = power();
	return pow;
}

std::shared_ptr<Expression> Parser::term()
{
	// factor ((*|/|%|) factor)* 
	unsigned startpos = current;
	auto lhs = factor();
	if (!lhs)
		return nullptr;
	if (typeIn({ Star, Slash, Mod }))
	{
		auto compOp = tokens[current].lexeme;
		current++;
		if (auto rhs = term())
			return std::make_shared<BinaryExpression>(compOp, lhs, rhs, lhs->getType());
		else
			return retNullptr(startpos); //error
	}
	else
		return lhs;
}

std::shared_ptr<Expression> Parser::expr()
{
	// term ((+|-) term)* 
	unsigned startpos = current;
	auto lhs = term();
	if (!lhs)
		return nullptr;
	if (typeIn({ Plus, Minus }))
	{
		auto op = tokens[current].lexeme;
		current++;
		if (auto rhs = expr())
			return std::make_shared<BinaryExpression>(op, lhs, rhs, lhs->getType());
		else
			return retNullptr(startpos); //error
	}
	else
		return lhs;
}


std::shared_ptr<Expression> Parser::comparison()
{
	// expr ('compOp' expr)* 
	unsigned startpos = current;
	auto lhs = expr();
	if (!lhs)
		return nullptr;
	if (typeIn({ LessThan, MoreThan, LessEqual, MoreEqual, EqualEqual, NotEqual }))
	{
		auto compOp = tokens[current].lexeme;
		current++;
		if (auto rhs = comparison())
			return std::make_shared<BinaryExpression>(compOp, lhs, rhs, BOOL);
		else
			return retNullptr(startpos); //error
	}
	else
		return lhs;
}

std::shared_ptr<Expression> Parser::inversion()
{
	// ('not' inversion | comparison)
	unsigned startpos = current;
	if (currType(Not))
	{
		current++;
		if (auto inv = inversion())
			return std::make_shared<UnaryExpression>("not", inv, BOOL); 
		else
			return retNullptr(startpos);
	}
	if (auto comp = comparison())
		return comp;
	return nullptr;
}

std::shared_ptr<Expression> Parser::conj()
{
	// inversion | inversion ('and' conj)+
	unsigned startpos = current;
	auto lhs = inversion();
	if (!lhs)
		return nullptr;

	if (currType(And))
	{
		current++;
		if (auto rhs = conj())
			return std::make_shared<BinaryExpression>("and", lhs, rhs, BOOL);
		else
		{
			current = startpos;
			return nullptr;
		}
	}
	else
		return lhs;
}

std::shared_ptr<Expression> Parser::disj()
{
	// conj | conj('or' disj)+
	unsigned startpos = current;
	auto lhs = conj();
	if (!lhs)
		return nullptr;

	if (currType(Or))
	{
		current++;
		if (auto rhs = disj())
			return std::make_shared<BinaryExpression>("or", lhs, rhs, BOOL);
		else
		{
			current = startpos;
			return nullptr;
		}
	}
	else
		return lhs;
}

std::shared_ptr<Expression> Parser::binaryExpr()
{
	return disj();
}

std::shared_ptr<Expression> Parser::breakCont()
{
	if(currType(Break) && peek() == Newline && ++current)
		return std::make_shared<UnaryExpression>("break", nullptr, 0);
	else if (currType(Continue) && peek() == Newline && ++current)
		return std::make_shared<UnaryExpression>("continue", nullptr, 0);
	return nullptr;
}

std::shared_ptr<Expression> Parser::unaryExpr()
{
	// unaryExpr :- ( (return|print)(funcCall|binExpr|atom) ) | (return|break|continue) ) NL 
	unsigned startpos = current;
	if (!typeIn({Return,Print,Break,Continue}))
		return nullptr;
	if (currType(Return) && peek() == Newline)
	{
		current++;
		return std::make_shared<UnaryExpression>("return", nullptr, 0);
	}
	if (auto unary = breakCont())
		return unary;
		
	auto op = tokens[current].lexeme;
	current++;

	if (auto lhs = binaryExpr())
		return std::make_shared<UnaryExpression>(op, lhs, lhs->getType());
	if (auto lhs = funcCall())
		return std::make_shared<UnaryExpression>(op, lhs, lhs->getType());
	if (auto lhs = atom())
		return std::make_shared<UnaryExpression>(op, lhs, lhs->getType());
	current = startpos;
	return nullptr;
}

std::shared_ptr<Expression> Parser::varDef()
{
	// TYPE identifer '='  functioncall | binExpr | atom
	unsigned startpos = current;
	if (!typeIn({IntD,FloatD,StringD,BoolD}))
		return nullptr;
	
	auto varType = tokens[current].lexeme;
	current++;
	std::string varName = tokens[current].lexeme;

	auto lhs = newIdentifier(varType); 

	if (!lhs || !currType(Equal))
	{
		current = startpos;//error
		return nullptr;
	}
	current++;

	if (auto rhs = funcCall())
		return std::make_shared<VariableDefinitionExpression>(varName, rhs, lhs->getType());
	if (auto rhs = binaryExpr())
		return std::make_shared<VariableDefinitionExpression>(varName, rhs, lhs->getType());
	if (auto rhs = atom())
		return std::make_shared<VariableDefinitionExpression>(varName, rhs, lhs->getType());
	//error
	current = startpos;
	return nullptr;
}

std::shared_ptr<Expression> Parser::assignementExpr()
{
	// identifier 'asOP' (functionCall|binary|atom)
	unsigned startpos = current;
	std::string varName;
	if (currType(Identifier))
		varName = tokens[current].lexeme;
	else 
		return nullptr;
	current++;
	
	if (!typeIn({ Equal,PlusEqual,MinusEqual,TimesEqual,DivEqual }))
		return retNullptr(startpos);
		
	auto opName = tokens[current].lexeme;
	current++;

	if (auto rhs = funcCall())
		return std::make_shared<AssignmentExpression>(opName, varName, rhs, rhs->getType());
	if (auto rhs = binaryExpr())
		return std::make_shared<AssignmentExpression>(opName, varName, rhs, rhs->getType());
	if (auto rhs = atom())
		return std::make_shared<AssignmentExpression>(opName, varName, rhs, rhs->getType());

	return retNullptr(startpos);
	
}

std::shared_ptr<Expression> Parser::whileStmt()
{
	// 'while' ifcond ':' NEWLINE block
	unsigned startpos = current;
	if (!currType(While))
		return nullptr;
	current++;

	auto cond = ifCond();
	if (!cond || !currType(Colon) || peek() != Newline)
		return retNullptr(startpos);

	current += 2;

	if (auto body = block())
		return std::make_shared<WhileStatement>(cond, body);
	else
		return retNullptr(startpos);
}

std::shared_ptr<Expression> Parser::elseStmt()
{
	// 'else' ':' NL block
	unsigned startpos = current;
	if (!currType(Else))
		return nullptr;
	current++;
	if (!currType(Colon) && peek() == Newline)
	{
		current = startpos;
		assert("Expected ':' and Newline after else" && 0);
		return nullptr;
	}
	current += 2;

	auto body = block();
	if (!body)
		return retNullptr(startpos);
	return std::make_shared<IfStatement>("else", nullptr, body, nullptr);
}

std::shared_ptr<Expression> Parser::elifStmt()
{
	// 'elif' ifCond ':' NL Block [elifStmt|elseStmt]
	unsigned startpos = current;
	if (!currType(Elif))
		return nullptr;
	current++;
	auto cond = ifCond();

	if (!cond || !currType(Colon) || !(peek() == Newline))
		return retNullptr(startpos);

	current += 2;

	auto body = block();

	if (!body)
		return retNullptr(startpos);

	if (auto elif = elifStmt())
		return std::make_shared<IfStatement>("elif", cond, body, elif);
	if (auto elseSt = elseStmt())
		return std::make_shared<IfStatement>("elif", cond, body, elseSt);

	return std::make_shared<IfStatement>("elif", cond, body, nullptr);
}

std::shared_ptr<Expression> Parser::ifCond()
{
	if (auto ifcond = binaryExpr())
		return ifcond;
	if (auto ifcond = atom())
		return ifcond;
	if (auto ifcond = funcCall())
		return ifcond;
	return nullptr;
}

std::shared_ptr<Expression> Parser::ifStmt()
{
	// 'if' ifCond ':' NL Block [elifStmt|elseStmt]
	unsigned startpos = current;
	if (!currType(If))
		return nullptr;
	current++;
	auto cond = ifCond();

	if (!cond || !currType(Colon) || !(peek() == Newline))
		return retNullptr(startpos);

	current += 2;

	auto body = block();
	
	if (!body)
		return retNullptr(startpos);

	if (auto elif = elifStmt())
		return std::make_shared<IfStatement>("if", cond, body, elif);
	if (auto elseSt = elseStmt())
		return std::make_shared<IfStatement>("if", cond, body, elseSt);

	return std::make_shared<IfStatement>("if", cond, body, nullptr);
}

std::vector<std::shared_ptr<Expression>> Parser::args()
{
	// (binExpr|atom) | ( (binExpr|atom) ',' args )
	std::vector<std::shared_ptr<Expression>> args;
	while (notEnd() && !currType(RightPar))
	{
		if (auto arg = binaryExpr())
			args.push_back(arg);
		else if (auto arg = atom()) 
			args.push_back(arg);
		if (!currType(RightPar) && !currType(Coma))
			assert("expected ',' or ')' " && 0);
		if(currType(Coma))
			current++;
	}
	return args;
}

std::shared_ptr<Expression> Parser::funcCall()
{
	// identifier '(' args ')'
	unsigned startpos = current;
	if (!currType(Identifier))
		return nullptr;
	auto name = tokens[current].lexeme;
	auto funcName = identifier();
	if (!currType(LeftPar))
		return retNullptr(startpos);
	current++;
	auto arguments = args();
	if (!currType(RightPar))
		return retNullptr(startpos);
	current++;
	if (returnTypes.count(name) == 0)
		assert("function called before definition" && 0);
	char type = returnTypes[name];
	return std::make_shared<FunctionCall>(name, arguments, type);
}

std::vector<std::shared_ptr<Expression>> Parser::params()
{
	std::vector<std::shared_ptr<Expression>> params;
	while (notEnd() && !currType(RightPar))
	{
		char t = 0;
		if (currType(IntD))
			t = INT;
		else if (currType(FloatD))
			t = FLOAT;
		else if (currType(BoolD))
			t = BOOL;
		else if (currType(StringD))
			t = STRING;
		else
			assert("error in fucntion definition" && 0);

		current++;
		if (currType(Identifier))
		{
			types.top()[tokens[current].lexeme] = t; 
			params.push_back(std::make_shared<IdentifierExpression>(tokens[current].lexeme, t));
			current++;
		}
		else
			assert("error in function definition" && 0);
				
		if (!currType(RightPar) && !currType(Coma))
		{
			assert("expected ',' or ')' " && 0);
		}
		
		if (currType(Coma))
			current++;
	}
	return params;
}

std::shared_ptr<Expression> Parser::funcDef()
{
	// 'def' [TYPE] identifier '(' [params] ')' ':' NEWLINE block //if no type is specified then the function returns nothing (void)
	unsigned startpos = current;
	if (!currType(Def)) 
		return nullptr;
	current++;

	if (!typeIn({ IntD, FloatD, StringD, BoolD, Identifier}))
		return retNullptr(startpos);

	char type = 0;
	if (tokens[current].lexeme == "int")
		type = INT;
	else if (tokens[current].lexeme == "float")
		type = FLOAT;
	else if (tokens[current].lexeme == "string")
		type = STRING;
	else if (tokens[current].lexeme == "bool")
		type = BOOL;
	
	if(type != 0)
		current++;

	std::shared_ptr<Expression> name;
	if (!(name = identifier()))
		return retNullptr(startpos);
	std::string fName = tokens[current - 1].lexeme;

	if (returnTypes.count(fName) != 0)
		assert("attempted function redefinition" && 0);
	returnTypes[fName] = type;

	if (!currType(LeftPar))
		return retNullptr(startpos); //error
		
	current++;

	std::unordered_map <std::string, char> t;
	types.push(t);
	auto parameters = params();

	if (!(currType(RightPar) && peek() == Colon))
	{
		types.pop();
		return retNullptr(startpos);
	}
		
	current += 2;
	
	if (!currType(Newline))
	{
		types.pop();
		return retNullptr(startpos);
	}
		
	current++;

	auto body = block();

	types.pop();
	if (!body)
		return retNullptr(startpos);
	return std::make_shared<FunctionDeclaration>(fName, parameters, body, type);
}

std::shared_ptr<Expression> Parser::block()
{
	// ident statement (statement)* dedent
	unsigned startpos = current;
	std::vector<std::shared_ptr<Expression>> block;
	if (!notEnd() || current - 1 < 0)
		return nullptr;
	//identation checks
	unsigned preBlockIdent = identations[tokens[current - 1].line];
	unsigned blockIdent = identations[tokens[current].line];
	if (preBlockIdent >= blockIdent)
		assert("Identation error" && 0);

	while (notEnd() && preBlockIdent < identations[tokens[current].line])
	{
		if (auto st = statement())
			block.push_back(st);
		else
			return retNullptr(startpos);

		while (notEnd() && currType(Newline))
			current++;
	}
	return std::make_shared<BlockStatement>(block);
}

std::shared_ptr<Expression> Parser::statement()
{
	if (auto st = varDef())
		return st;
	if (auto st = assignementExpr())
		return st;
	if (auto st = funcCall())
		return st;
	if (auto st = binaryExpr())
		return st;
	if (auto st = unaryExpr())
		return st;
	if (auto st = ifStmt())
		return st;
	if (auto st = whileStmt())
		return st;
	if (auto st = identifier())
		return st;
	if (auto st = literals())
		return st;
	return nullptr;
}

std::shared_ptr<Expression> Parser::topStmts()
{
	int functionCount = 0;
	std::vector<std::shared_ptr<Expression>> prog;
	while (current < tokens.size())
	{
		if (auto top = funcDef())
		{
			prog.push_back(top);
			functionCount++;
		}
			
		else if (auto top = statement())
			prog.push_back(top);
		else if (tokens[current].type == Newline)
			current++;
		else
		{
			assert("Parsing Error" && 0);
		}
			
	}
	return std::make_shared<TopStatements>(prog,functionCount);
}

std::shared_ptr<Expression> Parser::parse(const std::vector<Token>& tokens, const std::vector<unsigned>& ident)
{
	if (!tokens.empty())
	{
		this->tokens = tokens;
		this->identations = ident;
	}
	auto prog = topStmts();
	return prog;
}