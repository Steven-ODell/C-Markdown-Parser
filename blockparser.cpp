#include "blockparser.h"
#include "nodes.h"
#include "tokenizer.h"
#include <iostream>


blockParser::blockParser(std::vector<Token> *t) {
  tok = *t;
  cur = 0;
  pTree = &tree;
}

std::vector<blockNode> blockParser::createTree() { while (cur < tok.size()) {
    blockNode Node;
    if (tok[cur].index == 0 && tok[cur].type == h1 && tok[cur+1].type == space) {
      std::cout << "h1 found" << std::endl;
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
    else if (tok[cur].index == 0 && tok[cur].type == hr) {
      std::cout << "line found" << std::endl;
      while (tok[cur].type != newLine) {
        if (tok[cur+1].type != space && tok[cur+1].type != hr && tok[cur+1].type != newLine) {
          parseP();
          break; 
        }
        else {
          Node.type = hr;
          Node.value += tok[cur].value;
          Node.blockToks.push_back(tok[cur]);
          cur++;
        }
      }
      std::cout << Node.value << std::endl;
      tree.push_back(Node);
      cur++;
    }
    else if (tok[cur].index == 0 && (tok[cur].type == word || tok[cur].type == digit)) {
      parseP();
      cur++;
    }
    else if (tok[cur].index == 0 && (tok[cur].type == ul || tok[cur].type == space)) {
      parseUl();
    }
    else if (tok[cur].index == 0 && (tok[cur].type == ol || tok[cur].type == space)) {
      parseOl();
    }
    else cur++;
  }
  return tree;
}

void blockParser::parseP() {
  std::cout << "Paragraph found" << std::endl;
  blockNode Node;
  Node.type = p;
  while (cur + 1 < tok.size() && !(tok[cur].type == newLine && tok[cur+1].type == newLine)) {
    Node.value += tok[cur].value;
    Node.blockToks.push_back(tok[cur]);
    cur++;
  }
  std::cout << Node.value << std::endl;
  tree.push_back(Node);
  cur++;
}

void blockParser::parseOl() {
  std::cout << "Ordered list found" << std::endl;
  blockNode Node;
  Node.type = ol;
  while (cur + 1 < tok.size() && !(tok[cur].type == newLine && tok[cur+1].type == newLine)) {
    Node.value += tok[cur].value;
    Node.blockToks.push_back(tok[cur]);
    cur++;
  }
  std::cout << Node.value << std::endl;
  tree.push_back(Node);
  cur++;
}

void blockParser::parseUl() {
  std::cout << "Unordered list found" << std::endl;
  blockNode Node;
  Node.type = ul;
  while (cur + 1 < tok.size() && !(tok[cur].type == newLine && tok[cur+1].type == newLine)) {
    Node.value += tok[cur].value;
    Node.blockToks.push_back(tok[cur]);
    cur++;
  }
  std::cout << Node.value << std::endl;
  tree.push_back(Node);
  cur++;
}

