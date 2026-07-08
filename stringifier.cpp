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
        if (tree[curBlock].children[curChild].children.size() > 0) {
          outputString.append(createInlineHTML(tree[curBlock].children[curChild]));
        }
        outputString.append(tree[curBlock].children[curChild].value);
        curChild++;
      }
      curChild = 0;
      outputString.append("</h1>\n");
    }
    else if (tree[curBlock].type == h2) {
      outputString.append("<h2>");
      while (tree[curBlock].children.size() > curChild) {
        if (tree[curBlock].children[curChild].children.size() > 0) {
          outputString.append(createInlineHTML(tree[curBlock].children[curChild]));
        }
        outputString.append(tree[curBlock].children[curChild].value);
        curChild++;
      }
      curChild = 0;
      outputString.append("</h2>\n");
    }
    else if (tree[curBlock].type == h3) {
      outputString.append("<h3>");
      while (tree[curBlock].children.size() > curChild) {
        if (tree[curBlock].children[curChild].children.size() > 0) {
          outputString.append(createInlineHTML(tree[curBlock].children[curChild]));
        }
        outputString.append(tree[curBlock].children[curChild].value);
        curChild++;
      }
      curChild = 0;
      outputString.append("</h3>\n");
    }
    else if (tree[curBlock].type == h4) {
      outputString.append("<h4>");
      while (tree[curBlock].children.size() > curChild) {
        if (tree[curBlock].children[curChild].children.size() > 0) {
          outputString.append(createInlineHTML(tree[curBlock].children[curChild]));
        }
        outputString.append(tree[curBlock].children[curChild].value);
        curChild++;
      }
      curChild = 0;
      outputString.append("</h4>\n");
    }
    else if (tree[curBlock].type == h5) {
      outputString.append("<h5>");
      while (tree[curBlock].children.size() > curChild) {
        if (tree[curBlock].children[curChild].children.size() > 0) {
          outputString.append(createInlineHTML(tree[curBlock].children[curChild]));
        }
        outputString.append(tree[curBlock].children[curChild].value);
        curChild++;
      }
      curChild = 0;
      outputString.append("</h5>\n");
    }
    else if (tree[curBlock].type == h6) {
      outputString.append("<h6>");
      while (tree[curBlock].children.size() > curChild) {
        if (tree[curBlock].children[curChild].children.size() > 0) {
          outputString.append(createInlineHTML(tree[curBlock].children[curChild]));
        }
        outputString.append(tree[curBlock].children[curChild].value);
        curChild++;
      }
      curChild = 0;
      outputString.append("</h6>\n");
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
  if (node.type == text || node.type == dash) {
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
  else if (node.type == mark) {
    std::string inner;
    for (size_t i = 0; i < node.children.size(); i++) {
      inner += createInlineHTML(node.children[i]);
    }
    return "<mark>" + inner + "</mark>";
  }
  else if (node.type == del) {
    std::string inner;
    for (size_t i = 0; i < node.children.size(); i++) {
      inner += createInlineHTML(node.children[i]);
    }
    return "<del>" + inner + "</del>";
  }
  else if (node.type == sub) {
    std::string inner;
    inner = node.children[0].value;
    return "<sub>" + inner + "</sub>";
  }
  else if (node.type == sup) {
    std::string inner;
    inner = node.children[0].value;
    return "<sup>" + inner + "</sup>";
  }
  else if (node.type == link) {
    std::string url;
    std::string linkName;
    linkName = node.children[0].value;
    for (size_t i = 0; i < node.children[0].children.size(); i++) {
      url += createInlineHTML(node.children[0].children[i]);
    }
    return "<a href=\"" + url + "\">" + linkName + "</a>";
    
  }
  else if (node.type == image) {
    std::string url;
    std::string imgName;
    imgName = node.children[0].value;
    for (size_t i = 0; i < node.children[0].children.size(); i++) {
      url += createInlineHTML(node.children[0].children[i]);
    }
    return "<img src=\"" + url + "\" alt=\"" + imgName + "\">";
     
    
  }
  return "";
}

