#ifndef DIARY_HPP
#define DIARY_HPP

#include "String.hpp"
#include "storage.hpp"
#include "tokenscanner.hpp"

/*日志记录系统：
该系统实现功能如下：
1.实现Trade类，记录每笔交易的具体情况。
2.记录储存库：
2.1 仅按照时间记录：0-Trade。
2.2 员工工作情况：employee-Trade。
2.3系统日志：救命，什么玩应。
3.实现如下函数：
3.1查询最近n笔交易收支情况。
3.1.5生成所有交易财务报表。
3.2查询某位员工工作情况。
3.3生成系统日志。（？！？！？）*/

namespace Diary_system {
class Trade {
private:
  int Count_number_;
  double Profit_;
  MyString Employee_;
  MyString Book_ID_;
  int Stock_change_;

public:
  Trade() = default;
  Trade(int, double, const MyString &, const MyString &, int);
  Trade(const Trade &);
  void operator=(const Trade &);
  bool operator==(const Trade &) const;
  bool operator!=(const Trade &) const;
  bool operator>(const Trade &) const;
  bool operator>=(const Trade &) const;
  bool operator<(const Trade &) const;
  bool operator<=(const Trade &) const;
  friend void print_profit(Token_scanner &);
  friend void finace_report(Token_scanner &);
  friend void employee_report(Token_scanner &);
  friend void system_diary(Token_scanner &);
};

int count = 0;

Memory<int, Trade> Diary_storage("Diary_0", "Diary_file", 1000);
Memory<MyString, Trade> Employee_diary("employee_key", "employee_value", 1000);

void print_profit(Token_scanner &);
void finace_report(Token_scanner &);
void employee_report(Token_scanner &);
void system_diary(Token_scanner &);

} // namespace Diary_system

#endif