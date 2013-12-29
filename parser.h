#include <algorithm>
#include <iostream>
#include <vector>
#include <exception>
#include <string>
#include <map>
#include <fstream>
#include <unordered_set>
#include <boost/filesystem.hpp>

using namespace std;

struct Symbol
{
    string symbol_name;
    vector<vector<string>> symbol_definition;
};

class Exception : exception
{
    string msg;

    public:

    Exception(string msg) : msg(msg) {}

    void print_msg() { cout << msg << endl;}

};

class Parser
{
    // The BNF Grammar rules
    vector<Symbol> bnf_grammar_;

    // tokenized line
    vector<string> token_;

    // The list of the literals and its usages
    map<string, unordered_set<string>> literal_;

    // Reversed version of BNF Grammar 
    // Ex.	 if nondigit is used in the definition of identifier / pp_number,
    // then identifier and pp_number contains non_digit

    map<string, unordered_set<string>> reverse_grammar_;

    public:

    void remove_whitespace(string& str);
    bool grammar_contains_term(string& s);
    bool contains(vector<string>& vec, string s);
    void print_symbol_table();
    void print_reverse_table();
    void print_literal_table();
    
    Parser(string directory_name);
    void parse_line(string line);

};

class CharacterInput
{

public:
       
    enum CHAR_TYPE {
        SOURCE_CODE = 0x01,
        ALPHA_CHAR = 0x02,
        Q_CHAR = 0x04,
        S_CHAR = 0x08,
        H_CHAR = 0x10,
        C_CHAR = 0x20,
        R_CHAR = 0x40,
        D_CHAR = 0x80,
        DIGIT = 0x100,
        NEWLINE = 0x200,
        LPAREN = 0x400 };

    CHAR_TYPE type_;
    char delimiter_;

    // Delimiter is for handling the r-char
    CharacterInput(CHAR_TYPE type, char delimiter = 0) : type_(type), delimiter_(delimiter) {}

    char operator>> (char c); 
};

