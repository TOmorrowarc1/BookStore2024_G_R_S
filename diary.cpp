#include "diary.hpp"
#include "account.cpp"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdlib.h>

Diary_system::Trade::Trade() {
  Count_number_ = 0;
  Profit_ = 0;
  Stock_change_ = 0;
}

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
  if (Account_system::rank_now != 7 || order.count_string() > 2) {
    std::cout << "Invalid\n";
    return;
  }
  double profit_of_one = 0, earn = 0, cost = 0;
  if (order.count_string() == 0) {
    Trade *record = Diary_storage.all();
    for (int i = 0; i < count - 1; ++i) {
      profit_of_one = record[i].Profit_;
      if (profit_of_one > 0) {
        earn += profit_of_one;
      } else {
        cost += profit_of_one;
      }
    }
  } else {
    int count_num = 0;
    MyString count_string = order.next_token();
    for (int i = 0; count_string[i] != 0; ++i) {
      if (count_string[i] < '0' || count_string[i] > '9') {
        std::cout << "Invalid\n";
        return;
      }
    }
    count_num = std::atoi(&count_string[0]);
    if (count < count_num) {
      std::cout << "Invalid\n";
      return;
    }
    MyString blank_string;
    Trade first(count - count_num, 0, blank_string, blank_string, 0);
    Trade last(count, 0, blank_string, blank_string, 0);
    Trade *record = Diary_storage.list(0, first, last);
    for (int i = 0; record[i - 1].Count_number_ != count - 1; ++i) {
      profit_of_one = record[i].Profit_;
      if (profit_of_one > 0) {
        earn += profit_of_one;
      } else {
        cost += profit_of_one;
      }
    }
    delete[] record;
  }
  if (cost != 0) {
    cost = -cost;
  }
  std::cout << "+ " << std::fixed << std::setprecision(2) << earn << " - "
            << std::fixed << std::setprecision(2) << cost << '\n';
  return;
}
//未完待续，一会再说。
void Diary_system::finace_report(Token_scanner &);
void Diary_system::employee_report(Token_scanner &);
void Diary_system::system_diary(Token_scanner &);

void Diary_system::initialise() {
  std::fstream file;
  file.open("record_trade_count", std::fstream::in | std::fstream::out);
  if (!file) {
    file.close();
    file.open("record_trade_count", std::fstream::out);
    file.close();
    return;
  }
  file.read(reinterpret_cast<char *>(&Diary_system::count), sizeof(int));
  file.close();
  return;
}

void Diary_system::remember() {
  std::fstream file;
  file.open("record_trade_count", std::fstream::in | std::fstream::out);
  if (!file) {
    std::cout << "cannot open the file.\n";
  }
  file.write(reinterpret_cast<char *>(&Diary_system::count), sizeof(int));
  file.close();
  return;
}