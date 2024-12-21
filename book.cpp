//用于字符串转浮点数。
#include "book.hpp"
#include "String.hpp"
#include "account.hpp"
#include <iomanip>
#include <stdlib.h>

Book_manage::Book::Book(const MyString &ISBN, const MyString &Book_name,
                        const MyString &Author, const MyString &Key_words,
                        int storage, double value) {
  ISBN_ = ISBN;
  Book_name_ = Book_name;
  Author_ = Author;
  Key_string = Key_words;
  num_storage = storage;
  price = value;
}

Book_manage::Book::Book(const Book &B) {
  ISBN_ = B.ISBN_;
  Book_name_ = B.Book_name_;
  Author_ = B.Author_;
  Key_string = B.Key_string;
  num_storage = B.num_storage;
  price = B.price;
}

void Book_manage::Book::operator=(const Book &B) {
  ISBN_ = B.ISBN_;
  Book_name_ = B.Book_name_;
  Author_ = B.Author_;
  Key_string = B.Key_string;
  num_storage = B.num_storage;
  price = B.price;
}

bool Book_manage::Book::operator==(const Book &B) { return ISBN_ == B.ISBN_; }

bool Book_manage::Book::operator!=(const Book &B) { return ISBN_ != B.ISBN_; }

bool Book_manage::Book::operator>(const Book &B) { return ISBN_ > B.ISBN_; }

bool Book_manage::Book::operator>=(const Book &B) { return ISBN_ >= B.ISBN_; }

bool Book_manage::Book::operator<(const Book &B) { return ISBN_ < B.ISBN_; }

bool Book_manage::Book::operator<=(const Book &B) { return ISBN_ <= B.ISBN_; }




//下面是函数的正式实现：
// 1.选中对应图书：
void Book_manage::select(Token_scanner &order) {
  if (order.count_string() != 1 || Account_system::rank_now < 3) {
    std::cout << "Invalid.\n";
    return;
  }
  MyString ISBN = order.next_token();
  MyString User_ID = Account_system::Account_record.top().user_id();
  std::string user_id = User_ID.return_content();
  Book blank, target;
  target = storage.search(User_ID, blank);
  if (target.ISBN_ != ISBN) {
    blank.ISBN_ = ISBN;
    storage.insert(storage.create(ISBN, blank));
    selection[user_id] = blank;
  } else {
    selection[user_id] = target;
  }
  return;
}


// 2.修改图书信息
void Book_manage::modify(Token_scanner &order) {
  int flag = 0, pointer = 0;
  bool change[6] = {0};
  MyString ISBN, Book_name, Author, Keywords;
  double Price = 0;
  if (order.count_string() <= 1 || Account_system::rank_now < 3) {
    std::cout << "Invalid.\n";
    return;
  }
  //读取对应信息方法：锁定=.
  for (int i = 0; i < order.count_string() - 1; ++i) {
    std::string token = order.next_token();
    std::string type_string, message;
    char type[10] = {0};
    for (int words = 0;; ++words) {
      if (token[words - 1] == '=') {
        type_string = type;
        std::strcpy(&message[0], &token[words]);
        break;
      }
      type[words] = token[words];
    }
    if (type_string == "-ISBN=") {
      if (flag > 1 || change[1] == 1) {
        std::cout << "Invalid.\n";
        return;
      }
      flag = 1;
      change[1] = 1;
      ISBN = message;
    } else if (type_string == "-name=") {
      if (flag > 2 || change[2] == 1) {
        std::cout << "Invalid.\n";
        return;
      }
      flag = 2;
      change[2] = 1;
      Book_name = message;
    } else if (type_string == "-author=") {
      if (flag > 3 || change[3] == 1) {
        std::cout << "Invalid.\n";
        return;
      }
      flag = 3;
      change[3] = 1;
      Author = message;
    } else if (type_string == "-keyword=") {
      if (flag > 4 || change[4] == 1) {
        std::cout << "Invalid.\n";
        return;
      }
      flag = 4;
      change[4] = 1;
      Keywords = message;
    } else if (type_string == "-price=") {
      if (flag > 5 || change[5] == 1) {
        std::cout << "Invalid.\n";
        return;
      }
      flag = 5;
      change[5] = 1;
      Price = std::atof(&message[0]);
    }
  }
  //以上为输入部分，接下来修改对应文件。
  MyString User_ID = Account_system::Account_record.top().user_id();
  std::string user_id = User_ID.return_content();
  //找出原来书籍信息。
  Book origin = selection[user_id];
  Book now(origin);
  //根据输入内容修改本书信息。
  if (change[1]) {
    now.ISBN_ = ISBN;
  }
  if (change[2]) {
    now.Book_name_ = Book_name;
  }
  if (change[3]) {
    now.Author_ = Author;
  }
  if (change[4]) {
    now.Key_string = Keywords;
  }
  if (change[5]) {
    now.price = Price;
  }
  //开始向文件输入：向storage输入，向bookname_ISBN输入，向author_ISBN输入，向keywords_ISBN输入。
  storage.erase(storage.create(origin.ISBN_, origin));
  storage.insert(storage.create(now.ISBN_, now));
  bookname_ISBN.erase(bookname_ISBN.create(origin.Book_name_, origin.ISBN_));
  bookname_ISBN.insert(bookname_ISBN.create(now.Book_name_, now.ISBN_));
  author_ISBN.erase(author_ISBN.create(origin.Author_, origin.ISBN_));
  author_ISBN.insert(author_ISBN.create(now.Author_, now.ISBN_));
  MyString *catcher_1 = now.Key_string.words_split('|');
  MyString *catcher_2 = origin.Key_string.words_split('|');
  MyString blank_string;
  for (int i = 0; catcher_1[i] != blank_string; ++i) {
    keywords_ISBN.erase(keywords_ISBN.create(catcher_1[i], origin.ISBN_));
  }
  for (int i = 0; catcher_2[i] != blank_string; ++i) {
    keywords_ISBN.insert(keywords_ISBN.create(catcher_2[i], now.ISBN_));
  }
  delete[] catcher_1;
  delete[] catcher_2;
  selection[user_id] = now;
  return;
}


void Book_manage::show(Token_scanner &order) {

}


void Book_manage::sell(Token_scanner &order) {
  if (order.count_string() != 2 || Account_system::rank_now < 1) {
    std::cout << "Invalid\n";
    return;
  }
  MyString ISBN;
  std::string number;
  Book blank;
  ISBN = order.next_token();
  Book target = storage.search(ISBN, blank);
  if (target.ISBN_ != ISBN) {
    std::cout << "Invalid\n";
    return;
  }
  for (int i = 0; number[i] != 0; ++i) {
    if (number[i] < '0' || number[i] > '9') {
      std::cout << "Invalid\n";
      return;
    }
  }
  int num = std::atoi(&number[0]);
  if (num > target.num_storage) {
    std::cout << "Invalid\n";
    return;
  }
  std::cout << std::fixed << std::setprecision(2) << (num * target.price)
            << '\n';
  target.num_storage -= num;
  storage.erase(storage.create(ISBN, target));
  storage.insert(storage.create(ISBN, target));
  return;
}


void Book_manage::import(Token_scanner &order) {
  if (order.count_string() != 2 || Account_system::rank_now < 3) {
    std::cout << "Invalid\n";
    return;
  }
  MyString User_ID = Account_system::Account_record.top().user_id();
  std::string user_id = User_ID.return_content();
  //找出原来书籍信息。
  if (selection.find(user_id) == selection.end()) {
    std::cout << "Invalid\n";
    return;
  }
  Book origin = selection[user_id];
  std::string quantity, totalcost;
  int num_quantity = 0;
  quantity = order.next_token();
  totalcost = order.next_token();
  for (int i = 0; quantity[i] != 0; ++i) {
    if (quantity[i] < '0' || quantity[i] > '9') {
      std::cout << "Invalid\n";
      return;
    }
  }
  num_quantity = atoi(&quantity[0]);
  for (int i = 0; totalcost[i] != 0; ++i) {
    if (totalcost[i] < '0' || totalcost[i] > '9') {
      std::cout << "Invalid\n";
      return;
    }
  }
  origin.num_storage += num_quantity;
  storage.erase(storage.create(origin.ISBN_, origin));
  storage.insert(storage.create(origin.ISBN_, origin));
  return;
}