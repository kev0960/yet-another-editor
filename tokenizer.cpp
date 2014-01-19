#include "tokenizer.h"

Tokenizer::Tokenizer(string line) : line_(line)
{
    // We are going to tokenize the
    // terms of the source code into
    // an interpretable symbol

    // Tokenizer merely separates the
    // code into meaningful pieces. It
    // does not give any 'meaning' to
    // the pieces. 

    // There are four kinds of things to identify
    // as a token in this tokenizer
    //
    // 1. Identifiers
    // 2. Numbers
    // 3. Operators
    // 4. String literals

    int read_ptr = 0;
    bool current_reading = false;

    char begin_quote;

    int begin = 0, end = 0;
    TokenType type;

    cout << "PARSING :: " << line_ << endl;

    while (read_ptr != line_.size()) {
        // Basically remove all the whitespaces
        // Except when reading the string literals

        if(!current_reading) {
            if(is_digit(line_[read_ptr])) {
                type = TokenType::NUMBER;
            }
            else if(is_non_digit(line_[read_ptr])) {
                type = TokenType::IDENTIFIER;
            }
            else if(is_operator(line_[read_ptr])) {
                type = TokenType::OPERATOR;
            }
            else if(line_[read_ptr] == '\'' 
                    || line_[read_ptr] == '"') {
                type = TokenType::STRING_LITERAL;
                begin_quote = line_[read_ptr];
            }
            else if(is_whitespace(line_[read_ptr])) {
                read_ptr ++; continue;
            }

            current_reading = true;

            begin = read_ptr;
        }
        // Now deal with when current reading is enabled.
        // This means that we have to decide whether we 
        // accept next token as a part of a previous token
        // or the starting of a new token.
        else {
            if(type == TokenType::OPERATOR) {
                // ignores the whitespace between operators
                if(is_whitespace(line_[read_ptr]) || is_operator(line_[read_ptr])) {
                    read_ptr ++; continue;
                }
                else {
                    term_list_.emplace_back(Term(begin, read_ptr - 1, TokenType::OPERATOR));
                    current_reading = false;
                    continue; // Do not increase the read_ptr!
                }
            }
            else if(type == TokenType::IDENTIFIER || type == TokenType::NUMBER) {
                if(is_digit(line_[read_ptr]) || is_non_digit(line_[read_ptr])) {
                    read_ptr ++; continue;    
                }
                else if(type == TokenType::NUMBER && line_[read_ptr] == '.') {
                    read_ptr ++; continue;
                }
                else  {
                    term_list_.emplace_back(Term(begin, read_ptr - 1, type));
                    current_reading = false;
                    continue;
                }
            }
            else if(type == TokenType::STRING_LITERAL) {
                // We have to think about handling RAW string literals too. 
                // But here, lets only think the normal C++ string. 
                // (We have to update here later on)

                // Even if it meets quote or dquote, if that quote or dquote is 
                // followed by \, then it means that it is used to represent 
                // the character, not the end of the string. Hence we need to
                // check that part
                if(line_[read_ptr] == begin_quote && line_[read_ptr - 1] != '\\') {
                    // Point here is that unlike the previous ones, the 'end' of the term
                    // is same as the read_ptr. 
                    term_list_.emplace_back(Term(begin, read_ptr, TokenType::STRING_LITERAL));

                    current_reading = false;
                    read_ptr ++;
                    continue;
                }
                else {
                    read_ptr ++; continue;
                }
            }
        }
        read_ptr ++;
    }

    // Add the last term
    if(current_reading) {
        term_list_.emplace_back(Term(begin, read_ptr - 1, type));
    } 

    // Now parse the operators
    operator_separater();
}
// Separate the operators into a comprehensible form.
// For example, a+++b would be stored as a, +++, b. Hence
// we have to break up into meaningful operators, such as
// ++, +. This operation will make the parser's job much
// easier. 
void Tokenizer::operator_separater()
{
    vector<string> operator_list = {
        "{",
        "}",
        "[",
        "]",
        "#",
        "##",
        "(",
        ")",
        ";",
        ":",
        "...",
        "?",
        "::",
        ".",
        ".*",
        "+",
        "-",
        "*",
        "/",
        "%",
        "^",
        "&",
        "|",
        "~",
        "!",
        "=",
        "<",
        ">",
        "+=",
        "-=",
        "*=",
        "/=",
        "%=",
        "^=",
        "&=",
        "|=",
        "<<",
        ">>",
        ">>=",
        "<<=",
        "==",
        "!=",
        "<=",
        ">=",
        "&&",
        "||",
        "++",
        "--",
        ",",
        "->*",
        "->"
    };

    // The operator separating is greedy match. Find
    // the longest match operator each time to separate it. 
    for(int i = 0; i < term_list_.size(); i ++) {
        if(term_list_[i].type == TokenType::OPERATOR) {

            string operator_match;
            vector<Term> added_operator;

            int begin = term_list_[i].begin;
            for(int j = term_list_[i].begin; j <= term_list_[i].end; j ++) {
                if(!is_whitespace(line_[j])) {
                    operator_match.push_back(line_[j]);
                    // If the current operator looking for is in the list
                    // Then we continue on expanding that operator.
                    // When it isn't, then the previous operator_match
                    // will be our one operator
                    if(find(operator_list.begin(), operator_list.end(), operator_match) 
                            == operator_list.end()) {
                        added_operator.emplace_back(Term(begin, j - 1, TokenType::OPERATOR));
                        operator_match.clear();

                        begin = j;
                        j --; continue; 
                    }
                } 
            }
            // When it goes to the end while forming up the operator, then that
            // is it!
            if(!operator_match.empty()) {
                added_operator.emplace_back(Term(begin, term_list_[i].end, TokenType::OPERATOR));
            }
            // Remove the previously added operators
            term_list_.erase(term_list_.begin() + i, term_list_.begin() + i + 1);

            // Insert the newly separated operators.
            term_list_.insert(term_list_.begin() + i, added_operator.begin(), added_operator.end());

            i = i + added_operator.size() - 1;
        }
    }
}

