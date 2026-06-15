#pragma once
#include <vector>
#include <string>

enum TokenType {
  hr,
  h1,
  h2,
  h3,
  codeblock,
  blockquote,
  ul,
  ol,
  p,

  em,
  strong,
  strongEm,
  del,
  sub,
  sup,
  mark,
  code,

  newLine,
  space,
  digit,
  word,
  text,
  indent,
  link,

  lBracket,
  rBracket,
  lParen,
  rParen,
  period,
  comma,
  quote,
  dQuote,
  fSlash,
  bSlash,
  colon,
  semiColon,
};

struct Token {
  int type = TokenType();
  int index;
  std::string value;
};

class Lexer {
private:
  int c;
  int blockindex;
  std::string *source;
  std::vector<Token> tokens;
public:
  Lexer(std::string *incoming_string);
  std::vector<Token> tokenize();
};
