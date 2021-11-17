#include "token.h"
#include <map>
#include <string>
using namespace std;

#ifndef TOKENMAP_H
#define TOKENMAP_H

map<Token, string> token_to_string {
	{ Token::INT , "int" },
	{ Token::CHAR , "char" },
	{ Token::WHILE , "while" },
	{ Token::IF , "if" },
	{ Token::ELIF , "else" },
	{ Token::ELSE , "else" }, 
	{ Token::INPUT , "input" },
	{ Token::PRINT ,  "print"}, 
	{ Token::PRINTLN , "println" }, 
	{ Token::MUL ,  "multiply"}, 
	{ Token::DIV , "divide" }, 
	{ Token::ADD , "Addition" }, 
	{ Token::SUB , "subtraction" }, 
	{ Token::LT ,  "less than"},  
	{ Token::LTE , "less than equal" },  
	{ Token::GT ,  "greater than"},   
	{ Token::GTE , "greater than equal" },  
	{ Token::EQ ,  "equal"}, 
	{ Token::NEQ ,  "not equal"},  
	{ Token::SLC ,  "single line comment"},  
	{ Token::MLC ,  "multiple line comment"},  
	{ Token::ID ,  "identifier"}, 
	{ Token::NUM ,  "number"},  
	{ Token::LIT ,  "char literal"},  
	{ Token::STR ,  "string literal"},  
	{ Token::AS ,  "assignment"}, 
	{ Token::IN ,  "input"},  
	{ Token::SBO , "SBO" },  
	{ Token::SBC , "SBC" },  
	{ Token::FBO , "FBO" },  
	{ Token::FBC , "FBC" },  
	{ Token::PO , "PO" },  
	{ Token::PC , "PC" },  
	{ Token::COL , "COL" }, 
	{ Token::SCOL , "SCOL" },  
	{ Token::COM , "COM" }, 
	{ Token::INC , "INC" }, 
	{ Token::DEC , "DEC" } 
};

map<Token, string> token_to_symbol{
	{ Token::INT , "int" },
	{ Token::CHAR , "char" },
	{ Token::WHILE , "while" },
	{ Token::IF , "if" },
	{ Token::ELIF , "else" },
	{ Token::ELSE , "else" },
	{ Token::INPUT , "input" },
	{ Token::PRINT ,  "print"},
	{ Token::PRINTLN , "println" },
	{ Token::MUL ,  "*"},
	{ Token::DIV , "/" },
	{ Token::ADD , "+" },
	{ Token::SUB , "-" },
	{ Token::LT ,  "<"},
	{ Token::LTE , "<=" },
	{ Token::GT ,  ">"},
	{ Token::GTE , ">=" },
	{ Token::EQ ,  "=="},
	{ Token::NEQ ,  "~="},
	{ Token::SLC ,  "single line comment"},
	{ Token::MLC ,  "multiple line comment"},
	{ Token::ID ,  "identifier"},
	{ Token::NUM ,  "number"},
	{ Token::LIT ,  "char literal"},
	{ Token::STR ,  "string literal"},
	{ Token::AS ,  "="},
	{ Token::IN ,  "->"},
	{ Token::SBO , "[" },
	{ Token::SBC , "]" },
	{ Token::FBO , "{" },
	{ Token::FBC , "}" },
	{ Token::PO , "(" },
	{ Token::PC , ")" },
	{ Token::COL , ":" },
	{ Token::SCOL , ";" },
	{ Token::COM , ":" },
	{ Token::INC , "++" },
	{ Token::DEC , "--" }
};


#endif //TOKENMAP_H