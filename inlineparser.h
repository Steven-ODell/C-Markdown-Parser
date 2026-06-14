#pragma once
#include "nodes.h"

class inlineParser {
private:
  int curBlock;
  int curTok;
  std::vector<blockNode>* pTree;
public:
  inlineParser(std::vector<blockNode>*);
  std::vector<blockNode> parseInlines();
  std::vector<inlineNode> parseInlineChildren(int closer);
  void printTree(inlineNode Node, int depth);
};


