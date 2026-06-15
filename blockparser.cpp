#include "blockparser.h"
#include "nodes.h"
#include "tokenizer.h"
#include <iostream>


blockParser::blockParser(std::vector<Token> *t) {
  tok = *t;
  cur = 0;
  pTree = &tree;
}

std::vector<blockNode> blockParser::createTree() { 
  while (cur < tok.size()) {
    blockNode Node;

    int lookAhead = cur;
    while (lookAhead < tok.size() && tok[lookAhead].type == indent) {
      lookAhead++;
    }
    if (tok[cur].index == 0 && tok[cur].type == h1 && tok[cur+1].type == space) {
      std::cout << "h1 found" << std::endl;
      Node.value += "# ";
      cur+=2;
      while (tok[cur].type != newLine) {
        Node.value += tok[cur].value;
        Node.type = h1;
        Node.blockToks.push_back(tok[cur]);
        cur++;
      }
      std::cout << Node.value << std::endl;
      tree.push_back(Node);
      cur++;
    }
    else if (tok[cur].index == 0 && tok[cur].type == h2 && tok[cur+1].type == space) {
      std::cout << "h2 found" << std::endl;
      Node.value += "## ";
      cur+=2;
      while (tok[cur].type != newLine) {
        Node.value += tok[cur].value;
        Node.type = h2;
        Node.blockToks.push_back(tok[cur]);
        cur++;
      }
      std::cout << Node.value << std::endl;
      tree.push_back(Node);
      cur++;
    }
    else if (tok[cur].index == 0 && tok[cur].type == h3 && tok[cur+1].type == space) {
      std::cout << "h3 found" << std::endl;
      Node.value += "### ";
      cur+=2;
      while (tok[cur].type != newLine) {
        Node.value += tok[cur].value;
        Node.type = h3;
        Node.blockToks.push_back(tok[cur]);
        cur++;
      }
      std::cout << Node.value << std::endl;
      tree.push_back(Node);
      cur++;
    }
    else if (tok[cur].index == 0 && tok[cur].type == blockquote) {
      std::cout << "Blockquote found" << std::endl;
      parseBlockquote();
      continue;
    }
    else if (tok[cur].index == 0 && tok[cur].type == codeblock) {
      std::cout << "Codeblock found" << std::endl;
      parseCodeblock();
      continue;
    }
    else if (tok[cur].index == 0 && tok[cur].type == hr) {
      std::cout << "line found" << std::endl;
      if (cur + 1 < tok.size() && tok[cur+1].type != space && tok[cur+1].type != hr && tok[cur+1].type != newLine) {
        parseP();
        continue;
      }
      while (cur < tok.size() && tok[cur].type != newLine) {
        Node.type = hr;
        Node.value += tok[cur].value;
        Node.blockToks.push_back(tok[cur]);
        cur++;
      }
      std::cout << Node.value << std::endl;
      tree.push_back(Node);
      if (cur < tok.size()) cur++;
    }
    else if (tok[cur].index == 0 && (tok[cur].type == word || tok[cur].type == digit)) {
      std::cout << "Paragraph found" << std::endl;
      parseP();
      continue;
    }
    else if (lookAhead < tok.size() && tok[cur].index == 0 && tok[lookAhead].type == ul) {
      std::cout << "Unordered list found" << std::endl;
      parseUl();
      continue;
    }
    else if (lookAhead < tok.size() && tok[cur].index == 0 && tok[lookAhead].type == ol) {
      std::cout << "Ordered list found" << std::endl;
      parseOl();
      continue;
    }
    else cur++;
  }
  return tree;
}

void blockParser::parseP() {
  blockNode Node;
  Node.type = p;
  while (cur + 1 < tok.size() && !(tok[cur].type == newLine && tok[cur+1].type == newLine)) {
    Node.value += tok[cur].value;
    Node.blockToks.push_back(tok[cur]);
    cur++;
  }
  std::cout << Node.value << std::endl;
  tree.push_back(Node);
  if (cur < tok.size()) cur++;
}

void blockParser::parseBlockquote() {
  blockNode Node;
  Node.type = blockquote;
  Node.value += tok[cur].value;
  Node.blockToks.push_back(tok[cur]);
  cur++;
  if (cur < tok.size() && tok[cur].type == space) {
    Node.value += tok[cur].value;
    cur++;
  }
  while (cur + 1 < tok.size() && !(tok[cur].type == newLine && tok[cur+1].type == newLine)) {
    Node.value += tok[cur].value;
    Node.blockToks.push_back(tok[cur]);
    cur++;
  }
  Node.blockToks.push_back(tok[cur]);
  Node.value += tok[cur].value;
  if (cur < tok.size()) cur++;
  std::cout << Node.value << std::endl;
  tree.push_back(Node);
}

void blockParser::parseCodeblock() {
  blockNode Node;
  Node.type = codeblock;
  Node.value += tok[cur].value;
  Node.blockToks.push_back(tok[cur]);
  cur++;
  while (cur + 1 < tok.size() && tok[cur].type != codeblock) {
    Node.value += tok[cur].value;
    Node.blockToks.push_back(tok[cur]);
    cur++;
  }
  Node.blockToks.push_back(tok[cur]);
  Node.value += tok[cur].value;
  if (cur < tok.size()) cur++;
  std::cout << Node.value << std::endl;
  tree.push_back(Node);
}

void blockParser::parseOl() {
  blockNode Node;
  Node.type = ol;
  while (cur + 1 < tok.size() && tok[cur].type != newLine) {
    Node.value += tok[cur].value;
    Node.blockToks.push_back(tok[cur]);
    cur++;
  }
  std::cout << Node.value << std::endl;
  tree.push_back(Node);
  if (cur < tok.size()) cur++;
}

void blockParser::parseUl() {
  blockNode Node;
  Node.type = ul;
  while (cur + 1 < tok.size() && tok[cur].type != newLine) {
    Node.value += tok[cur].value;
    Node.blockToks.push_back(tok[cur]);
    cur++;
  }
  std::cout << Node.value << std::endl;
  tree.push_back(Node);
  if (cur < tok.size()) cur++;
}

