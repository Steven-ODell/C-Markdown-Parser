#pragma once 
#include "tokenizer.h"
#include <iostream>


Lexer::Lexer(std::string &incoming_string) {
  source = incoming_string;
  c = 0;
  blockindex = 0;
}

std::vector<Token> Lexer::tokenize() {
  Token t;
  while (c < source.length()) {
    if (source[c] == '#' && source[c+1] == '#' && source[c+2] == '#' && source[c+3] == '#' && source[c+4] == '#' && source[c+5] == '#') {
      t.type = h6;
      t.value = "######";
      t.index = blockindex;
      tokens.push_back(t);
      c += 6;
      blockindex++;
    }
    else if (source[c] == '#' && source[c+1] == '#' && source[c+2] == '#' && source[c+3] == '#' && source[c+4] == '#') {
      t.type = h5;
      t.value = "#####";
      t.index = blockindex;
      tokens.push_back(t);
      c += 5;
      blockindex++;
    }
    else if (source[c] == '#' && source[c+1] == '#' && source[c+2] == '#' && source[c+3] == '#') {
      t.type = h4;
      t.value = "####";
      t.index = blockindex;
      tokens.push_back(t);
      c += 4;
      blockindex++;
    }
    else if (source[c] == '#' && source[c+1] == '#' && source[c+2] == '#') {
      t.type = h3;
      t.value = "###";
      t.index = blockindex;
      tokens.push_back(t);
      c += 3;
      blockindex++;
    }
    else if (source[c] == '#' && source[c+1] == '#') {
      t.type = h2;
      t.value = "##";
      t.index = blockindex;
      tokens.push_back(t);
      c += 2;
      blockindex++;
    }
    else if (source[c] == '#') {
      t.type = h1;
      t.value = "#";
      t.index = blockindex;
      tokens.push_back(t);
      c++;
      blockindex++;
    }
    else if (source[c] == '`' && source[c+1] == '`' && source[c+2] == '`') {
      t.type = codeblock;
      t.value = "```";
      t.index = blockindex;
      tokens.push_back(t);
      c += 3;
      blockindex++;
    }
    else if (source[c] == '`') {
      t.type = code;
      t.value = "`";
      t.index = blockindex;
      tokens.push_back(t);
      c++;
      blockindex++;
    }
    else if (source[c] == '>') {
      t.type = blockquote;
      t.value = ">";
      t.index = blockindex;
      tokens.push_back(t);
      c++;
      blockindex++;
    }
    else if (source[c] == '-' && source[c+1] == '-' && source[c+2] == '-') {
      t.type = hr;
      t.value = "---";
      t.index = blockindex;
      tokens.push_back(t);
      c += 3;
      blockindex++;
    }
    else if (source[c] == '-') {
      t.type = ul;
      t.value = "-";
      t.index = blockindex;
      tokens.push_back(t);
      c++;
      blockindex++;
    }
    else if (isalpha(source[c]) || source[c] == '\'') {
      t.type = word;
      std::string new_word;
      while (isalpha(source[c]) || source[c] == '\'') {
        new_word += source[c];
        c++;
      }
      t.value = new_word;
      t.index = blockindex;
      tokens.push_back(t);
      blockindex++;
    }
    else if (isdigit(source[c]) && source[c+1] == '.') {
      t.type = ol;
      t.value = source[c];
      t.value += '.';
      t.index = blockindex;
      tokens.push_back(t);
      c += 2;
      blockindex++;
    }
    else if (isdigit(source[c])) {
      t.type = digit;
      std::string new_digit;
      while (isdigit(source[c])) {
        new_digit += source[c];
        c++;
      }
      t.value = new_digit;
      t.index = blockindex;
      tokens.push_back(t);
      blockindex++;
    }
    else if (isspace(source[c])) {
      if (source[c] == '\n') {
        t.type = newLine;
        t.value = "\n";
        t.index = blockindex;
        blockindex = 0;
        tokens.push_back(t);
        c++;
      }
      else if (isspace(source[c]) && isspace(source[c+1]) && isspace(source[c+2]) && isspace(source[c+3])) {
        t.type = indent;
        t.value = "'    '";
        t.index = blockindex;
        tokens.push_back(t);
        c+=4;
      }
      else if (isspace(source[c]) && isspace(source[c+1])) {
        t.type = indent;
        t.value = "'  '";
        t.index = blockindex;
        tokens.push_back(t);
        c+=2;
      }
      else {
        t.type = space;
        t.value = " ";
        t.index = blockindex;
        tokens.push_back(t);
        c++;
        blockindex++;
      }
    }
    else if (source[c] == '*' && source[c+1] == '*' && source[c+2] == '*') {
      t.type = strongEm;
      t.value = "***";
      t.index = blockindex;
      tokens.push_back(t);
      c += 3;
      blockindex++;
    }
    else if (source[c] == '*' && source[c+1] == '*') {
      t.type = strong;
      t.value = "**";
      t.index = blockindex;
      tokens.push_back(t);
      c += 2;
      blockindex++;
    }
    else if (source[c] == '*') {
      t.type = em;
      t.value = "*";
      t.index = blockindex;
      tokens.push_back(t);
      c++;
      blockindex++;
    }
    else if (source[c] == '=' && source[c+1] == '=') {
      t.type = mark;
      t.value = "==";
      t.index = blockindex;
      tokens.push_back(t);
      c += 2;
      blockindex++;
    }
    else if (source[c] == '~' && source[c+1] == '~') {
      t.type = del;
      t.value = "~~";
      t.index = blockindex;
      tokens.push_back(t);
      c += 2;
      blockindex++;
    }
    else if (source[c] == '~') {
      t.type = sub;
      t.value = "~";
      t.index = blockindex;
      tokens.push_back(t);
      c++;
      blockindex++;
    }
    else if (source[c] == '^') {
      t.type = sup;
      t.value = "^";
      t.index = blockindex;
      tokens.push_back(t);
      c++;
      blockindex++;
    }
    else if (source[c] == '`') {
      t.type = code;
      t.value = "`";
      t.index = blockindex;
      tokens.push_back(t);
      c++;
      blockindex++;
    }
    else if (source[c] == '[') {
      t.type = lBracket;
      t.value = "[";
      t.index = blockindex;
      tokens.push_back(t);
      c++;
      blockindex++;
    }
    else if (source[c] == ']') {
      t.type = rBracket;
      t.value = "]";
      t.index = blockindex;
      tokens.push_back(t);
      c++;
      blockindex++;
    }
    else if (source[c] == '(') {
      t.type = lParen;
      t.value = "(";
      t.index = blockindex;
      tokens.push_back(t);
      c++;
      blockindex++;
    }
    else if (source[c] == ')') {
      t.type = rParen;
      t.value = ")";
      t.index = blockindex;
      tokens.push_back(t);
      c++;
      blockindex++;
    }
    else if (source[c] == '.') {
      t.type = period;
      t.value = ".";
      t.index = blockindex;
      tokens.push_back(t);
      c++;
      blockindex++;
    }
    else if (source[c] == ',') {
      t.type = comma;
      t.value = ",";
      t.index = blockindex;
      tokens.push_back(t);
      c++;
      blockindex++;
    }
    else if (source[c] == '\'') {
      t.type = quote;
      t.value = "'";
      t.index = blockindex;
      tokens.push_back(t);
      c++;
      blockindex++;
    }
    else if (source[c] == '"') {
      t.type = dQuote;
      t.value = "\"";
      t.index = blockindex;
      tokens.push_back(t);
      c++;
      blockindex++;
    }
    else if (source[c] == '/') {
      t.type = fSlash;
      t.value = "/";
      t.index = blockindex;
      tokens.push_back(t);
      c++;
      blockindex++;
    }
    else if (source[c] == '\\') {
      t.type = bSlash;
      t.value = "\\";
      t.index = blockindex;
      tokens.push_back(t);
      c++;
      blockindex++;
    }
    else if (source[c] == ':') {
      t.type = colon;
      t.index = blockindex;
      t.value = ":";
      tokens.push_back(t);
      c++;
      blockindex++;
    }
    else if (source[c] == ';') {
      t.type = semiColon;
      t.value = ";";
      tokens.push_back(t);
      c++;
      blockindex++;
    }
    else {
      c++;
      blockindex++;
    }
  }
  for (int i = 0; i < tokens.size(); i++) {
    std::cout << "Type| '" << tokens[i].type << "' Value| '" << tokens[i].value << "' Index| '" << tokens[i].index << std::endl;
  }
  return tokens;
}
