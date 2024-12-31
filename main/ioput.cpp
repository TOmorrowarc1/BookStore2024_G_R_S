#include "String.hpp"
#include "book.cpp"
#include "book.hpp"
#include "diary.hpp"

int main() {
  Token_scanner Order;
  std::string order, flag;
  MyString flag_1;
  Account_system::start();
  Diary_system::initialise();
  Diary_system::Operation operation;
  int counter = 0;
  while (std::cin.fail() == false) {
    std::getline(std::cin, order, '\n');
    Order = order;
    if (Order.count_string() == 0) {
      continue;
    } else {
      int valid = 1;
      flag_1 = Order.next_token();
      flag = flag_1.return_content();
      if (flag == "su") {
        Account_system::sign_in(Order);
      } else if (flag == "logout") {
        valid = 2;
        if (!Book_manage::Account_selection.empty()) {
          operation.user_name =
              Book_manage::Account_selection.top().account_now.user_name();
        }
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
            std::cout << "这张财务报表或许是赏心悦目的。\n";
            Diary_system::finace_report(Order);
          } else if (judge == employee_report) {
            std::cout << "这张员工工作报表大概是赏心悦目的。\n";
            Diary_system::employee_report(Order);
          } else if (judge == log) {
            std::cout << "这张系统工作报表可能是赏心悦目的。\n";
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
        valid = 0;
      }
      if (valid != 0) {
        ++counter;
        operation.counter = counter;
        if (valid != 2) {
          if (Book_manage::Account_selection.empty()) {
            operation.user_name = "";
          } else {
            operation.user_name =
                Book_manage::Account_selection.top().account_now.user_name();
          }
        }
        operation.order_record = order;
        Diary_system::System_record.insert(
            Diary_system::System_record.create(counter, operation));
      }
    }
  }
  Diary_system::remember();
  return 0;
}