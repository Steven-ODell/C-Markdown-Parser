#pragma once
#include "nodes.h"
#include <string>
#include <vector>

class Stringifier {
private:
  int curBlock;
  int curChild;
  int curInline;
  std::string outputString;
  std::vector<blockNode>& tree;
public:
  Stringifier(std::vector<blockNode>&);
  std::string createHTML();
  void ulLoop(inlineNode&);
  void olLoop(inlineNode&);
  std::string createInlineHTML(inlineNode&);
};
