#include "parser.h"

// if that character fits the expected input 
// Then save the character. If the character is not
// expected one, it throws the exception

// CharacterInput input;
// input >> 'a';
char CharacterInput::operator>> (char c)
{
    // SOURCE_CODE character type is to simply retrieve
    // the non-identifier components of the code. For
    // example, if we parse ++a; , then '++' part of the code
    // is retrieved as 'SOURCE_CODE'. The important thing is
    // there must be a keyword that has SOURCE_CODE as a part
    // of a keyword
    if(type_ & CHAR_TYPE::SOURCE_CODE) {

    }
    else if(type_ & CHAR_TYPE::ALPHA_CHAR) {
        if(isalpha(c)) return c;
    }
    else if(type_ & CHAR_TYPE::DIGIT) {
        if(isdigit(c)) return c;
    }
}
