#include "inlineparser.h"
#include "nodes.h"
#include "tokenizer.h"
#include <iostream>

inlineParser::inlineParser(std::vector<blockNode>* tree) {
  curBlock = 0;
  curTok = 0;
  pTree = tree;
}

std::vector<blockNode> inlineParser::parseInlines() {
  while (curBlock < pTree->size()) {
    curTok = 0;
    if ((*pTree)[curBlock].blockToks.size() > 0) {
      (*pTree)[curBlock].children = parseInlineChildren(-1);
    }
    curBlock++;
  } 
  return *pTree;
}

std::vector<inlineNode> inlineParser::parseInlineChildren(int closer) {
  std::vector<inlineNode> children;
  while (curTok < (*pTree)[curBlock].blockToks.size()) {
    int t = (*pTree)[curBlock].blockToks[curTok].type;
    if (t == closer) {
    curTok++;
    return children;
    }
    else if (t == strongEm) {
      inlineNode strongEmNode;
      strongEmNode.type = strongEm;
      curTok++;
      strongEmNode.children = parseInlineChildren(strongEm);
      children.push_back(strongEmNode);
    }
    else if (t == strong) {
      inlineNode strongNode;
      strongNode.type = strong;
      curTok++;
      strongNode.children = parseInlineChildren(strong);
      children.push_back(strongNode);
    }
    else if (t == em) {
      inlineNode emNode;
      emNode.type = em;
      curTok++;
      emNode.children = parseInlineChildren(em);
      children.push_back(emNode);
    }
    else if (t == hr) {
      curTok++;
    }
    else if (t == link) {
      inlineNode linkNode;
      linkNode.type = link;
      while (curTok < (*pTree)[curBlock].blockToks.size() && ((*pTree)[curBlock].blockToks[curTok].type == space || (*pTree)[curBlock].blockToks[curTok].type == newLine)) {
        linkNode.children = parseInlineChildren(rParen);
        curTok++;
      }
      children.push_back(linkNode);
      curTok++;
    }
    else if (t == indent) {
      inlineNode indentNode;
      indentNode.type = indent;
      indentNode.value = (*pTree)[curBlock].blockToks[curTok].value;
      children.push_back(indentNode);
      curTok++;
    }
    else if (t == word || t == space || t == digit || t == period || t == comma) {
      inlineNode textNode;
      textNode.type = text;
      while (curTok < (*pTree)[curBlock].blockToks.size() && ((*pTree)[curBlock].blockToks[curTok].type == word || (*pTree)[curBlock].blockToks[curTok].type == space || (*pTree)[curBlock].blockToks[curTok].type == period || (*pTree)[curBlock].blockToks[curTok].type == comma || (*pTree)[curBlock].blockToks[curTok].type == digit)) {
        textNode.value += (*pTree)[curBlock].blockToks[curTok].value;
        curTok++;
      }
      children.push_back(textNode);
    }
    else if (t == newLine) {
      curTok++;
    }
    else if (t == ul || t == ol) {
      inlineNode markerNode;
      markerNode.type = t;
      markerNode.value = (*pTree)[curBlock].blockToks[curTok].value;
      children.push_back(markerNode);
      curTok++;
    }
    else {
      inlineNode miscNode;
      miscNode.type = text;
      miscNode.value = (*pTree)[curBlock].blockToks[curTok].value;
      children.push_back(miscNode);
      curTok++;
    }
  }
  return children;
}

void inlineParser::printTree(inlineNode Node, int depth) {
  for (int d = depth; d > 0; d--) {
    std::cout << "  ";
  }
  std::cout << "Child type: " << Node.type << " | " << "Child value: " << Node.value << std::endl;
  
  for (int i = 0; i < Node.children.size(); i++) {
    printTree(Node.children[i], depth+1);
  }
}
