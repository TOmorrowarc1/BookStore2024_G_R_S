#pragma once
#ifndef STRING_HPP
#define STRING_HPP

#include <cstring>
#include <iostream>
#include <string>

// String类：作为std::string 使用char数组的替代品，重载比较与移动赋值运算。
//同时重载输入输出运算符。
class MyString {
private:
  char content[150] = {0};

public:
  MyString() = default;
  MyString(const std::string &);
  //利用MyString初始化部分内容。
  MyString(const MyString &, int, int);
  //利用std::string部分内容初始化。
  MyString(const std::string &, int, int);
  ~MyString() = default;
  void operator=(const MyString &);
  void operator=(const std::string &);
  void change(const MyString &, int, int);
  void change(const std::string &, int, int);
  std::string return_content();
  MyString *words_split(char signal);
  void delete_quote();
  char& operator[](int);
  bool operator>(const MyString &) const;
  bool operator==(const MyString &) const;
  bool operator!=(const MyString &) const;
  bool operator>=(const MyString &) const;
  bool operator<(const MyString &) const;
  bool operator<=(const MyString &) const;
  friend std::ostream &operator<<(std::ostream &output, const MyString &object);
  friend std::istream &operator>>(std::istream &output, const MyString &object);
};

#endif