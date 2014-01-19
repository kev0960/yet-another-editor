// COMMAND TO COMPILE
// g++ -std=c++11 parser.cpp -o parser -lboost_filesystem -lboost_system

#include "parser.h"
#include "tokenizer.h"

Parser::Parser(string directory_name)
{
    // Read the Grammar from the file
    boost::filesystem::path directory_path(directory_name);

    if(!boost::filesystem::is_directory(directory_path)) 
        throw Exception("Illegal Directory Name" + directory_name);

    vector<string> file_list; 

    boost::filesystem::directory_iterator end_itr; 
    for(boost::filesystem::directory_iterator itr (directory_path); itr != end_itr; itr ++) {
        if(boost::filesystem::is_regular_file(itr->status())) file_list.push_back(itr->path().string());
        else throw Exception("Cannot found the file : " + itr->path().string());
    }

    // Read the Lex files. 
    for(auto i = 0; i < file_list.size(); i ++) {
        ifstream read_file(file_list[i].c_str());

        cout << file_list[i] << endl;
        if(read_file.bad()) continue;

        string line;
        while (read_file.good()) {
            getline(read_file, line);

            // Remove the preceding whitespaces
            for(auto i = 0; i < line.size(); i ++) {
                if(line[i] == ' ' || line[i] == '\t' || line[i] == '\n') {
                    line.erase(i, 1);
                    i --;
                } else break;
            }

            // Then it is defines a symbol
            if(line.find(":=") != string::npos) {
                string symbol_name = line.substr(0, line.find(":="));
                remove_whitespace(symbol_name);

                bnf_grammar_.emplace_back(Symbol());
                bnf_grammar_.back().symbol_name = symbol_name;
            }
            else {
                int prev = 0;

                // For the convenient processing, add the ' ' to the back
                // of the string
                line.push_back(' ');
                int check = line.find(" ");

                vector<string> temp;
                bnf_grammar_.back().symbol_definition.push_back(temp);

                while (check != string::npos) {
                    if(check - prev == 0) break;

                    string term = line.substr(prev, check - prev);
                    remove_whitespace(term);

                    // The opt-able terms are expressed as (term)@
                    // For the convenient processing, we moved the location of @
                    // to the front, changing it into @(term)
                    if(term.back() == '@') {
                        term.pop_back();
                        term.insert(term.begin(), '@');
                    }

                    bnf_grammar_.back().symbol_definition.back().push_back(term);

                    prev = check;
                    check = line.find(" ", prev + 1);

                    prev ++;
                }
            }
        }
    }

    // Create a reverse version of BNF Grammar
    for(auto i = 0; i < bnf_grammar_.size(); i ++) {
        Symbol sym = bnf_grammar_[i];
        if(sym.symbol_name[0] == '@')
            sym.symbol_name.erase(0, 1);

        for(auto j = 0; j < sym.symbol_definition.size(); j ++) {
            for(auto k = 0; k < sym.symbol_definition[j].size(); k ++) {
                string term = sym.symbol_definition[j][k];

                // Remove the 'option' notation symbol
                if(term[0] == '@') term.erase(0, 1);

                // If that symbol had not previously added
                if(reverse_grammar_.find(term) 	== reverse_grammar_.end()) {
                    unordered_set<string> temp;
                    reverse_grammar_[term] = temp;
                    reverse_grammar_[term].insert(sym.symbol_name);
                } else reverse_grammar_[term].insert(sym.symbol_name);
            }
        }
    }

    // Create the list of the literals
    // From the list of the symbols that are used, find the 
    // ones that does not have a definition (== literal)
    for(auto itr = reverse_grammar_.begin(); itr != reverse_grammar_.end(); itr ++) {
        string term = itr->first;

        // If there is term has no definition
        // Add it to the literal
        if(!grammar_contains_term(term)) {
            literal_[term] = itr->second;
        }
    }

    print_reverse_table();
    print_literal_table();
}
void Parser::parse_line(string line)
{
    Tokenizer token(line);
    for(auto i = 0; i < token.term_list_.size(); i ++) {
        string s (line.begin() + token.term_list_[i].begin, 
                  line.begin() + token.term_list_[i].end + 1);
        cout << token.token_type(token.term_list_[i].type) << " ( " <<
            token.term_list_[i].begin << " , " << 
            token.term_list_[i].end   << " ) : " << s << endl;
    }

    // After the files being parsed, now we have to 
    // Analyze the semantic meaning of the each terms
    // Once the tokenizer identifies each terms, 
    // we have to make the parser to comprehend what 
    // the each terms are. 


}

int main(int argc, char* argv[])
{
    Parser p("./Parser");   
    ifstream in("./Source/source");

    while (in.good()) {
        string s; getline(in, s);
        if(s.substr(0, 2) != "//")
            p.parse_line(s);
    }
}   
