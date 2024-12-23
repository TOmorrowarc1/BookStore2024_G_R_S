#include "tokenscanner.hpp"

//读入一个内部含空格，以"\0"结尾的字符串，做如下操作：
//遍历整个字符串，记录空格之后的第一个字符的位置，标记每一串字符串的起止位置。
//每确定一串就将总数加一。
Token_scanner::Token_scanner(const std::string &target) {
  content = target;
  totalnum = 0;
  pointer = 0;
  space[0][0] = 0;
  for (int i = 0; i <= target.length(); ++i) {
    if (target[i] == ' ' && target[i - 1] != ' ') {
      space[totalnum][1] = i - 1;
      ++totalnum;
    }
    if (i != 0 && target[i] != ' ' && target[i - 1] == ' ') {
      space[totalnum][0] = i;
    }
  }
}

void Token_scanner::operator=(const std::string &target) {
  content = target;
  totalnum = 0;
  pointer = 0;
  space[0][0] = 0;
  for (int i = 1; i <= target.length(); ++i) {
    if ((target[i] == ' ' || target[i] == 0) &&
        (target[i - 1] != ' ' && target[i - 1] != 0)) {
      space[totalnum][1] = i - 1;
      ++totalnum;
    }
    if (i != 0 && target[i] != ' ' && target[i - 1] == ' ') {
      space[totalnum][0] = i;
    }
  }
}

Token_scanner::Token_scanner(const Token_scanner &target) {
  content = target.content;
  totalnum = target.totalnum;
  for (int i = 0; i < totalnum; ++i) {
    space[i][0] = target.space[i][0];
    space[i][1] = target.space[i][1];
  }
  pointer = 0;
}

void Token_scanner::operator=(const Token_scanner &target) {
  content = target.content;
  totalnum = target.totalnum;
  for (int i = 0; i < totalnum; ++i) {
    space[i][0] = target.space[i][0];
    space[i][1] = target.space[i][1];
  }
  pointer = 0;
}

//返回pointer指向的字符并将pointer加1.
std::string Token_scanner::next_token() {
  if (pointer >= totalnum) {
    return "\0";
  }
  std::string result(&content[space[pointer][0]],&content[space[pointer][1] + 1]);
  ++pointer;
  return result;
}

//令pointer返回上一个字符串起始。
void Token_scanner::move_back() {
  if (pointer == 0) {
    return;
  }
  --pointer;
  return;
}

//返回储存字符串的数量。
int Token_scanner::count_string() { return totalnum; }