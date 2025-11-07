/* Given Part for Building a Parser
 * for the Basic Perl-Like Language (BPL)
 * GivenParserPart.cpp
 * Programming Assignment 2
 * Fall 2025
*/

#include "parser.h"

map<string, bool> defVar;
map<string, Token> SymTable;

namespace Parser {
	bool pushed_back = false;
	LexItem	pushed_token;

	static LexItem GetNextToken(istream& in, int& line) {
		if( pushed_back ) {
			pushed_back = false;
			return pushed_token;
		}
		return getNextToken(in, line);
	}

	static void PushBackToken(LexItem & t) {
		if( pushed_back ) {
			abort();
		}
		pushed_back = true;
		pushed_token = t;	
	}

}

static int error_count = 0;
bool synctatic_error = false;
int ErrCount()
{
    return error_count;
}

void ParseError(int line, string msg)
{
	++error_count;
	cout << error_count << ". Line " << line << ": " << msg << endl;
}

bool Prog(istream& in, int& line) {
	StmtList(in, line);
	if(error_count > 0) {
		return false;
	}

	cout << "Declared Variables:\n";
	for(auto i = defVar.begin();i != defVar.end();i++) {
		cout << i->first;
		if(next(i) != defVar.end()) {
			cout << ", ";
		}
	}
	cout << "\n\nDONE\n";

	return true;
}
bool StmtList(istream& in, int& line) {
	while(Stmt(in, line)) { // loop through as many statements, as long as it returns true (no errors)
		LexItem t = Parser::GetNextToken(in, line);
		if(t.GetToken() == DONE || t.GetToken() == RBRACES) {
			Parser::PushBackToken(t); // end of file or end of block reached
			break;
		} else {
			Parser::PushBackToken(t); // more statements to process
		}
	}
	if(synctatic_error) {
		ParseError(line, "Syntactic error in Program Body");
		return false; // error found
	}
	return true;
}
bool Stmt(istream& in, int& line) {
	LexItem t = Parser::GetNextToken(in, line);
	switch (t.GetToken()) {
	case IF:
		Parser::PushBackToken(t);
		if(!IfStmt(in, line)) {
			ParseError(line, "Incorrect If-Statement");
			synctatic_error = true;
			return false;
		}
		if(Parser::GetNextToken(in, line).GetToken() != SEMICOL) {
			ParseError(line-1, "Missing semicolon at end of Statement");
			return false;
		}
		return true;
	case IDENT:
		Parser::PushBackToken(t);
		if(!AssignStmt(in, line)) {
			ParseError(line, "Incorrect Assignment Statement");
			synctatic_error = true;
			return false;
		}
		if(Parser::GetNextToken(in, line).GetToken() != SEMICOL) {
			ParseError(line-1, "Missing semicolon at end of Statement");
			return false;
		}
		return true;
	case PRINTLN:
		Parser::PushBackToken(t);
		if(!PrintLnStmt(in, line)) {
			ParseError(line, "Incorrect PrintLn Statement");
			synctatic_error = true;
			return false;
		}
		if(Parser::GetNextToken(in, line).GetToken() != SEMICOL) {
			ParseError(line-1, "Missing semicolon at end of Statement");
			return false;
		}
		return true;
	case DONE:
		cout << "(DONE)\n";
		return false; // end of file reached
	case SEMICOL:
		return true; // empty statement, just a semicolon
	case ELSE:
		ParseError(line, "Illegal If Statement Else-Clause");
		return false;
	default:
		ParseError(line, "Invalid statement start: " + t.GetLexeme());
		return false; // error found, return false
	}
}
bool PrintLnStmt(istream& in, int& line) {
	if(Parser::GetNextToken(in, line).GetToken() != PRINTLN) {
		ParseError(line, "Missing PRINTLN keyword at start of PrintLnStmt"); // should never happen
		return false;
	}
	if(Parser::GetNextToken(in, line).GetToken() != LPAREN) {
		ParseError(line, "Missing Left Parenthesis of PrintLn Statement");
		return false;
	}
	bool list = ExprList(in, line);

	if(Parser::GetNextToken(in, line).GetToken() != RPAREN) {
		ParseError(line, "Missing Right Parenthesis of PrintLn Statement");
		return false;
	}
	
	return list;
}
bool IfStmt(istream& in, int& line) {
	LexItem t = Parser::GetNextToken(in, line);
	if(t.GetToken() != IF) {
		ParseError(line, "Missing IF keyword at start of IfStmt"); // should never happen
		return false;
	}
	t = Parser::GetNextToken(in, line);
	if(t.GetToken() != LPAREN) {
		ParseError(line, "Missing Left Parenthesis of If condition");
		return false;
	}
	if(!Expr(in, line)) {
		ParseError(line, "Invalid expression in IfStmt");
		return false;
	}
	t = Parser::GetNextToken(in, line);
	if(t.GetToken() != RPAREN) {
		ParseError(line, "Missing Right Parenthesis of If condition");
		return false;
	}
	t = Parser::GetNextToken(in, line);
	if(t.GetToken() != LBRACES) {
		ParseError(line, "Missing left brace for If Statement Clause");
		return false;
	}
	if(!StmtList(in, line)) {
		ParseError(line, "Invalid statement list in IF body");
		return false;
	}
	t = Parser::GetNextToken(in, line);
	if(t.GetToken() != RBRACES) {
		ParseError(line, "Missing right brace for If Statement Clause");
		return false;
	}
	t = Parser::GetNextToken(in, line);
	if(t.GetToken() == ELSE) {
		if(Parser::GetNextToken(in, line).GetToken() != LBRACES) {
			ParseError(line, "Missing left brace for an Else-Clause");
			return false;
		}
		if(!StmtList(in, line)) {
			ParseError(line, "Missing Statement for Else-Clause");
			return false;
		}
		if(Parser::GetNextToken(in, line).GetToken() != RBRACES) {
			line -= 1;
			ParseError(line, "Missing right brace for an Else-Clause");
			return false;
		}
	} else {
		Parser::PushBackToken(t); // no else statement.
	}
	return true;
}

bool AssignStmt(istream& in, int& line) {
	LexItem ident = Parser::GetNextToken(in, line);
	defVar[ident.GetLexeme()] = true; // mark variable as defined
	if(ident.GetToken() != IDENT) {
		ParseError(line, "Missing identifier at start of AssignStmt"); // should never happen
		return false;
	}
	if(!AssigOp(in, line)) {
		return false;
	}
	if(!Expr(in, line)) {
		ParseError(line, "Missing Expression in Assignment Statement");
		return false;
	}
	
	return true;
}

bool Var(istream& in, int& line) {
	if(Parser::GetNextToken(in, line).GetToken() != IDENT) {
		ParseError(line, "Missing identifier in Var");
		return false;
	}
	return true;
}

bool ExprList(istream& in, int& line) {
	while(true) {
		if(!Expr(in, line)) {
			ParseError(line, "Invalid expression in ExprList");
			return false;
		}
		LexItem t = Parser::GetNextToken(in, line);
		if(t.GetToken() == COMMA) {
			continue;
		} else {
			Parser::PushBackToken(t); // push back non-comma token
			break; 
		}
	}
	return true;
}

bool Expr(istream& in, int& line) {
	if(!OrExpr(in, line)) {
		return false;
	}
	return true;
}

bool AssigOp(istream& in, int& line) {
	LexItem t = Parser::GetNextToken(in, line);
	if(!(t.GetToken() == ASSOP || t.GetToken() == CADDA || t.GetToken() == CSUBA || t.GetToken() == CCATA)) { // every valid assignment operator
		ParseError(line, "Missing Assignment Operator");
		return false;
	}
	return true;
}

bool OrExpr(istream& in, int& line) {
	if(!AndExpr(in, line)) {
		return false;
	}
	while(true) {
		LexItem t = Parser::GetNextToken(in, line);
		if(t.GetToken() == OR) {
			if(!AndExpr(in, line)) {
				return false;
			}
		} else {
			Parser::PushBackToken(t);
			break;
		}
	}
	return true;
}

bool AndExpr(istream& in, int& line) {
	if(!RelExpr(in, line)) {
		return false;
	}
	while(true) {
		LexItem t = Parser::GetNextToken(in, line);
		if(t.GetToken() == AND) {
			if(!RelExpr(in, line)) {
				return false;
			}
		} else {
			Parser::PushBackToken(t);
			break;
		}
	}
	return true;
}

bool RelExpr(istream& in, int& line) {
	if(!AddExpr(in, line)) {
		return false;
	}
	LexItem t = Parser::GetNextToken(in, line);
	if(t.GetToken() == SLTE || t.GetToken() == SGT || t.GetToken() == SEQ || t.GetToken() == NLT || t.GetToken() == NGTE || t.GetToken() == NEQ) {
		if(!AddExpr(in, line)) {
			return false;
		}
	} else {
		Parser::PushBackToken(t);
	}
	return true;
}

bool AddExpr(istream& in, int& line) {
	if(!MultExpr(in, line)) {
		return false;
	}
	while(true) {
		LexItem t = Parser::GetNextToken(in, line);
		if(t.GetToken() == PLUS || t.GetToken() == MINUS || t.GetToken() == CAT) {
			if(!MultExpr(in, line)) {
				ParseError(line, "Missing operand after an operator");
				return false;
			}
		} else {
			Parser::PushBackToken(t);
			break;
		}
	}
	return true;
}
bool MultExpr(istream& in, int& line) {
	if(!UnaryExpr(in, line)) {
		return false;
	}
	while(true) {
		LexItem t = Parser::GetNextToken(in, line);
		if(t.GetToken() == MULT || t.GetToken() == DIV || t.GetToken() == REM || t.GetToken() == SREPEAT) {
			if(!UnaryExpr(in, line)) {
				ParseError(line, "Missing operand after an operator");
				return false;
			}
		} else {
			Parser::PushBackToken(t);
			break;
		}
	}
	return true;
}
bool UnaryExpr(istream& in, int& line) {
	LexItem t = Parser::GetNextToken(in, line);
	if(t.GetToken() == MINUS) {
		if(!ExponExpr(in, line, 0)) {
			return false;
		}
	} else if (t.GetToken() == PLUS ||  t.GetToken() == NOT) {
		if(!ExponExpr(in, line, 1)) {
			return false;
		}
	} else {
		Parser::PushBackToken(t);
		if(!ExponExpr(in, line, 1)) {
			return false;
		}
	}
	return true;
}

bool ExponExpr(istream& in, int& line, int sign) { // 1 == positive, 0 == negative
	if(!PrimaryExpr(in, line, sign)) {
		ParseError(line, "Missing operand for an operator");
		return false;
	}
	while(true) {
		LexItem t = Parser::GetNextToken(in, line);
		if(t.GetToken() == EXPONENT) {
			if(!PrimaryExpr(in, line, 1)) {
				ParseError(line, "Missing exponent operand after exponentiation");
				ParseError(line, "Missing operand for an operator");
				return false;
			}
		} else {
			Parser::PushBackToken(t);
			break;
		}
	}
	return true;
}
bool PrimaryExpr(istream& in, int& line, int sign) {
	LexItem t = Parser::GetNextToken(in, line);

	switch(t.GetToken()) {
		case IDENT : {
			if(defVar.find(t.GetLexeme()) == defVar.end()) {
				ParseError(line, "Use of undefined variable: " + t.GetLexeme());
				return false;
			}
			return true;
		} case ICONST : {
			return true;
		} case FCONST : {
			return true;
		} case SCONST : {
			return true;
		}
		default : {
			if(t.GetToken() == LPAREN) {
				if(!Expr(in, line)) {
					ParseError(line, "Missing expression after Left Parenthesis");
					return false;
				}
				if(Parser::GetNextToken(in, line).GetToken() != RPAREN) {
					ParseError(line, "Missing right Parenthesis after expression");
					return false;
				}
			} else {
				return false;
			}
		}
	}
	return true;
}

