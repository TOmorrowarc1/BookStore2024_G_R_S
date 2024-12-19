#ifndef STRING_HPP
#define STRING_HPP

#include<iostream>
#include<string>
#include<cstring>

//String类：作为std::string 使用char数组的替代品，重载比较与移动赋值运算。
//同时重载输入输出运算符。
class String {
private:
  char content[65] = {0};

public:
  String() = default;
  String(const std::string&);
  ~String() = default;
  bool operator>(const String &)const;
  bool operator==(const String &)const;
  bool operator!=(const String &)const;
  bool operator>=(const String &)const;
  bool operator<(const String &)const;
  bool operator<=(const String &)const;
  void operator=(const String &);
  void operator=(const std::string&);
  friend std::ostream& operator<<(std::ostream& output,const String& object);
  friend std::istream& operator>>(std::istream& output,const String& object);
};

#endif