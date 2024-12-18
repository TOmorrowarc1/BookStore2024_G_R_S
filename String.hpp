#ifndef S_T_R_I_N_G_H_P_P
#define S_T_R_I_N_G_H_P_P

#include<iostream>
#include<string>
#include<cstring>

//String类：作为std::string 使用char数组的替代品，重载比较与移动赋值运算。
class String {
private:
  char content[65] = {0};

public:
  String() = default;
  String(std::string);
  ~String() = default;
  bool operator>(const String &)const;
  bool operator==(const String &)const;
  bool operator!=(const String &)const;
  bool operator>=(const String &)const;
  bool operator<(const String &)const;
  bool operator<=(const String &)const;
  void operator=(const String &);
  void operator=(const std::string&);
};

#endif