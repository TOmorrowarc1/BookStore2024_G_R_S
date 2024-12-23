#ifndef BOOKMANAGE_HPP
#define BOOKMANAGE_HPP

#include "String.hpp"
#include "storage.hpp"
#include "tokenscanner.hpp"
#include <unordered_map>

class MyString;
class Token_scanner;

/*在书籍管理系统中，本程序实现如下功能：
1.实现Book类，储存一本书籍相关信息：ISBN，书名，作者名，关键词，库存书籍数量，单价。
2.系统有关文件如下：ISBN-Book储存库，书名/作者名/关键词-ISBN储存库
3.需要实现如下函数：
3.1 通过ISBN查找对应图书并返回其信息。
3.2 通过其他三方面词汇查找对应ISBN信息。
3.3 选中图书。（需要与account中logout，sign_in联动）
3.3 创建、修改图书除库存外信息，并更新所有相关数据库。
3.4 购买图书。
3.5 图书进货。*/

namespace Book_manage {
class Book {
  //比较ISBN号以区分大小。
private:
  MyString ISBN_;
  MyString Book_name_;
  MyString Author_;
  //用一串字符串配合对应|储存关键字。
  MyString Key_string;
  int num_storage = 0;
  double price = 0;

public:
  Book() = default;
  Book(const MyString &, const MyString &, const MyString &, const MyString &,
       int, double);
  Book(const Book &);
  void operator=(const Book &);
  bool operator==(const Book &) const;
  bool operator!=(const Book &) const;
  bool operator>(const Book &) const;
  bool operator>=(const Book &) const;
  bool operator<(const Book &) const;
  bool operator<=(const Book &) const;
  friend void select(Token_scanner &);
  friend void modify(Token_scanner &);
  friend void show(Token_scanner &);
  //记得输出浮点数。
  friend void sell(Token_scanner &);
  friend void import(Token_scanner &);
};

//如何使用自定义类？
std::unordered_map<std::string, Book> selection;

Memory<MyString, Book> ISBN_book("ISBN_file", "Book_file", 1000);
Memory<MyString, Book> bookname_book("bookname_file", "bookname_ISBN_file",
                                         1000);
Memory<MyString, Book> author_book("author_file", "author_ISBN_file", 1000);
Memory<MyString, Book> keywords_book("keywords_file", "keywords_ISBN_file",
                                         1000);

void select(Token_scanner &);
void modify(Token_scanner &);
void show(Token_scanner &);
//记得输出浮点数。
void sell(Token_scanner &);
void import(Token_scanner &);

} // namespace Book_manage

#endif
