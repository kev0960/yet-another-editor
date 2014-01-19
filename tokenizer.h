#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Without hardly parse everything from
// the lexicon files, it will try to separate
// the terms into symbols from terminals
class Tokenizer
{
    enum TokenType {IDENTIFIER, NUMBER, OPERATOR, STRING_LITERAL};
    struct Term
    {
        int begin, end;
        TokenType type;

        Term(int begin, int end, TokenType type) : begin(begin), end(end), type(type) {}
    };

    string line_;

    bool is_digit(char c) {
        if('0' <= c && c <= '9') return true;
        return false;
    }
    bool is_operator(char c) {
        // The list of the operators that are used
        // in the C/C++
        switch(c) {
            case '+':
            case '-':
            case '*':
            case '/':
            case '=':
            case '&':
            case '|':
            case '<':
            case '>':
            case '(':
            case ')':
            case '{':
            case '}':
            case '[':
            case ']':
            case '~':
            case '!':
            case ',':
            case '.':
            case '%':
            case '#':
            case ':':
            case '?':
            case '^':
            case ';':
                return true;
            default:
                return false;
        }
    }
    // The characters that can be used as an identifier
    bool is_non_digit(char c)
    {
        if('a' <= c && c <= 'z') return true;
        else if('A' <= c && c <= 'Z') return true;
        else if(c == '_') return true;
        return false;
    }

    bool is_whitespace(char c)
    {
        if(c == ' ' || c == '\n'
                || c == '\t' || c == '\r') 
            return true;
        return false; 
    }

    public:

    vector<Term> term_list_;
    Tokenizer(string line);

    // Separate the operators into meaningful forms. 
    void operator_separater();
    string token_type(TokenType t) {
        switch(t) {
            case TokenType::OPERATOR:
                return "id:OPERATOR";
            case TokenType::NUMBER:
                return "id:NUMBER";
            case TokenType::IDENTIFIER:
                return "id:IDENTIFIER";
            case Tokenizer::STRING_LITERAL:
                return "id:STRING_LITERAL";
        }
        return "";
    }
};
