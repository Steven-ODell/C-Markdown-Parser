#include "tokenizer.h"
#include "loadfile.h"
#include "blockparser.h"
#include "inlineparser.h"
#include <iostream>



int main() {
  std::string inputfile = load_file();
  std::string* pInputfile = &inputfile;
  std::cout << "Parsing:\n" << inputfile << std::endl;

  Lexer lex(*pInputfile);
  std::vector<Token> allTokens = lex.tokenize();
  std::vector<Token> *pAllTokens = &allTokens;

  blockParser block(pAllTokens);
  std::vector<blockNode> blocks = block.createTree();

  std::vector<blockNode>* pBlocks = &blocks;

  inlineParser inlineparser(pBlocks);
  std::vector<blockNode> finalTree = inlineparser.parseInlines();
  
  for (size_t i = 0; i < finalTree.size(); i++) {
    blockNode block = finalTree[i];
    std::cout << "Block Type: " << block.type << " | " << block.value << std::endl;

    for (size_t k = 0; k < block.children.size(); k++) {
      inlineNode childNode = block.children[k];
      inlineparser.printTree(childNode, 1);
    }
  }
  
  return 0;
}
