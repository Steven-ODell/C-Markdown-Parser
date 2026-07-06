#include "stringifier.h"
#include "nodes.h"
#include "tokenizer.h"
#include <iostream>


Stringifier::Stringifier(std::vector<blockNode>& fullTree) : tree(fullTree){
  curBlock = 0;
  curChild = 0;
  outputString = "";
  curInline = 0;
}

std::string Stringifier::createHTML() {
  while (curBlock < tree.size()){
    if (tree[curBlock].type == h1) {
      outputString.append("<h1>");
      while (tree[curBlock].children.size() > curChild) {
        outputString.append(tree[curBlock].children[curChild].value);
        curChild++;
      }
      curChild = 0;
      outputString.append("</h1>\n");
    }
    else if (tree[curBlock].type == h2) {
      outputString.append("<h2>");
      while (tree[curBlock].children.size() > curChild) {
        outputString.append(tree[curBlock].children[curChild].value);
        curChild++;
      }
      curChild = 0;
      outputString.append("</h2>\n");
    }
    else if (tree[curBlock].type == h3) {
      outputString.append("<h3>");
      while (tree[curBlock].children.size() > curChild) {
        outputString.append(tree[curBlock].children[curChild].value);
        curChild++;
      }
      curChild = 0;
      outputString.append("</h3>\n");
    }
    else if (tree[curBlock].type == hr) {
      outputString.append("<hr>\n");
      while (tree[curBlock].children.size() > curChild) {
        outputString.append(tree[curBlock].children[curChild].value);
        curChild++;
      }
      curChild = 0;
    }
    else if (tree[curBlock].type == codeblock) {
      outputString.append("<pre><code>");
      while (tree[curBlock].children.size() > curChild) {
        outputString.append(tree[curBlock].children[curChild].value);
        curChild++;
      }
      curChild = 0;
      outputString.append("</code></pre>\n");
    }
    else if (tree[curBlock].type == blockquote) {
      outputString.append("<blockquote>");
      while (tree[curBlock].children.size() > curChild) {
        outputString.append(tree[curBlock].children[curChild].value);
        curChild++;
      }
      curChild = 0;
      outputString.append("</blockquote>\n");
    }
    else if (tree[curBlock].type == p) {
      outputString.append("<p>");
      for (size_t i = 0; i < tree[curBlock].children.size(); i++) {
        outputString.append(createInlineHTML(tree[curBlock].children[i]));
      }
      outputString.append("</p>\n");
    }
    else if (tree[curBlock].type == ul) {
      outputString.append("<ul>\n");
      for (size_t i = 0; i < tree[curBlock].children.size(); i++) {
        outputString.append("<li>");
        outputString.append(createInlineHTML(tree[curBlock].children[i]));
        for (size_t k = 0; k < tree[curBlock].children[i].children.size(); k++) {
          if (tree[curBlock].children[i].children[k].type == ul) {
            ulLoop(tree[curBlock].children[i].children[k]);
          }
          else {
            outputString.append(createInlineHTML(tree[curBlock].children[i].children[k]));
          }
        }
        outputString.append("</li>\n");
      }
      outputString.append("</ul>\n");
    }
    else if (tree[curBlock].type == ol) {
      outputString.append("<ol>\n");
      for (size_t i = 0; i < tree[curBlock].children.size(); i++) {
        outputString.append("<li>");
        outputString.append(createInlineHTML(tree[curBlock].children[i]));
        for (size_t k = 0; k < tree[curBlock].children[i].children.size(); k++) {
          if (tree[curBlock].children[i].children[k].type == ol) {
            olLoop(tree[curBlock].children[i].children[k]);
          }
          else {
            outputString.append(createInlineHTML(tree[curBlock].children[i].children[k]));
          }
        }
        outputString.append("</li>\n");
      }
      outputString.append("</ol>\n");
    }
    curBlock++;
  } 
  std::cout << outputString << std::endl;
  return outputString;
}

void Stringifier::ulLoop(inlineNode& listNode) {
    outputString.append("<ul>\n");
    for (size_t i = 0; i < listNode.children.size(); i++) {
      outputString.append("<li>");
      outputString.append(createInlineHTML(listNode.children[i]));
      for (size_t k = 0; k < listNode.children[i].children.size(); k++) {
        if (listNode.children[i].children[k].type == ul) {
          ulLoop(listNode.children[i].children[k]);
        }
        else {
          outputString.append(createInlineHTML(listNode.children[i].children[k]));
        }
      }
      outputString.append("</li>\n");
    }
    outputString.append("</ul>");
}

void Stringifier::olLoop(inlineNode& listNode) {
    outputString.append("<ol>\n");
    for (size_t i = 0; i < listNode.children.size(); i++) {
      outputString.append("<li>");
      outputString.append(createInlineHTML(listNode.children[i]));
      for (size_t k = 0; k < listNode.children[i].children.size(); k++) {
        if (listNode.children[i].children[k].type == ol) {
          olLoop(listNode.children[i].children[k]);
        }
        else {
          outputString.append(createInlineHTML(listNode.children[i].children[k]));
        }
      }
      outputString.append("</li>\n");
    }
    outputString.append("</ol>");
}

std::string Stringifier::createInlineHTML(inlineNode& node) {
  if (node.type == text) {
    return node.value;
  }
  else if (node.type == em) {
    std::string inner;
    for (size_t i = 0; i < node.children.size(); i++) {
      inner += createInlineHTML(node.children[i]);
    }
    return "<em>" + inner + "</em>";
  }
  else if (node.type == strong) {
    std::string inner;
    for (size_t i = 0; i < node.children.size(); i++) {
      inner += createInlineHTML(node.children[i]);
    }
    return "<strong>" + inner + "</strong>";
  }
  else if (node.type == code) {
    std::string inner;
    for (size_t i = 0; i < node.children.size(); i++) {
      inner += createInlineHTML(node.children[i]);
    }
    return "<code>" + inner + "</code>";
  }
  else if (node.type == link) {
    std::string url;
    std::string linkName;
    linkName = node.children[0].value;
    for (size_t i = 0; i < node.children.size(); i++) {
      url += createInlineHTML(node.children[i].children[i]);
    }
    return "<a href=\"" + url + "\">" + linkName + "</a>";
    
  }
  return "";
}

