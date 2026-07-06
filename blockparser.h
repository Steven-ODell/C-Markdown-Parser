#pragma once 
#include <vector>
#include "tokenizer.h"
#include "nodes.h"


class blockParser {
private:
  int cur;
  std::vector<blockNode> tree;
  std::vector<blockNode>* pTree;
  std::vector<Token> tok;
public:
  blockParser(std::vector<Token> *t);
  std::vector<blockNode> createTree();
  void parseP();
  void parseCodeblock();
  void parseBlockquote();
  inlineNode parseOl(int);
  inlineNode parseUl(int);
};
