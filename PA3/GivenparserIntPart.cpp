/* Implementation of Interpreter
 * for the Basic Perl-Like (BPL) Language
 * parserInt.cpp
 * Programming Assignment 3
 * Fall 2025
*/


#include "parserInt.h"

map<string, bool> defVar;
//map<string, Token> SymTable;
map<string, Value> TempsResults; //Container of temporary locations of Value objects for results of expressions, variables values and constants 
queue <Value> * ValQue; //declare a pointer variable to a queue of Value objects

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
bool execute = true;
int ErrCount()
{
    return error_count;
}

void ParseError(int line, string msg)
{
	++error_count;
	cout << error_count << ". Line " << line << ": " << msg << endl;
}


//Program is: Prog ::= StmtList
bool Prog(istream& in, int& line)
{
	bool f1;
	LexItem tok = Parser::GetNextToken(in, line);
	//cout << "in Prog" << endl;
	//cout << tok << " " << tok.GetLinenum() <<endl;
	
	if(tok.GetToken() == DONE && tok.GetLinenum() <= 1){
		ParseError(line, "Empty File");
		return true;
	}
	Parser::PushBackToken(tok);
	f1 = StmtList(in, line); 
			
	if(!f1) 
	{
		ParseError(line, "Missing Program");
		return false;
	}
	else 
	{
		
		cout << endl <<endl;
		cout << "DONE" << endl;	
		return true;
	}	
}//End of Prog

//StmtList ::= Stmt; { Stmt; }
bool StmtList(istream& in, int& line){
	bool status;
		
	LexItem tok;
	
	status = Stmt(in, line);
	while(status )
	{
		tok = Parser::GetNextToken(in, line);
		if(tok == DONE)
		{
			Parser::PushBackToken(tok);
			return true;
		}
		else if(tok == RBRACES)
		{
			Parser::PushBackToken(tok);
			return true;
		}
		
		if(tok != SEMICOL)
		{
			line--;
			ParseError(line, "Missing semicolon at end of Statement");
			return false;
		}
		
		status = Stmt(in, line);
		
	}
	int l = line;		
	tok = Parser::GetNextToken(in, line);
	line = l;
	if(tok == ELSE )
	{
		ParseError(line, "Illegal If Statement Else-Clause");
		return false;
	}
	else if(tok == RBRACES)
	{
		Parser::PushBackToken(tok);
		return true;
	}
	
	else if(tok == DONE && !status)
	{
		ParseError(line, "Syntactic error in Program Body.");
		return false;
	}
	else
	{
		ParseError(line, "Syntactic error in Program Body.");
		return false;
	}
	
}//End of StmtList function

//Stmt ::= AssignStme | PrintLnStmt | IfStmt  
bool Stmt(istream& in, int& line){
	bool status=false;
	
	LexItem t = Parser::GetNextToken(in, line);
	
	switch( t.GetToken() ) {
	case IDENT: 
		Parser::PushBackToken(t);
		status = AssignStmt(in, line);
		if(!status)
		{
			ParseError(line, "Incorrect Assignment Statement");
			return status;
		}
		break;
	case PRINTLN:
		
		status = PrintLnStmt(in, line);
		
		if(!status)
		{
			ParseError(line, "Incorrect PrintLn Statement");
			return status;
		}
		break;
	case IF:  
		status = IfStmt(in, line);
		
		if(!status)
		{
			ParseError(line, "Incorrect If-Statement");
			return status;
		}
		execute = true;
		break;
	case ELSE:
		Parser::PushBackToken(t);
		return false;
		break;
	
	case DONE:
		return true;
	default:
		Parser::PushBackToken(t);
		return true;
	}
	return status;
}//End of Stmt function


//PrintLnStmt:= PRINTLN (ExpreList) 
bool PrintLnStmt(istream& in, int& line) {
	LexItem t;
	ValQue = new queue<Value>();
	t = Parser::GetNextToken(in, line);
	if( t != LPAREN ) {
		
		ParseError(line, "Missing Left Parenthesis of PrintLn Statement");
		return false;
	}
	
	bool ex = ExprList(in, line);
	
	if( !ex ) {
		ParseError(line, "Missing expression list after PrintLn");
		return false;
	}
	
	t = Parser::GetNextToken(in, line);
	if(t != RPAREN ) {
		
		ParseError(line, "Missing Right Parenthesis of PrintLn Statement");
		return false;
	}

	while(!((*ValQue).empty())) {
		std::cout << (*ValQue).front();
		(*ValQue).pop();
	}
	std::cout << "\n";
	delete ValQue;

	return true;
}//End of PrintlnStmt

//IfStmt:= IF (Expr) '{' StmtList '}' [ Else '{' StmtList '}' ] 
bool IfStmt(istream& in, int& line) {
	bool ex = false, status ; 
	LexItem t;
	
	t = Parser::GetNextToken(in, line);
	if( t != LPAREN ) {
		
		ParseError(line, "Missing Left Parenthesis of If condition");
		return false;
	}
	Value retVal = Value();
	ex = Expr(in, line, retVal);
	if( !ex ) {
		ParseError(line, "Missing if statement Logic Expression");
		return false;
	}

	if((retVal.IsBool() && retVal.GetBool()) || (retVal.IsNum() && retVal.GetNum() != 0) || (retVal.IsString() && retVal.GetString() != "")) {
		execute = true;
	} else {
		execute = false;
	}


	t = Parser::GetNextToken(in, line);
	if(t != RPAREN ) {
		
		ParseError(line, "Missing Right Parenthesis of If condition");
		return false;
	}
	
	t = Parser::GetNextToken(in, line);
	if(t != LBRACES)
	{
		Parser::PushBackToken(t);
		ParseError(line, "Missing left brace for If Statement Clause");
		return false;
	}
	status = StmtList(in, line);
	if(!status )
	{
		ParseError(line, "Missing Statement for If Statement Clause");
		return false;
	}
	t = Parser::GetNextToken(in, line);
	if(t != RBRACES)
	{
		ParseError(line, "Missing right brace for If Statement Clause");
		return false;
	}	
	t = Parser::GetNextToken(in, line);
	
	if( t == ELSE ) {
		if((retVal.IsBool() && retVal.GetBool()) || (retVal.IsNum() && retVal.GetNum() != 0) || (retVal.IsString() && retVal.GetString() != "")) {
			execute = false;
		} else {
			execute = true;
		} 

		t = Parser::GetNextToken(in, line);
		if(t != LBRACES)
		{
			Parser::PushBackToken(t);
			ParseError(line, "Missing left brace for Else-Clause");
			return false;
		}
		status = StmtList(in, line);
		if( !status)
		{
			ParseError(line, "Missing Statement for Else-Clause");
			return false;
		}
		t = Parser::GetNextToken(in, line);
		if( t != RBRACES)
		{
			line--;
			Parser::PushBackToken(t);
			ParseError(line, "Missing right brace for an Else-Clause");
			return false;
		}
		
	}
	else
	{
		Parser::PushBackToken(t);
		return true;
	}
	return true;
}//End of IfStmt function

//Var ::= IDENT
bool Var(istream& in, int& line, LexItem& idtok)
{
	string identstr;
	
	LexItem tok = Parser::GetNextToken(in, line);
	
	if (tok == IDENT ){
		identstr = tok.GetLexeme();
		
		if (!(defVar.find(identstr)->second))
		{
			defVar[identstr] = true;
			
		}	
		idtok = tok;
		return true;
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		
		return false;
	}
	
	return false;
}//End of Var

//AssignStmt:= Var AssignOp Expr
bool AssignStmt(istream& in, int& line) {
	bool varstatus = false, status = false;
	LexItem t;
	
	LexItem name = LexItem();
	varstatus = Var(in, line, name);
		
	if (varstatus){
		t = Parser::GetNextToken(in, line);
		
		if (t == ASSOP || t == CADDA || t == CSUBA || t == CCATA){
			Value val1 = Value();
			status = Expr(in, line, val1);
			if(!status) {
				ParseError(line, "Missing Expression in Assignment Statement.");
				return false;
			}

			if(execute) {
				if(t == ASSOP) {
					if(val1.IsBool()) {
						ParseError(line, "Illegal Assignment of a boolean value to a variable.");
						return false;
					}
					TempsResults[name.GetLexeme()] = val1;
				} else if(t == CADDA) {
					if(val1.IsBool()) {
						ParseError(line, "Illegal Assignment of a boolean value to a variable.");
						return false;
					}
					Value test = TempsResults[name.GetLexeme()] + val1;
					if(test.IsErr()) {
						ParseError(line, "this is my error. CADDA issue.");
						return false;
					}
					TempsResults[name.GetLexeme()] = test;
				} else if(t == CSUBA) {
					if(val1.IsBool()) {
						ParseError(line, "Illegal Assignment of a boolean value to a variable.");
						return false;
					}
					Value test = TempsResults[name.GetLexeme()] - val1;
					if(test.IsErr()) {
						ParseError(line, "this is my error. CSUBA issue.");
						return false;
					}
					TempsResults[name.GetLexeme()] = test;
				} else if(t == CCATA) {
					if(val1.IsBool()) {
						ParseError(line, "Illegal Assignment of a boolean value to a variable.");
						return false;
					}
					Value test = TempsResults[name.GetLexeme()].Catenate(val1);
					if(test.IsErr()) {
						ParseError(line, "this is my error. CCATA issue.");
						return false;
					}
					TempsResults[name.GetLexeme()] = test;
				}
			}

		}
		else if(t.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << t.GetLexeme() << ")" << endl;
			return false;
		}
		else {
			ParseError(line, "Missing Assignment Operator");
			return false;
		}
	}
	else {
		ParseError(line, "Missing Left-Hand Side Variable in Assignment statement");
		return false;
	}
	return status;	
}//End of AssignStmt

//ExprList:= Expr {,Expr}
bool ExprList(istream& in, int& line) {
	bool status = false;
	Value retVal = Value();
	status = Expr(in, line, retVal);
	if(!status){
		ParseError(line, "Missing Expression");
		return false;
	}
	if(execute)
		(*ValQue).push(retVal);
	LexItem tok = Parser::GetNextToken(in, line);
	
	if (tok == COMMA) {
		
		status = ExprList(in, line);
		
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	else{
		Parser::PushBackToken(tok);
		return true;
	}
	return status;
}//End of ExprList

//Expr ::= OrExpr ::= AndExpr { || AndExpr }
bool Expr(istream& in, int& line, Value& retVal) {
	
	LexItem tok;
	Value val1 = Value();
	bool t1 = AndExpr(in, line, val1);
		
	if( !t1 ) {
		return false;
	}
	
	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	while ( tok == OR  ) 
	{
		Value val2 = Value();
		t1 = AndExpr(in, line, val2);
		if( !t1 ) 
		{
			ParseError(line, "Missing operand after OR operator");
			return false;
		}
		
		tok = Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}
		if(execute)		
			val1 = val1 || val2;
	}
	Parser::PushBackToken(tok);
	retVal = val1;
	return true;
}//End of OrExpr or Expr

//	AndExpr ::= RelExpr  [ && RelExpr ]
bool AndExpr(istream& in, int& line, Value& retVal) {
	LexItem tok;
	Value val1 = Value();
	bool t1 = RelExpr(in, line, val1);
		
	if( !t1 ) {
		return false;
	}
	
	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	while ( tok == AND  ) 
	{
		Value val2 = Value();
		t1 = RelExpr(in, line, val2);
		if( !t1 ) 
		{
			ParseError(line, "Missing operand after AND operator");
			return false;
		}
		
		tok = Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}
		if(execute)		
			val1 = val1 && val2;
	}
	Parser::PushBackToken(tok);
	retVal = val1;
	return true;
}//End of AndExpr

//RelExpr ::= AddExpr  [ ( @le | @gt | @eq | < | >= | == ) AddExpr ]
bool RelExpr(istream& in, int& line, Value& retVal) {
	LexItem tok;
	Value val1 = Value();
	bool t1 = AddExpr(in, line, val1);
		
	if( !t1 ) {
		return false;
	}
	
	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	LexItem oper = tok;
	if ( tok == NGTE || tok == NLT || tok == NEQ || tok == SLTE || tok == SGT || tok == SEQ ) 
	{
		Value val2 = Value();
		t1 = AddExpr(in, line, val2);
		if( !t1 ) 
		{
			ParseError(line, "Missing operand after a relational operator");
			return false;
		}
		
		tok = Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}	

		if(execute) {
			if(oper == NGTE) {
				val1 = val1 >= val2;
			} else if (oper == NLT) {
				val1 = val1 < val2;
			} else if (oper == NEQ) {
				val1 = !(val1 == val2);
			} else if(oper == SLTE) {
				val1 = (val1.SLE(val2) || val1.SEQ(val2));
			} else if (oper == SGT) {
				val1 = val1.SGT(val2);
			} else if (oper == SEQ) {
				val1 = val1.SEQ(val2);
			}
			if(val1.IsErr()) {
				ParseError(line, "Illegal Relational operation.");
				return false;
			}
		}

	}
	
	Parser::PushBackToken(tok);
	retVal = val1;
	return true;
}//End of RelExpr

//AddExpr :: MultExpr { ( + | - | .) MultExpr }
bool AddExpr(istream& in, int& line, Value& retVal) {
	Value val1 = Value();
	bool t1 = MultExpr(in, line, val1);
	LexItem tok;
	
	if( !t1 ) {
		return false;
	}
	
	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	LexItem oper = tok;
	while ( tok == PLUS || tok == MINUS || tok == CAT) 
	{
		Value val2 = Value();
		t1 = MultExpr(in, line, val2);
		if( !t1 ) 
		{
			ParseError(line, "Missing operand after operator");
			return false;
		}
		
		tok = Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}		
		
		if(execute) {
			Value test;
			if(oper == PLUS) {
				test = val1+val2;
			} else if(oper == MINUS) {
				test = val1-val2;
			} else if(oper == CAT) {
				test = val1.Catenate(val2);
			}
			if(test.IsErr()) {
				ParseError(line, "Illegal opernad type for the operation.");
				return false;
			}
			val1 = test;
		}

	}
	Parser::PushBackToken(tok);
	retVal = val1;
	return true;
}//End of AddExpr

//MultExpr ::= ExponExpr { ( * | / | .x.) ExponExpr }
bool MultExpr(istream& in, int& line, Value& retVal) {
	
	Value val1 = Value();
	bool t1 = UnaryExpr(in, line, val1);
	LexItem tok;
	
	if( !t1 ) {
		return false;
	}
	
	tok	= Parser::GetNextToken(in, line);
	LexItem oper = tok;
	if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
	}
	while ( tok == MULT || tok == DIV  || tok == SREPEAT || tok == REM)
	{
		Value val3 = Value();
		t1 = UnaryExpr(in, line, val3);
		if( !t1 ) {
			ParseError(line, "Missing operand after operator");
			return false;
		}
		
		tok	= Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}

		if(execute) {
			Value test;
			if(oper == MULT) {
				test = val1*val3;
			} else if(oper == DIV) {
				if(val3.IsNum() && val3.GetNum() == 0) {
					ParseError(line, "Illegal operand type or value for the division operation.");
					return false;
				}
				test = val1/val3;
				if(test.IsErr()) {
					ParseError(line, "Illegal operand type or value for the division operation.");
					return false;
				}
			} else if(oper == REM) {
				test = val1%val3;
				if(test.IsErr()) {
					ParseError(line, "Illegal operand type for the operation.");
					return false;
				}
			} else if (oper == SREPEAT) {
				test = val1.Repeat(val3);
				if(test.IsErr()) {
					ParseError(line, "Illegal operand type for the string repetition operation.");
					return false;
				}
			}
			
			val1 = test;
		}

	}
	Parser::PushBackToken(tok);
	retVal = val1;
	return true;
}//End of MultExpr

//UnaryExpr ::= [( - | + | ! )] ExponExpr 
bool UnaryExpr(istream& in, int& line, Value& retVal)
{
	LexItem t = Parser::GetNextToken(in, line);
	bool status;
	int sign = 0;
	if(t == MINUS)
	{
		sign = -1;
	}
	else if(t == PLUS)
	{
		sign = 1;
	}
	else if(t == NOT)
	{
		sign = 2;
	}
	else
		Parser::PushBackToken(t);
	
	if(execute) {
			LexItem tok = Parser::GetNextToken(in, line);
			while(tok == MINUS || tok == PLUS || tok == NOT){
				tok = Parser::GetNextToken(in, line);
			}
			Parser::PushBackToken(tok);
	}

	Value v1 = Value();
	status = ExponExpr(in, line, sign, v1);
	
	if(execute) {
		if(!status)
		{
			ParseError(line, "Missing operand for an operator");
			return false;
		}
	}

	if(execute) {
		Value test = Value();
		if(sign == -1) {
			if(!v1.IsNum()) {
				ParseError(line, "Illegal Operand Type for Sign Operator");
				return false;
			}
			test = v1*(Value(-1.0));
		} else if (sign==1) {
			test = v1;
		} else if (sign == 2){
			test = !v1;
		} else {
			test = v1;
		}
		if(test.IsErr()) {
			std::cout << "v1: " << v1;
			ParseError(line, "Illegal Operand Type for Sign Operator");
			return false;
		}
		v1 = test;
	}

	retVal = v1;
	return status;
}//End of UnaryExpr

//ExponExpr ::= PrimaryExpr { ** PrimaryExpr }
//enforcing right associativity using right recursiveness
bool ExponExpr(istream& in, int& line, int sign, Value& retVal)
{
	bool status;
	Value val1 = Value();
	status = PrimaryExpr(in, line, sign, val1);

	if( !status ) {
		return false;
	}
	
	

	LexItem tok = Parser::GetNextToken(in, line);
	if (tok == EXPONENT)
	{
		Value val2 = Value();
		status = ExponExpr(in, line, sign, val2);
		if( !status && execute) {
			ParseError(line, "Missing exponent operand after exponentiation");
			return false;
		}
		
		tok	= Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}
		
		if(execute) {
			val1 = val1.Expon(val2);
			if(val1.IsErr()) {
				ParseError(line, "Illegal exponentiation operation.");
				return false;
			}
		}

	}
	else if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
	}
	Parser::PushBackToken(tok);
	retVal = val1;
	return true;
}//End of ExponExpr


//PrimaryExpr ::= IDENT | ICONST | FCONST | SCONST | (Expr)
bool PrimaryExpr(istream& in, int& line, int sign, Value& retVal) {
	
	LexItem tok = Parser::GetNextToken(in, line);
	
	if( tok == IDENT ) {
		 
		string lexeme = tok.GetLexeme();
		if (!(defVar.find(lexeme)->second))
		{
			ParseError(line, "Using Undefined Variable: "+ lexeme);
			Parser::PushBackToken(tok);
			
			return false;	
		}
		retVal = TempsResults[lexeme];
		return true;
	}
	else if( tok == ICONST ) {
		retVal = Value(std::stod(tok.GetLexeme()));
		return true;
	}
	else if( tok == SCONST ) {
		retVal = Value(tok.GetLexeme());
		return true;
	}
	else if( tok == FCONST ) {
		retVal = Value(std::stod(tok.GetLexeme()));
		return true;
	}
	else if( tok == LPAREN ) {
		bool ex = Expr(in, line, retVal);
		if( !ex ) {
			ParseError(line, "Missing expression after Left Parenthesis");
			return false;
		}
		if( Parser::GetNextToken(in, line) == RPAREN )
			return ex;
		else 
		{
			Parser::PushBackToken(tok);
			ParseError(line, "Missing right Parenthesis after expression");
			return false;
		}
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	
	return false;
}