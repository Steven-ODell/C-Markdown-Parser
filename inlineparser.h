#pragma once
#include "nodes.h"

class inlineParser {
private:
  int curBlock;
  int curTok;
  std::vector<blockNode>& blocks;
  std::vector<Token>* activeToks;
public:
  inlineParser(std::vector<blockNode>&);
  std::vector<blockNode> parseInlines();
  std::vector<inlineNode> parseInlineChildren(int closer);
  void listCheck(std::vector<inlineNode>*);
  void printTree(inlineNode Node, int depth);
};


