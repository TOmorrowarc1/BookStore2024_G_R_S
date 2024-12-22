#pragma once
#ifndef STRING_HPP
#define STRING_HPP

#include<iostream>
#include<string>
#include<cstring>

//String类：作为std::string 使用char数组的替代品，重载比较与移动赋值运算。
//同时重载输入输出运算符。
class MyString {
private:
  char content[65] = {0};

public:
  MyString() = default;
  MyString(const std::string&);
  ~MyString() = default;
  std::string return_content();
  bool operator>(const MyString &)const;
  bool operator==(const MyString &)const;
  bool operator!=(const MyString &)const;
  bool operator>=(const MyString &)const;
  bool operator<(const MyString &)const;
  bool operator<=(const MyString &)const;
  void operator=(const MyString &);
  void operator=(const std::string&);
  MyString* words_split(char signal);
  void delete_quote();
  friend std::ostream& operator<<(std::ostream& output,const MyString& object);
  friend std::istream& operator>>(std::istream& output,const MyString& object);
};

#endif