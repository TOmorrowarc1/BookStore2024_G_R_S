#include "book.cpp"
#include "book.hpp"
#include "diary.hpp"

int rank_now = 0;

int main() {
  Token_scanner Order;
  std::string order, flag;
  Account_system::start();
  while (std::cin.fail() == false) {
    std::cin >> flag;
    char a = std::getchar();
    if (flag == "su") {
      std::getline(std::cin, order, '\n');
      Order = order;
      Account_system::sign_in(Order);
    } else if (flag == "logout") {
      Account_system::log_out();
    } else if (flag == "register") {
      std::getline(std::cin, order, '\n');
      Order = order;
      Account_system::Register(Order);
    } else if (flag == "passwd") {
      std::getline(std::cin, order, '\n');
      Order = order;
      Account_system::Password_change(Order);
    } else if (flag == "useradd") {
      std::getline(std::cin, order, '\n');
      Order = order;
      Account_system::User_add(Order);
    } else if (flag == "delete") {
      std::getline(std::cin, order, '\n');
      Order = order;
      Account_system::Delete_user(Order);
    } else if (flag == "show") {
      if (a == '\n') {
        Order = "";
        Book_manage::show(Order);
      } else {
        std::getline(std::cin, order, '\n');
        Order = order;
        std::string judge = Order.next_token();
        if (judge == "finance") {
          Diary_system::print_profit(Order);
        } else {
          Order.move_back();
          Book_manage::show(Order);
        }
      }
    } else if (flag == "buy") {
      std::getline(std::cin, order, '\n');
      Order = order;
      Book_manage::sell(Order);
    } else if (flag == "select") {
      std::getline(std::cin, order, '\n');
      Order = order;
      Book_manage::select(Order);
    } else if (flag == "modify") {
      std::getline(std::cin, order, '\n');
      Order = order;
      Book_manage::modify(Order);
    } else if (flag == "import") {
      std::getline(std::cin, order, '\n');
      Order = order;
      Book_manage::import(Order);
    } else if (flag == "exit") {
      break;
    } else {
      std::getline(std::cin, order, '\n');
      std::cout << "Invalid\n";
    }
  }
  return 0;
}