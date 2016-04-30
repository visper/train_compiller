/*
program need source code. file with code named *.train
we don`t create high level language, we create only assembler
program.train->assember->hex_code.

program read input file and save it in some structure of data

input Grammar I can owerwrite like
G = {N,E,P,S}
N - non terminal symbols
E - terminal symbols
P - rules of grammar
S - N is a non trminal symbol that says about start of code

N = keywords, tokens.
keywords = "goto", "ifflag", "forward", "backward".
tokens = ":", " ", ";", "\n", "/", "/t", "{", "}".

E = numbers, chars.
number = "0" | "2" | ... | "9" |.
char = "a" | "b" | ... | "z".


//lexem = owerwrite
init_code_section = "{".
code = labeled_line | non_labeled_line.
non_labeled_line = keyword, ";", ...
labeled_line = (chars[chars,chars], number[number]), [" "], ":", [" "],
               keyword, ";", ...
fini_code_section = "}".

P = 
[ //if nothing grammar is NULL
init_code_section.
{code}.
fini_code_section.
]

S = "{".
( end - "}").
(comments, empty lines and more than one space must be deleted in lexer)
*/

#include <iostream>
#include <fstream>
#include <list>
//#include "stack_support.c"
//#include <string.h>
#include <sstream>
#include <vector>
#include <bitset>

using namespace std;

list<string> code;
list<string> errors;

struct table_line {
	bitset<4> adr_bin;
	size_t address; //address in mem
	string command; //name of command
	string label; //if here label, name of label
	string jmp_adr; // if goto, address for jump
	bitset<4> jmp_adr_bin;
};

vector<table_line> table;

int read_file(char* name_of_file)
{
//read file and write it in list of list
	ifstream infile; // ifsream for read file
	infile.open(name_of_file, ios::out);
	if(!infile.is_open()) { // if file is not open
		cout << "error name of file!" << endl;
		return -1;
	}
	else {
		string str;
		while( getline(infile,str) )
		{
//			cout << str << endl; // Prints our STRING.
			//here need to save somewhere
			code.push_back(str);
		}
	}
	infile.close();
	return 0; // reading seccess
}

/*
string cutter(size_t pos, string line)
{
	cout << "pos: " << pos << endl;
	string cut_line = line.substr(pos, line.length() ); //cut from pos to line.len
	cout << "cut_line " << cut_line << endl;
	size_t new_pos = cut_line.find(" ");
	string finish_str = cut_line.substr(0, new_pos ); //cut from pos to " "
	cout << "fin_str: " << finish_str << endl;
	size_t pos_eq = cut_line.find("=");
	string fin_line = finish_str.substr(++pos_eq, line.length() );
//	size_t new_pos = new_line.find(" ");
	cout << "fin_line: " << fin_line << endl;
	return fin_line;
}
*/

 //fuu detector that findes lexems in lines
 //detector creates list of errors if it findes
 //detector is P = (Q, E, I, D, q0, z0, F)
 //Q = enumeration of states
 //E = output alphabet
 //I = alphabet of stack symbols
 //D = conditions of changing states
 //q0 = start state of devide
 //z0 = symbol that initialized stack
 //F = ended states
 //Q = {
 //q1 = init step
 //q2 = next step

 //q3 = error in word
 //q4 = error in with wrong symbol in line

 //q5 = symbol_1
 //q6 = symbol_2 like in keyword
 //q7 = if keyword - write in stack
 //q8 = if after keyword ";"
 //q9 = line is readed

 //q10 = symbol_1 - "char", symbol_2 - "char"
 //q11 = symbol_1 - "char", symbol_2 - "number"
 //q12 = symbol_1 - "char", symbol_2 - "number", symbol_3 - " "
 //q13 = symbol_1 - "char", symbol_2 - "char", symbol_3 - " " (error)
 //q14 = symbol_1 - "char", symbol_2 - "number", symbol_3 - " ", symbol_4 - ":"
 //q15 = symbol_1 - "char", symbol_2 - "number", symbol_3 - " ", symbol_4 - ":",
 //      symbol_4 - " "
 //q16 = symbol_1 - "char", symbol_2 - "char", symbol_3 - "char"
 //q17 = symbol_1 - "char", symbol_2 - "char", symbol_3 - "number"
 //q18 = symbol_1 - "char", symbol_2 - "char", symbol_3 - "char",
 //      symbol_4 - "char" (error)
 //q19 = symbol_1 - "char", symbol_2 - "char", symbol_3 - "number",
 //      symbol_4 - "number"
 //q20 = symbol_1 - "char", symbol_2 - "char", symbol_3 - "char",
 //      symbol_4 - "number"
 //q21 = symbol_1 - "char", symbol_2 - "char", symbol_3 - "char",
 //      symbol_4 - "number", symbol_5 - "number"
 //q22 = symbol_1 - "char", symbol_2 - "char", symbol_3 - "char",
 //      symbol_4 - "number", symbol_5 - "number", symbol_6 - " "
 //q23 = symbol_1 - "char", symbol_2 - "char", symbol_3 - "char",
 //      symbol_4 - "number", symbol_5 - "number", symbol_6 - " ",
 //      symbol_6 - ":"
 //q24 = symbol_1 - "char", symbol_2 - "char", symbol_3 - "char",
 //      symbol_4 - "number", symbol_5 - "number", symbol_6 - " ",
 // symbol_7 - ":", symbol_8 - " "
 //q25 = q24 + q6
 //q26 = q15 + q6
 //}
 //E = table
 //D = next step
 //q0 = stek is empty, iterator on first element
 //z0 = NULL, stack is empty
 //F = lexem in table or error

string goto_parse( string command )
{
	//find " " space, find ";". this part is label
	//from 0 to " " space is command

	string label;
	size_t space = command.find(" ");
	size_t end_of_line = command.find(";");
	if ( space != string::npos && end_of_line != string::npos ) {
		//line is correct
		//cut from " " to ";"
		label = command.substr( ++space, --end_of_line - space );
	}
	else {
		//error ";" or space " " is not available
		string egor = "error ";" or space " " is not available  \n";
		errors.push_back(egor);			
		return "";
	}

	return label;
}

void non_labeled_part(string )
{
	list<string> keyw;
	keyw.push_back("ifflag;");
	keyw.push_back("goto");
	keyw.push_back("backward;");
	keyw.push_back("forward;");

	bool find = 0;
	for(list<string>::iterator itr_key =  keyw.begin();
	    itr_key != keyw.end(); itr_key ++) {
		size_t found = str.find(*itr_key);
		if( found != string::npos ) {
			find = 1;
			//write into table

			table_line tbl;
			tbl.address = num_of_line++;
			tbl.label = label;
			if(*itr_key == "goto") {
				//find label
				string lb = goto_parse(command);

				tbl.command = "goto";
				tbl.jmp_adr = lb;
				table.push_back(tbl);
				break;
			}
			else {
				tbl.command = *itr_key;
				tbl.jmp_adr = "";
				table.push_back(tbl);
				break;
			}
		}
	}
}

 //void detector(char ch, string line)
void detector(const char &itr, const string& line)
{
//int  stack_init(struct stack *s, int count);
//void stack_fini(struct stack *s);
//int  stack_pop(struct stack *s);
//void stack_push(struct stack *s, int elem);

	static size_t num_of_line = 0; // maybe it init every time
	static vector<char> buf;
	static int num_of_char;
	static bool labeled = 0; // maybe delete init

	string str = line;	
	string::iterator str_itr = str.begin();
	if(itr == *str_itr) { //if first char in line
		num_of_char = 0;
//		num_of_line++;

		//find ":". if ":" than string is labled, and vice versa
		size_t found = str.find(":");
		if (found != string::npos) {
			labeled = 1; // line with label
		}
		else {
			labeled = 0;
		}
	}

	if(num_of_line == 0) {
		table.clear(); // maybe erase
	}

	//may to test is line more than 17 symbols, and less than ...

	if(labeled) { //line with label
		//find label before ":"
		size_t space = str.find(" ");
		size_t colon = str.find(":");
		string label;
		if (space != string::npos && colon != string::npos) {
			if (colon < space) {
				//cut to colon
				label = str.substr(0, colon); //cut from 0 to :
			}
			else {
				//cut to space
				label = str.substr(0, space);//cut from 0 to " "
			}
		}
		else if(space != string::npos) {
			//if only space
			//error. in labeled line must be colon
			string egor = "error. in labeled line must be colon \n"
				+ str;
			errors.push_back(egor);
			return;
		}
		else if(colon != string::npos) {
			//if only colon
			label = str.substr(0, colon); //cut from 0 to :		
		}
		else {
			//syntaxis error
			string egor = "error. in labeled line must be tokens \n"
				+ str;
			errors.push_back(egor);			
			return; //maybe change
		}
		//cut command
		string command;
		size_t end_line = str.find(";");
		if ( end_line != string::npos ) {
			//from ":+1" to ";"
			command = str.substr( ++colon, end_line);

			//in start of command can be " " space
			string::iterator sp = command.begin();
			if ( *sp == ' ' ) {
				command = command.substr( 1, end_line);
			}

			//type of command

			//call fuu
		}
		else {
			//error, no ";" in line
			string egor = "error. in line must be ; \n"
				+ str;
			errors.push_back(egor);			
			return;
		}
	}
	else { //without label
		if(num_of_char == 0) {
			if(    itr == 'i' //ifflag
			    || itr == 'g' //goto
			    || itr == 'b' //backward
			    || itr == 'f' //forward
				) { //first char of keyword
				char c_t = itr;
				buf.clear();
				buf.push_back(c_t);
			}
			else {
				//error of first char
				string egor = "error. check keyword \n"
					+ str;
				errors.push_back(egor);			
				return;
			}
		}
		else if(num_of_char == 1) { 
			if(    itr == 'f' // ifflag
			    || itr == 'o' //goto | forward
			    || itr == 'a' //backward
				) { //second char of keyword
				char c_t = itr;
				buf.push_back(c_t);

				stringstream ss;
				char ln[3];
				ln[1] = buf[1];
				ln[0] = buf[0];
				ln[2] = '\0';

				ss << ln;
				string sub_str = ss.str();

				//find substring into string, write into table

				size_t found = str.find(sub_str);
				if (found != string::npos) {
					//sub str is finded
					//find full line in line with ";"
					list<string> keyw;
					keyw.push_back("ifflag;");
					keyw.push_back("goto");
					keyw.push_back("backward;");
					keyw.push_back("forward;");

					bool find = 0;
					for(list<string>::iterator itr_key = 
						    keyw.begin(); itr_key != 
						    keyw.end(); itr_key ++) {

						found = str.find(*itr_key);
//						cout << *itr_key << endl;
						if( found != string::npos ) {
							find = 1;
							//write into table

							table_line tbl;
							tbl.address = num_of_line++;
							tbl.label = "";

							if(*itr_key == "goto") {
								//find label
								string lb = goto_parse(str);
								tbl.command = "goto";
								tbl.jmp_adr = lb;
								table.push_back(tbl);
								break;
							}
							else {
								tbl.command = *itr_key;
								tbl.jmp_adr = "";
								table.push_back(tbl);
								break;
							}
						}
					}
					if(!find) {
						//syntaxis error
						string egor = "error. wrong keyword\n"
							+ str;
						errors.push_back(egor);
						return;
					}   
				}
				else {
					//syntaxis error
					string egor = "some syntaxis error\n"
						+ str;
					errors.push_back(egor);			
					return;
				}
			}
			else {
				//another symbol - error
				string egor = "error. check keyword \n"
					+ str;
				errors.push_back(egor);			
				return;
			}
		}
		else { 
			//line already must be detecte. if program come here
			//it`s error or function caller must take next line
			cout << "must take next line\n";
		}
	}
	num_of_char++;
}

string if_space(string& line)
{
// - must delete more than one space 
// - delete spaces infront the string

	bool space = 0;
	for(string::iterator line_itr = line.begin(); line_itr != line.end();
	    line_itr++) {

		//delete all spaces infront of string
		if ( line_itr == line.begin() && 
		     (*line_itr == ' ' || *line_itr == '\t') ) {
			line_itr = line.erase(line_itr);
			while(*line_itr == ' ') {
				line_itr = line.erase(line_itr);
			}
		}

		//delete more than one space
		if(*line_itr == ' ' && space == 0) {
			space = 1;
		}
		else if(*line_itr == ' ' && space == 1) {
			line_itr = line.erase(line_itr);
			line_itr--;
		}
		else if(*line_itr != ' ') {
			space = 0;
		}
		else {

		}
	}
//	cout << "line is " << line << endl;
	return line;
}

//input - list of symbols of source code
//output - enums of lexems 
//this fuu must find symbols that are in language
int lexycal_analysis()
{
	for(list<string>::iterator code_itr = code.begin();
	    code_itr != code.end(); code_itr++) {
		string current_line(*code_itr);

		//delete all lines that size of it less than 5
		if(current_line.size() < 5) {
			code_itr = code.erase(code_itr);
			code_itr--;
			continue;
		}

		//del more than 1 space and space or tar infront of line
		current_line = if_space(current_line);
		*code_itr = current_line;

		for( string::iterator str_itr = current_line.begin();
		        str_itr != current_line.end(); str_itr++ ) {
			//delete current_line if empty, \t, \n or start with /
			if ( str_itr == current_line.begin() ) {
				if (*str_itr == '#' || *str_itr == '/' 
				        || *str_itr == '\n'
				        || *str_itr == '\r') {
//					cout << *code_itr << endl;
					code_itr = code.erase(code_itr);
					code_itr--;
					break;
				}
			}
			//check if { or }. if { or } -> continue
			if( (code_itr == code.begin() 
			    || code_itr == --code.end()) 
			    && str_itr == current_line.begin()){
				//if { or } break
				if( *str_itr == '{' || *str_itr == '}' ){
					break;
				}
			}

			//here start work detector,that works like state mashine
			//check size of list errors
			size_t before = errors.size();
			size_t tabl_bef  = table.size();

			detector(*str_itr, current_line);

			size_t after = errors.size();
			size_t tabl_aft  = table.size();
			if( before != after || tabl_aft != tabl_bef) {
				break;
			}

			//if size of list errors changed go to next line
		}//each symbol in line
	}
/*
	cout << "cout \n";

	cout << "start\n";
	for(list<string>::iterator all_code = code.begin();
	        all_code != code.end(); all_code++) {
		cout << *all_code << endl;
	}
	cout << "finish\n";
*/
	return 0;
}

void work_table()
{
//work with tables of lexems

	//need to change int num of line in hex.
	for(int i = 0; i < table.size(); i++) {

		bitset<4> x(table[i].address);
		table[i].adr_bin = x;
	}

	//work with labels
	for(int i = 0; i < table.size(); i++) {
		if( table[i].jmp_adr != "") {
			//find from start this label
			for(int j = 0; j < table.size(); j++) {
				//if not fins can be error
				if( table[i].jmp_adr == table[j].label ) {
					table[i].jmp_adr_bin = table[j].adr_bin;
				}
			}
		}
	}	
}


void syntaxis_analisis()
{
//input: list of lexems 
//output: tree with syntaxys struct
//syntaxis analisis need list of syntaxis rules

		//find lexems
		list<string> keywords;
		keywords.push_back("goto");
		keywords.push_back("ifflag");
		keywords.push_back("backward");
		keywords.push_back("forward");

}

void code_generator()
{
//input: tree that was build by syntaxis analyzator
//output: code like assembler
        if( errors.size() != 0 ){
                return;
        }

	ofstream offile; // iofsream for write file
	offile.open("code", ios::in);
	if(!offile.is_open()) { // if file is not open
		cout << "error name of write file!" << endl;
		return;
	}
	else {
		for(int i = 0; i < table.size(); i++) {
			size_t found = table[i].command.find("ifflag");
			if( found != string::npos ) {
				offile << "11000000\n";
//				cout << "11000000\n";				
				continue;
			}
			found = table[i].command.find("backward");
			if( found != string::npos ) {
				offile << "01000000\n";
//				cout << "01000000\n";
				continue;
			}
			found = table[i].command.find("forward");
			if( found != string::npos ) {
				offile << "00000000\n";
//				cout << "00000000\n";
				continue;
			}
			found = table[i].command.find("goto");
			if( found != string::npos ) {
				offile << "10" << "00" << table[i].jmp_adr_bin << "\n";
//				cout << "10" << "00" << table[i].jmp_adr_bin << "\n";
				continue;
			}
		}
	}
	offile.close();
}
/*
void code_optimizator()
{


}

void syntaxis_analisis(string& line)
{

}
*/

//into main comes source file with code
int main(int argc, char** argv)
{
	if(argc != 3) {
		cout << "Usage: " << argv[0] << " <file.train> " 
                         "<1 | 0 - to determine use hex or not>" << endl;
		return -1;
	}

	int rc = 0;
	rc = read_file(argv[1]);
	if(rc != 0) {
		cout << "error in lexical analisys" << endl;
		return -1;
	}

	rc = lexycal_analysis();

	cout << table.size() << endl;
	cout << "errors " << errors.size() << endl;

	for( list<string>::iterator itr_er = errors.begin(); itr_er != errors.end();
	     itr_er++) {
		cout << *itr_er << endl;
	}

	for(int i = 0; i < table.size(); i++) {
		cout << table[i].address << " " << table[i].command << " "
		     << table[i].label << " " << table[i].jmp_adr << endl;
	}

	work_table();

//	syntaxis_analisis();
//	semantic_analisis(); //lexer
//	parser();
	code_generator();

	return 0;
}
