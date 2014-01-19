#include "parser.h"
void Parser::remove_whitespace(string& str)
{
    for(int i = 0; i < str.size(); i ++) {
        if(str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || str[i] == '\r') {
            str.erase(i, 1); i --;
        }   
    }
}
bool Parser::grammar_contains_term(string& s)
{
    for(auto itr = bnf_grammar_.begin(); itr != bnf_grammar_.end(); itr ++) {
        if(itr->symbol_name == s) return true;
    }
    return false; 
}
bool Parser::contains(vector<string>& vec, string s)
{
    vector<string>::iterator itr = vec.begin();
    while (itr != vec.end()) {
        if(*itr == s) return true;
        itr ++;
    }
    return false; 
}
void Parser::print_symbol_table()
{
    // Print the Symbol Table
    for(auto i = 0; i < bnf_grammar_.size(); i ++) {
        cout << bnf_grammar_[i].symbol_name << endl;
        for(auto j = 0; j < bnf_grammar_[i].symbol_definition.size(); j ++) {
            cout << "\t::== ";
            for(auto k = 0; k < bnf_grammar_[i].symbol_definition[j].size(); k ++) {
                cout << bnf_grammar_[i].symbol_definition[j][k] << " ";
            }
            cout << endl;
        }
    }
}
void Parser::print_reverse_table()
{
    ofstream out("log.txt");
    out << "---------- REVERSE TABLE BEGINS ------------" << endl;
    for(auto itr = reverse_grammar_.begin(); itr != reverse_grammar_.end(); itr ++) {
        out << itr->first << endl;
        for(auto itr2 = itr->second.begin(); itr2 != itr->second.end(); itr2 ++) {
            out << "\t" << *itr2 << endl;
        }
        out << endl;
    }
    out << "---------- REVERSE TABLE ENDS  -------------" << endl;
    out.close();
}

void Parser::print_literal_table()
{
    ofstream out("log.txt");
    
    out << "---------- LITERAL TABLE BEGINS ------------" << endl;
    for(auto itr = literal_.begin(); itr != literal_.end(); itr ++) {
        out << itr->first << endl;
        for(auto itr2 = itr->second.begin(); itr2 != itr->second.end(); itr2 ++) {
            out << "\t" << *itr2 << endl;
        }
        out << endl;
    }
    out << "---------- LITERAL TABLE ENDS -------------" << endl;
}
