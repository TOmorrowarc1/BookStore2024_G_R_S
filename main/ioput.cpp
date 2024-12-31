#include "String.hpp"
#include "book.cpp"
#include "diary.hpp"

int main() {
  Token_scanner Order;
  std::string order, flag;
  MyString flag_1;
  Account_system::start();
  Diary_system::initialise();
  int counter = 0;
  while (std::cin.fail() == false) {
    std::getline(std::cin, order, '\n');
    Order = order;
    ++counter;
    if (Order.count_string() == 0) {
      continue;
    } else {
      flag_1 = Order.next_token();
      flag = flag_1.return_content();
      if (flag == "su") {
        Account_system::sign_in(Order);
      } else if (flag == "logout") {
        Account_system::log_out();
      } else if (flag == "register") {
        Account_system::Register(Order);
      } else if (flag == "passwd") {
        Account_system::Password_change(Order);
      } else if (flag == "useradd") {
        Account_system::User_add(Order);
      } else if (flag == "delete") {
        Account_system::Delete_user(Order);
      } else if (flag == "show") {
        if (Order.count_string() == 0) {
          Order = "";
          Book_manage::show(Order);
        } else {
          MyString judge = Order.next_token();
          MyString finance("finance");
          MyString finance_report("finance_report");
          MyString employee_report("employee_report");
          MyString log("log");
          if (judge == finance) {
            Diary_system::print_profit(Order);
          } else if (judge == finance_report) {
            Diary_system::finace_report(Order);
          } else if (judge == employee_report) {
            Diary_system::employee_report(Order);
          } else if (judge == log) {
            Diary_system::system_diary(Order);
          } else {
            Order.move_back();
            Book_manage::show(Order);
          }
        }
      } else if (flag == "buy") {
        Book_manage::sell(Order);
      } else if (flag == "select") {
        Book_manage::select(Order);
      } else if (flag == "modify") {
        Book_manage::modify(Order);
      } else if (flag == "import") {
        Book_manage::import(Order);
      } else if (flag == "exit" || flag == "quit") {
        break;
      } else {
        std::cout << "Invalid\n";
      }
    }
  }
  Diary_system::remember();
  return 0;
}