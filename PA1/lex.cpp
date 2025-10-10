#include "lex.h"
#include <vector>
ostream& operator<<(ostream& out, const LexItem& tok) {
    if(tok.GetToken() == ICONST) {
        out << "ICONST" << ": [" << tok.GetLexeme() << "]\n";
    } else if (tok.GetToken() == FCONST) {
        out << "FCONST" << ": [" << tok.GetLexeme() << "]\n";
    }else if (tok.GetToken() == IDENT) {
        out << "IDENT" << ": (" << tok.GetLexeme() << ")\n";
    } else if (tok.GetToken() == SCONST) {
        out << "SCONST" << ": <" << tok.GetLexeme() << ">\n";
    } else if (tok.GetToken() == ERR) {
        out << "ERR: " << "Error-Unrecognized Lexeme {" << tok.GetLexeme() << "} in line " << tok.GetLinenum() << "\n";
    } else if(tok.GetToken() == COMMA) {
        out << "COMMA" << ": \",\"\n";
    } else if(tok.GetToken() == SEMICOL) {
        out << "SEMICOL" << ": \";\"\n";
    } else if(tok.GetToken() == LPAREN) {
        out << "LPAREN" << ": \"(\"\n";
    } else if(tok.GetToken() == RPAREN) {
        out << "RPAREN" << ": \")\"\n";
    } else if(tok.GetToken() == LBRACES) {
        out << "LBRACES" << ": \"{\"\n";
    } else if(tok.GetToken() == RBRACES) {
        out << "RBRACES" << ": \"}\"\n";
    } else if(tok.GetToken() == PLUS) {
        out << "PLUS" << ": \"+\"\n";
    } else if(tok.GetToken() == MINUS) {
        out << "MINUS" << ": \"-\"\n";
    } else if(tok.GetToken() == MULT) {
        out << "MULT" << ": \"*\"\n";
    } else if(tok.GetToken() == DIV) {
        out << "DIV" << ": \"/\"\n";
    } else if(tok.GetToken() == REM) {
        out << "REM" << ": \"%\"\n";
    } else if(tok.GetToken() == EXPONENT) {
        out << "EXPONENT" << ": \"**\"\n";
    } else if(tok.GetToken() == NEQ) {
        out << "NEQ" << ": \"==\"\n";
    } else if(tok.GetToken() == SEQ) {
        out << "SEQ" << ": \"@eq\"\n";
    } else if(tok.GetToken() == NLT) {
        out << "NLT" << ": \"<\"\n";
    } else if(tok.GetToken() == NGTE) {
        out << "NGTE" << ": \">=\"\n";
    } else if(tok.GetToken() == SLTE) {
        out << "SLTE" << ": \"@le\"\n";
    } else if(tok.GetToken() == SGT) {
        out << "SGT" << ": \"@gt\"\n";
    } else if(tok.GetToken() == CAT) {
        out << "CAT" << ": \".\"\n";
    } else if(tok.GetToken() == SREPEAT) {
        out << "SREPEAT" << ": \".x.\"\n";
    } else if(tok.GetToken() == AND) {
        out << "AND" << ": \"&&\"\n";
    } else if(tok.GetToken() == OR) {
        out << "OR" << ": \"||\"\n";
    } else if(tok.GetToken() == NOT) {
        out << "NOT" << ": \"!\"\n";
    } else if(tok.GetToken() == ASSOP) {
        out << "ASSOP" << ": \"=\"\n";
    } else if(tok.GetToken() == CADDA) {
        out << "CADDA" << ": \"+=\"\n";
    } else if(tok.GetToken() == CSUBA) {
        out << "CSUBA" << ": \"-=\"\n";
    } else if(tok.GetToken() == CCATA) {
        out << "CCATA" << ": \".=\"\n";
    } else if(tok.GetToken() == PRINTLN) {
        out << "PRINTLN" << ": \"println\"\n";
    } else if(tok.GetToken() == IF) {
        out << "IF" << ": \"if\"\n";
    } else if(tok.GetToken() == ELSE) {
        out << "ELSE" << ": \"else\"\n";
    } else if(tok.GetToken() == DONE) {
        out << "DONE" << ": [EOF]\n";
    }
    return out;
}

LexItem id_or_kw(const string& lexeme, int linenum) {
    std::string check = lexeme;
    for(int i = 0; i < check.length() ;i++) {
        check[i] = tolower(check[i]);
    }
    if(check == "println") {
        return LexItem(PRINTLN, lexeme, linenum);
    } else if(check == "if") {
        return LexItem(IF, lexeme, linenum);
    } else if(check == "else") {
        return LexItem(ELSE, lexeme, linenum);
    } else {
        return LexItem(IDENT, lexeme, linenum);
    }
}

std::pair<std::string, int> testNumber(istream& in, char first) {
    std::string lexeme = "";
    lexeme += first;
    char c;
    while(in.get(c)) {
        if(isdigit(c)) {
            lexeme += c;
        } else if (c == '.') {
            char afterdecimal;
            if(!isdigit(in.peek()) ) {
                in.putback(c);
                return std::make_pair(lexeme, FCONST);
            }
            lexeme += c;
            while(in.get(afterdecimal)) {
                if(isdigit(afterdecimal)) {
                    lexeme += afterdecimal;
                } else if (afterdecimal == '.') {
                    lexeme += afterdecimal;
                    in.putback(afterdecimal);
                    return std::make_pair(lexeme, ERR);
                } else if (afterdecimal == 'e' || afterdecimal == 'E') {
                    lexeme += afterdecimal;
                    char sign = in.get();
                    if(sign == '+' || sign == '-') {
                        lexeme += sign;
                    } else {
                        in.putback(sign);
                    }

                    char expdigitl;
                    while(in.get(expdigitl)) {
                        if(isdigit(expdigitl)) {
                            lexeme += expdigitl;
                        } else {
                            in.putback(expdigitl);
                            return std::make_pair(lexeme, FCONST);
                        }
                    }
                } else {
                    in.putback(afterdecimal);
                    return std::make_pair(lexeme, FCONST);
                }
            }
        } else {
            in.putback(c);
            return std::make_pair(lexeme, ICONST);
        }
    }
    return std::pair<std::string,int>(lexeme, ICONST);
}

std::string testString(istream& in, char first) {
    std::string lexeme = "";
    lexeme += first;
    char c;
    while(in.get(c)) {
        if(isalnum(c) || c == '$' || c == '_') {
            lexeme += c;
        } else {
            in.putback(c);
            return lexeme;
        }
    }
    return lexeme;
}

LexItem getNextToken(istream& in, int& linenum) {
    do {
        char c = in.get();
        if(in.eof()) {
            return LexItem(DONE, "", linenum);
        }
        if(c == '\n') {
            linenum++;
            continue;
        }
        switch(c) { // single character / low character tokens
            case '+' : {
                char next = in.get();
                if(next == '=') {
                    return LexItem(CADDA, "+=", linenum);
                } else {
                    in.putback(next);
                    return LexItem(PLUS, "+", linenum);
                }
                break;
            }
            case '-' : {
                char next = in.get();
                if(next == '=') {
                    return LexItem(CSUBA, "-=", linenum);
                } else {
                    in.putback(next);
                    return LexItem(MINUS, "-", linenum);
                }
                break;
            } case '*' : {
                char next = in.get();
                if(next == '*') {
                    return LexItem(EXPONENT, "**", linenum);
                } else {
                    in.putback(next);
                    return LexItem(MULT, "*", linenum);
                }
                break;
            }
            case '/' : {
                return LexItem(DIV, "/", linenum);
                break;
            }
            case '%' : {
                return LexItem(REM, "%", linenum);
                break;
            }
            case '#' : {
                //comment, ignore the rest of the line
                std::string discard;
                getline(in, discard);
                linenum++;
                continue;
            }
            case '=' : {
                char next = in.get();
                if(next == '=') {
                    return LexItem(NEQ, "==", linenum);
                } else {
                    in.putback(next);
                    return LexItem(ASSOP, "=", linenum);
                }
                break;
            }
            case '&' : {
                char next = in.get();
                if(next == '&') {
                    return LexItem(AND, "&&", linenum);
                } else {
                    in.putback(next);
                    return LexItem(ERR, string(1, c), linenum);
                }
                break;
            }
            case '|' : {
                char next = in.get();
                if(next == '|') {
                    return LexItem(OR, "||", linenum);
                } else {
                    in.putback(next);
                    return LexItem(ERR, string(1, c), linenum);
                }
                break;
            }
            case '@' : {
                char* next = new char[2];
                next[0] = in.get();
                next[1] = in.get();
                next[0] = std::tolower(next[0]);
                next[1] = std::tolower(next[1]);
                if(next[0] == 'e' && next[1] == 'q') {
                    delete[] next;
                    return LexItem(SEQ, "@eq", linenum);
                } else if (next[0] == 'l' && next[1] == 'e') {
                    delete[] next;
                    return LexItem(SLTE, "@le", linenum);
                } else if(next[0] == 'g' && next[1] == 't') {
                    delete[] next;
                    return LexItem(SGT, "@gt", linenum);
                } else {
                    in.putback(next[1]);
                    in.putback(next[0]);
                    delete[] next;
                    return LexItem(ERR, string(1, c), linenum);
                }
                break;
            }
            case '<' : {
                return LexItem(NLT, "<", linenum);
                break;
            }
            case '>' : {
                char next = in.get();
                if(next == '=') {
                    return LexItem(NGTE, ">=", linenum);
                } else {
                    in.putback(next);
                    return LexItem(ERR, string(1, c), linenum);
                }
                break;
            }
            case '!' : {
                return LexItem(NOT, "!", linenum);
                break;
            }
            case '.' : {
                char next = in.get();
                if(next == '=') {
                    return LexItem(CCATA, ".=", linenum);
                } else if(next == 'x') {
                    char next2 = in.get();
                    if(next2 == '.') {
                        return LexItem(SREPEAT, ".x.", linenum);
                    } else {
                        in.putback(next2);
                        in.putback(next);
                        return LexItem(ERR, string(1, c), linenum);
                    }
                } else {
                    in.putback(next);
                    return LexItem(CAT, ".", linenum);
                }
                break;
            }
            case '(' : {
                return LexItem(LPAREN, "(", linenum);
                break;
            }
            case ')' : {
                return LexItem(RPAREN, ")", linenum);
                break;
            }
            case '{' : {
                return LexItem(LBRACES, "{", linenum);
                break;
            }
            case '}' : {
                return LexItem(RBRACES, "}", linenum);
                break;
            }
            case ',' : {
                return LexItem(COMMA, ",", linenum);
                break;
            }
            case ';' : {
                return LexItem(SEMICOL, ";", linenum);
                break;
            }
            default : {
                break;
            }
        } // end switch statement

        if(isdigit(c)) {
            std::pair<std::string, int> num = testNumber(in, c);
            return LexItem((Token)num.second, num.first, linenum);
        }

        if(isalpha(c) || c == '$') {
            std::string str = testString(in, c);
            return id_or_kw(str, linenum);
        }

        if(c == '"') {
            std::string str = "";
            char next;
            while(in.get(next)) {
                if(next == '"') {
                    return LexItem(SCONST, str, linenum);
                } else if (next == '\n' || in.eof()) {
                    in.putback(next);
                    return LexItem(ERR, "\"" + str, linenum);
                } else {
                    str += next;
                }
            }
            return LexItem(ERR, "\"" + str, linenum);
        }

        if(c == '\'') {
            std::string str = "";
            char next;
            while(in.get(next)) {
                if(next == '\'') {
                    return LexItem(SCONST, str, linenum);
                } else if (next == '\n' || in.eof()) {
                    in.putback(next);
                    return LexItem(ERR, "\'" + str, linenum);
                } else {
                    str += next;
                }
            }
            return LexItem(ERR, "\'" + str, linenum);
        }
        if(isspace(c)) {
            continue;
        }
        return LexItem(ERR, string(1, c), linenum);
    } while(true);
}