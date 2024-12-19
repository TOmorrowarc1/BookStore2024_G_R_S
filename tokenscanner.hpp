#ifndef T_O_K_E_N
#define T_O_K_E_N

#include <cstring>
#include <iostream>
#include <string>

class Token_scanner {
private:
  std::string content;
  //储存每一串字符的起止，不包含空格。
  int space[8][2]={0};
  int pointer;
  int totalnum;

public:
  Token_scanner() = default;
  Token_scanner(const std::string &target);
  void operator=(const std::string &target);
  Token_scanner(const Token_scanner &target);
  void operator=(const Token_scanner &target);
  //返回pointer指向的字符并将pointer加1.
  std::string next_token();
  //令pointer返回上一个字符串起始。
  void move_back();
  //返回储存字符串的数量。
  int count_string();
};

#endif