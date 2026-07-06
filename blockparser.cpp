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
    else if (tok[cur].index == 0 && tok[cur].type == h4 && tok[cur+1].type == space) {
      std::cout << "h4 found" << std::endl;
      Node.value += "#### ";
      cur+=2;
      while (tok[cur].type != newLine) {
        Node.value += tok[cur].value;
        Node.type = h4;
        Node.blockToks.push_back(tok[cur]);
        cur++;
      }
      std::cout << Node.value << std::endl;
      tree.push_back(Node);
      cur++;
    }
    else if (tok[cur].index == 0 && tok[cur].type == h5 && tok[cur+1].type == space) {
      std::cout << "h5 found" << std::endl;
      Node.value += "##### ";
      cur+=2;
      while (tok[cur].type != newLine) {
        Node.value += tok[cur].value;
        Node.type = h5;
        Node.blockToks.push_back(tok[cur]);
        cur++;
      }
      std::cout << Node.value << std::endl;
      tree.push_back(Node);
      cur++;
    }
    else if (tok[cur].index == 0 && tok[cur].type == h6 && tok[cur+1].type == space) {
      std::cout << "h6 found" << std::endl;
      Node.value += "###### ";
      cur+=2;
      while (tok[cur].type != newLine) {
        Node.value += tok[cur].value;
        Node.type = h6;
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
      if (cur + 3 < tok.size() && tok[cur+1].type != space && tok[cur+1].type != hr && tok[cur+1].type != newLine) {
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
    else if (tok[cur].index == 0 && (tok[cur].type == word || tok[cur].type == digit || tok[cur].type == lBracket)) {
      std::cout << "Paragraph found" << std::endl;
      parseP();
      continue;
    }
    else if (lookAhead < tok.size() && tok[cur].index == 0 && tok[lookAhead].type == dash) {
      std::cout << "Unordered list found" << std::endl;
      blockNode ulNode;
      ulNode.type = ul;
      inlineNode listContent = parseUl(0);
      ulNode.children = listContent.children;
      tree.push_back(ulNode);
      cur++;
      continue;
    }
    else if (lookAhead < tok.size() && tok[cur].index == 0 && tok[lookAhead].type == ol) {
      std::cout << "Ordered list found" << std::endl;
      blockNode olNode;
      olNode.type = ol;
      inlineNode listContent = parseOl(0);
      olNode.children = listContent.children;
      tree.push_back(olNode);
      cur++;
      continue;
    }
    else if (tok[cur].type == newLine) {
      blockNode newlineNode;
      newlineNode.type = newLine;
      newlineNode.value = "\\n";
      tree.push_back(newlineNode);
      cur++;
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
  if (cur < tok.size()) cur++;
  std::cout << Node.value << std::endl;
  Node.value += tok[cur].value;
  tree.push_back(Node);
}

void blockParser::parseCodeblock() {
  blockNode Node;
  Node.type = codeblock;
  cur++;
  while (cur + 1 < tok.size() && tok[cur].type != codeblock) {
    Node.value += tok[cur].value;
    Node.blockToks.push_back(tok[cur]);
    cur++;
  }
  if (cur < tok.size()) cur++;
  std::cout << Node.value << std::endl;
  tree.push_back(Node);
}

inlineNode blockParser::parseOl(int myDepth) {
  inlineNode Node;
  Node.type = ol;

  while (cur < tok.size()) {
    std::cout << "cur=" << cur << " tok type=" << tok[cur].type << std::endl;
    int depth = 0;
    int peek = cur;
    while (peek < tok.size() && tok[peek].type == indent) {
      depth++;
      peek++;
    }
    if (peek >= tok.size() || tok[peek].type != ol) break;
    if (depth < myDepth) break;

    if (depth > myDepth) {
      if (Node.children.empty()) break;
      Node.children.back().children.push_back(parseOl(depth));
      if (Node.children.empty()) break;
      continue;
    }
    cur = peek + 1;

    inlineNode item;
    item.type = text;
    if (cur < tok.size() && tok[cur].type == space) cur++;
    while (cur < tok.size() && tok[cur].type != newLine) {
      item.value += tok[cur].value;
      item.inlineToks.push_back(tok[cur]);
      cur++;
    }
    if (cur < tok.size() && tok[cur].type == newLine) cur++;
    Node.children.push_back(item);
  }
  return Node;
}

inlineNode blockParser::parseUl(int myDepth) {
  inlineNode Node;
  Node.type = ul;

  while (cur < tok.size()) {
    std::cout << "cur=" << cur << " tok type=" << tok[cur].type << std::endl;
    int depth = 0;
    int peek = cur;
    while (peek < tok.size() && tok[peek].type == indent) {
      depth++;
      peek++;
    }
    if (peek >= tok.size() || tok[peek].type != dash) break;
    if (depth < myDepth) break;

    if (depth > myDepth) {
      if (Node.children.empty()) break;
      Node.children.back().children.push_back(parseUl(depth));
      if (Node.children.empty()) break;
      continue;
    }
    cur = peek + 1;

    inlineNode item;
    item.type = text;
    if (cur < tok.size() && tok[cur].type == space) cur++;
    while (cur < tok.size() && tok[cur].type != newLine) {
      item.value += tok[cur].value;
      item.inlineToks.push_back(tok[cur]);
      cur++;
    }
    if (cur < tok.size() && tok[cur].type == newLine) cur++;
    Node.children.push_back(item);
  }
  return Node;
}

