#pragma once
#include "tokenizer.h"

struct inlineNode {
  int type = TokenType();
  std::string value;
  std::vector<inlineNode> children;
  std::vector<Token> inlineToks;
};

struct blockNode {
  int type = TokenType();
  std::string value;
  std::vector<inlineNode> children;
  std::vector<Token> blockToks;
};
