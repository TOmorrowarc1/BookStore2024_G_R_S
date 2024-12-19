#include "String.cpp"
#include "String.hpp"
#include "account.cpp"
#include "account.hpp"
#include "tokenscanner.cpp"
#include "tokenscanner.hpp"

int rank_now = 0;

int main() {
  Token_scanner Order;
  std::string order, flag;
  while (std::cin.fail() == false) {
    std::cin >> flag;
    if (flag == "su") {
      std::getline(std::cin, order, '\n');
      Order = order;
      Account_system::sign_in(Order);
    } 
    else if (flag == "logout") {
      Account_system::log_out();
    } 
    else if (flag == "register") {
      std::getline(std::cin, order, '\n');
      Order = order;
      Account_system::Register(Order);
    } 
    else if (flag == "passwd") {
      std::getline(std::cin, order, '\n');
      Order = order;
      Account_system::Password_change(Order);
    } 
    else if (flag == "useradd") {
      std::getline(std::cin, order, '\n');
      Order = order;
      Account_system::User_add(Order);
    } 
    else if (flag == "delete") {
      std::getline(std::cin, order, '\n');
      Order = order;
      Account_system::Delete_user(Order);
    }
  }
}