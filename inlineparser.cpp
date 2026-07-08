#include "inlineparser.h"
#include "nodes.h"
#include "tokenizer.h"
#include <iostream>

inlineParser::inlineParser(std::vector<blockNode> &tree) : blocks(tree) {
  curBlock = 0;
  curTok = 0;
}

std::vector<blockNode> inlineParser::parseInlines() {
  while (curBlock < blocks.size()) {
    curTok = 0;
    if (blocks[curBlock].type == ul || blocks[curBlock].type == ol) {
      listCheck(&blocks[curBlock].children);
    }
    else if (blocks[curBlock].blockToks.size() > 0) {
      activeToks = &blocks[curBlock].blockToks;
      blocks[curBlock].children = parseInlineChildren(-1);
    }
    curBlock++;
  } 
  return blocks;
}

void inlineParser::listCheck(std::vector<inlineNode>* inlineList) {
  for (int i = 0; i < inlineList->size(); i++) {
    inlineNode& item = (*inlineList)[i];
    if (item.inlineToks.size() > 0) {
      curTok = 0;
      activeToks = &(*inlineList)[i].inlineToks;
      std::vector<inlineNode> parsed = parseInlineChildren(-1);
      if (parsed.size() == 1 && parsed[0].children.size() == 0) {
        (*inlineList)[i].value = parsed[0].value;
      }
      else {
        (*inlineList)[i].children = parsed;
        (*inlineList)[i].value = "";
      }
    }
    for (int k = 0; k < (*inlineList)[i].children.size(); k++) {
      if ((*inlineList)[i].children[k].type == ul || (*inlineList)[i].children[k].type == ol) {
        listCheck(&(*inlineList)[i].children[k].children);
      }
    }
  }
}

std::vector<inlineNode> inlineParser::parseInlineChildren(int closer) {
  std::vector<inlineNode> children;
  while (curTok < (*activeToks).size()) {
    int t = (*activeToks)[curTok].type;
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
    else if (t == code) {
      inlineNode codeNode;
      codeNode.type = code;
      curTok++;
      codeNode.children = parseInlineChildren(code);
      children.push_back(codeNode);
    }
    else if (t == sup) {
      inlineNode supNode;
      supNode.type = sup;
      supNode.value = "^";
      curTok++;
      
      inlineNode supContext;
      supContext.type = t;
      supContext.value = (*activeToks)[curTok].value;
      supNode.children.push_back(supContext);

      curTok++;
      children.push_back(supNode);
    }
    else if (t == sub) {
      inlineNode subNode;
      subNode.type = sub;
      subNode.value = "~";
      curTok++;
      
      inlineNode subContext;
      subContext.type = t;
      subContext.value = (*activeToks)[curTok].value;
      subNode.children.push_back(subContext);

      curTok++;
      children.push_back(subNode);
    }
    else if (t == mark) {
      inlineNode markNode;
      markNode.type = mark;
      markNode.value = "==";
      curTok++;
      markNode.children = parseInlineChildren(mark);
      children.push_back(markNode);
    }
    else if (t == del) {
      inlineNode delNode;
      delNode.type = del;
      delNode.value = "~~";
      curTok++;
      delNode.children = parseInlineChildren(del);
      children.push_back(delNode);
    }
    else if (t == hr) {
      curTok++;
    }
    else if (t == lBracket) {
      int peek = curTok+1;
      bool foundClose = false;
      while (peek < (*activeToks).size()) {
        if ((*activeToks)[peek].type == rBracket) {
          foundClose = true;
          break;
        }
        peek++;
      }

      if (!foundClose) {
        inlineNode textNode;
        textNode.type = text;
        while (curTok < (*activeToks).size() && ((*activeToks)[curTok].type == word || (*activeToks)[curTok].type == space || (*activeToks)[curTok].type == period || (*activeToks)[curTok].type == comma || (*activeToks)[curTok].type == digit || (*activeToks)[curTok].type == colon || (*activeToks)[curTok].type == fSlash || (*activeToks)[curTok].type == bSlash || (*activeToks)[curTok].type == lBracket || (*activeToks)[curTok].type == dash)) {
          textNode.value += (*activeToks)[curTok].value;
          curTok++;
        }
        children.push_back(textNode);
      }
      else {
        inlineNode bracketNode;
        bracketNode.type = lBracket;
        if ((*activeToks)[curTok-1].type == bang) {
          curTok++;
          bracketNode.children = parseInlineChildren(rBracket);
          if ((*activeToks)[curTok].type == lParen) {
            curTok++;
            bracketNode.type = image;
            bracketNode.value = "";
            int nodesizeone = bracketNode.children.size();
            bracketNode.children[nodesizeone-1].children = parseInlineChildren(rParen);
          }
        }
        else {
          curTok++;
          bracketNode.children = parseInlineChildren(rBracket);
          if ((*activeToks)[curTok].type == lParen) {
            curTok++;
            bracketNode.type = link;
            bracketNode.value = "";
            int nodesize = bracketNode.children.size();
            bracketNode.children[nodesize-1].children = parseInlineChildren(rParen);
          }
        }
        children.push_back(bracketNode);
      }
    }
    else if (t == indent) {
      inlineNode indentNode;
      indentNode.type = indent;
      indentNode.value = (*activeToks)[curTok].value;
      children.push_back(indentNode);
      curTok++;
    }
    else if (t == newLine || t == word || t == space || t == digit || t == period || t == comma || t == colon || t == fSlash || t == bSlash) {
      inlineNode textNode;
      textNode.type = text;
      while (curTok < (*activeToks).size() && ((*activeToks)[curTok].type == newLine || (*activeToks)[curTok].type == word || (*activeToks)[curTok].type == space || (*activeToks)[curTok].type == period || (*activeToks)[curTok].type == comma || (*activeToks)[curTok].type == digit || (*activeToks)[curTok].type == colon || (*activeToks)[curTok].type == fSlash || (*activeToks)[curTok].type == bSlash)) {
        textNode.value += (*activeToks)[curTok].value;
        curTok++;
      }
      children.push_back(textNode);
    }
    else if (t == ul || t == ol) {
      inlineNode markerNode;
      markerNode.type = t;
      while (curTok < (*activeToks).size() && ((*activeToks)[curTok].type == newLine || (*activeToks)[curTok].type == word || (*activeToks)[curTok].type == space || (*activeToks)[curTok].type == period || (*activeToks)[curTok].type == comma || (*activeToks)[curTok].type == digit || (*activeToks)[curTok].type == colon || (*activeToks)[curTok].type == fSlash || (*activeToks)[curTok].type == bSlash)) {
        markerNode.value += (*activeToks)[curTok].value;
        curTok++;
      }
      children.push_back(markerNode);
    }
    else {
      inlineNode miscNode;
      miscNode.type = text;
      miscNode.value = (*activeToks)[curTok].value;
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
