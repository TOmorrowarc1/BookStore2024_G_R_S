#include "diary.hpp"
#include "String.hpp"
#include "account.hpp"
#include "tokenscanner.hpp"
#include <iomanip>
#include <iostream>
#include <stdlib.h>

Diary_system::Trade::Trade(int count_number, double profit,
                           const MyString &employee, const MyString &ISBN,
                           int number) {
  Count_number_ = count_number;
  Profit_ = profit;
  Employee_ = employee;
  Book_ID_ = ISBN;
  Stock_change_ = number;
}

Diary_system::Trade::Trade(const Trade &B) {
  Count_number_ = B.Count_number_;
  Profit_ = B.Profit_;
  Employee_ = B.Employee_;
  Book_ID_ = B.Book_ID_;
  Stock_change_ = B.Stock_change_;
}

void Diary_system::Trade::operator=(const Trade &B) {
  Count_number_ = B.Count_number_;
  Profit_ = B.Profit_;
  Employee_ = B.Employee_;
  Book_ID_ = B.Book_ID_;
  Stock_change_ = B.Stock_change_;
}

bool Diary_system::Trade::operator==(const Trade &B) const {
  return Count_number_ == B.Count_number_;
}

bool Diary_system::Trade::operator!=(const Trade &B) const {
  return Count_number_ != B.Count_number_;
}

bool Diary_system::Trade::operator>(const Trade &B) const {
  //因为大者在后，为速度conut大者为大（顺序）。
  //此处可以优化。
  return Count_number_ > B.Count_number_;
}
bool Diary_system::Trade::operator>=(const Trade &B) const {
  return Count_number_ >= B.Count_number_;
}

bool Diary_system::Trade::operator<(const Trade &B) const {
  return Count_number_ < B.Count_number_;
}

bool Diary_system::Trade::operator<=(const Trade &B) const {
  return Count_number_ <= B.Count_number_;
}

void Diary_system::print_profit(Token_scanner &order) {
  if (Account_system::rank_now != 7 || order.count_string() > 1) {
    std::cout << "Invalid\n";
    return;
  }
  double profit_of_one = 0, earn = 0, cost = 0;
  if (order.count_string() == 0) {
    Trade *record = Diary_storage.all();
    Trade blank_trade;
    for (int i = 0; record[i] != blank_trade; ++i) {
      profit_of_one = record[i].Profit_;
      if (profit_of_one > 0) {
        earn += profit_of_one;
      } else {
        cost += profit_of_one;
      }
    }
  } else {
    int count_num = 0;
    std::string count_string = order.next_token();
    count_num = std::atoi(&count_string[0]);
    if (count < count_num) {
      std::cout << "Invalid\n";
      return;
    }
    MyString blank_string;
    Trade blank_trade;
    Trade first(count - count_num, 0, blank_string, blank_string, 0);
    Trade last(count, 0, blank_string, blank_string, 0);
    Trade *record = Diary_storage.list(0, first, last);
    for (int i = 0; record[i] != blank_trade; ++i) {
      profit_of_one = record[i].Profit_;
      if (profit_of_one > 0) {
        earn += profit_of_one;
      } else {
        cost += profit_of_one;
      }
    }
  }
  cost = -cost;
  std::cout << "+ " << std::fixed << std::setprecision(2) << earn << " - "
            << std::fixed << std::setprecision(2) << cost << '\n';
  return;
}

void Diary_system::finace_report(Token_scanner &);
void Diary_system::employee_report(Token_scanner &);
void Diary_system::system_diary(Token_scanner &);