#include "String.cpp"
#include "String.hpp"
#include "tokenscanner.cpp"
#include "tokenscanner.hpp"

int rank_now = 0;

int main() {
  Token_scanner Order;
  std::string order, flag;
  while (std::cin.fail() == false) {
    std::cin >> flag;
    std::getchar();
    if (flag == "su") {
      std::getline(std::cin, order, '\n');
      Order = order;
    } else if (flag == "logout") {
    } else if (flag == "register") {
      std::getline(std::cin, order, '\n');
      Order = order;
      std::string target;
      for (int i = 0; i < 4; ++i) {
        target = Order.next_token();
        std::cout << target << '\n';
      }
      Order.move_back();
      Order.move_back();
      target = Order.next_token();
      std::cout << target;
    } else if (flag == "passwd") {
      std::getline(std::cin, order, '\n');
      Order = order;
    } else if (flag == "useradd") {
      std::getline(std::cin, order, '\n');
      Order = order;
    } else if (flag == "delete") {
      std::getline(std::cin, order, '\n');
      Order = order;
    }
  }
}