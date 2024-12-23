//用于字符串转浮点数。
#include "book.hpp"
#include "String.hpp"
#include "account.cpp"
#include "account.hpp"
#include <iomanip>
#include <set>
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

bool Book_manage::Book::operator==(const Book &B) const {
  return ISBN_ == B.ISBN_;
}

bool Book_manage::Book::operator!=(const Book &B) const {
  return ISBN_ != B.ISBN_;
}

bool Book_manage::Book::operator>(const Book &B) const {
  return ISBN_ > B.ISBN_;
}

bool Book_manage::Book::operator>=(const Book &B) const {
  return ISBN_ >= B.ISBN_;
}

bool Book_manage::Book::operator<(const Book &B) const {
  return ISBN_ < B.ISBN_;
}

bool Book_manage::Book::operator<=(const Book &B) const {
  return ISBN_ <= B.ISBN_;
}

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
  target = ISBN_book.search(ISBN, blank);
  if (target.ISBN_ != ISBN) {
    blank.ISBN_ = ISBN;
    ISBN_book.insert(ISBN_book.create(ISBN, blank));
    selection[user_id] = blank;
  } else {
    selection[user_id] = target;
  }
  return;
}

// 2.修改图书信息
void Book_manage::modify(Token_scanner &order) {
  bool change[6] = {0};
  MyString ISBN, Book_name, Author, Keywords, blank_string;
  MyString max_string("~~~~~~~~~");
  double Price = 0;
  if (order.count_string() == 0 || Account_system::rank_now < 3) {
    std::cout << "Invalid.\n";
    return;
  }
  //读取对应信息方法：锁定=.
  for (int i = 0; i < order.count_string(); ++i) {
    std::string token = order.next_token();
    std::string type_string, message;
    char type[10] = {0};
    for (int words = 0;; ++words) {
      if (token[words - 1] == '=') {
        type_string = type;
        message = (&token[words]);
        break;
      }
      type[words] = token[words];
    }
    if (type_string == "-ISBN=") {
      if (change[1] == 1) {
        std::cout << "Invalid.\n";
        return;
      }
      change[1] = 1;
      ISBN = message;
    } else if (type_string == "-name=") {
      if (change[2] == 1) {
        std::cout << "Invalid.\n";
        return;
      }
      change[2] = 1;
      Book_name = message;
      Book_name.delete_quote();
    } else if (type_string == "-author=") {
      if (change[3] == 1) {
        std::cout << "Invalid.\n";
        return;
      }
      change[3] = 1;
      Author = message;
      Author.delete_quote();
    } else if (type_string == "-keyword=") {
      if (change[4] == 1) {
        std::cout << "Invalid.\n";
        return;
      }
      change[4] = 1;
      Keywords = message;
      Keywords.delete_quote();
    } else if (type_string == "-price=") {
      if (change[5] == 1) {
        std::cout << "Invalid.\n";
        return;
      }
      change[5] = 1;
      Price = std::atof(&message[0]);
    }
  }
  //以上为输入部分，接下来修改对应文件。
  MyString User_ID = Account_system::Account_record.top().user_id();
  std::string user_id = User_ID.return_content();
  //找出原来书籍信息。
  if (selection.find(user_id) == selection.end()) {
    std::cout << "Invalid.\n";
    return;
  }
  Book origin = selection[user_id];
  Book now(origin);
  //根据输入内容修改本书信息。
  if (change[1]) {
    if (ISBN == origin.ISBN_) {
      std::cout << "Invalid.\n";
      return;
    }
    now.ISBN_ = ISBN;
  }
  if (change[2]) {
    now.Book_name_ = Book_name;
  }
  if (change[3]) {
    now.Author_ = Author;
  }
  if (change[4]) {
    MyString *check = Keywords.words_split('|');
    for (int i = 0; check[i] != blank_string; ++i) {
      for (int j = i + 1; check[j] != blank_string; ++j) {
        if (check[i] == check[j]) {
          delete[] check;
          std::cout << "Invalid.\n";
          return;
        }
      }
    }
    now.Key_string = Keywords;
  }
  if (change[5]) {
    now.price = Price;
  }
  //开始向文件输入：向storage输入，向bookname_ISBN输入，向author_ISBN输入，向keywords_ISBN输入。
  ISBN_book.erase(ISBN_book.create(origin.ISBN_, origin));
  ISBN_book.insert(ISBN_book.create(now.ISBN_, now));
  bookname_book.erase(bookname_book.create(origin.Book_name_, origin));
  bookname_book.insert(bookname_book.create(now.Book_name_, now));
  author_book.erase(author_book.create(origin.Author_, origin));
  author_book.insert(author_book.create(now.Author_, now));
  MyString *catcher_1 = now.Key_string.words_split('|');
  MyString *catcher_2 = origin.Key_string.words_split('|');
  for (int i = 0; catcher_2[i] != blank_string; ++i) {
    keywords_book.erase(keywords_book.create(catcher_2[i], origin));
  }
  for (int i = 0; catcher_1[i] != blank_string; ++i) {
    keywords_book.insert(keywords_book.create(catcher_1[i], now));
  }
  delete[] catcher_1;
  delete[] catcher_2;
  selection[user_id] = now;
  return;
}

void Book_manage::show(Token_scanner &order) {
  if (Account_system::rank_now == 0) {
    std::cout << "Invalid.\n";
    return;
  }
  if (order.count_string() == 0) {
    MyString blank_string;
    Book blank;
    Book *all = ISBN_book.all();
    for (int i = 0; all[i] != blank; ++i) {
      std::cout << all[i].ISBN_ << '\t' << all[i].Book_name_ << '\t'
                << all[i].Author_ << '\t' << all[i].Key_string << '\t'
                << std::fixed << std::setprecision(2) << all[i].price << '\t'
                << all[i].num_storage << '\n';
    }
  } else {
    bool index[5] = {0};
    MyString ISBN, Book_name, Author, Keywords, blank_string, max_string("~~~");
    Book blank_book;
    Book max_book(max_string, blank_string, blank_string, blank_string, 0, 0);
    double Price = 0;
    if (order.count_string() == 0 || Account_system::rank_now == 0) {
      std::cout << "Invalid.\n";
      return;
    }
    //读取对应信息方法：锁定=.
    for (int i = 0; i < order.count_string(); ++i) {
      std::string token_1 = order.next_token();
      std::string type_string_1, message_1;
      char type[10] = {0};
      for (int words = 0;; ++words) {
        if (token_1[words - 1] == '=') {
          type_string_1 = type;
          message_1 = (&token_1[words]);
          break;
        }
        type[words] = token_1[words];
      }
      if (type_string_1 == "-ISBN=") {
        if (index[1] == 1) {
          std::cout << "Invalid.\n";
          return;
        }
        index[1] = 1;
        ISBN = message_1;
      } else if (type_string_1 == "-name=") {
        if (index[2] == 1) {
          std::cout << "Invalid.\n";
          return;
        }
        index[2] = 1;
        Book_name = message_1;
        Book_name.delete_quote();
      } else if (type_string_1 == "-author=") {
        if (index[3] == 1) {
          std::cout << "Invalid.\n";
          return;
        }
        index[3] = 1;
        Author = message_1;
        Author.delete_quote();
      } else if (type_string_1 == "-keyword=") {
        if (index[4] == 1) {
          std::cout << "Invalid.\n";
          return;
        }
        index[4] = 1;
        Keywords = message_1;
        Keywords.delete_quote();
      } else {
        std::cout << "Invalid.\n";
        return;
      }
    }
    //信息读取完毕，接下来开始查找,取各存在组之间的交集。
    std::set<Book> targets_exist;
    std::set<Book> targets_temp;
    bool flag = 0;
    if (index[1]) {
      flag = 1;
      Book target = ISBN_book.search(ISBN, blank_book);
      if (target.ISBN_ == ISBN) {
        targets_exist.insert(target);
      }
    }
    if (index[2]) {
      Book *possi = bookname_book.list(Book_name, blank_book, max_book);
      if (possi == nullptr) {
        flag = 1;
        targets_exist.clear();
      } else if (flag == 1) {
        for (int i = 0; possi[i] != blank_book; ++i) {
          if (targets_exist.count(possi[i])) {
            targets_temp.insert(possi[i]);
          }
        }
        targets_exist.clear();
        for (auto iter = targets_temp.begin(); iter != targets_temp.end();
             ++iter) {
          targets_exist.insert(*iter);
        }
      } else {
        flag = 1;
        for (int i = 0; possi[i] != blank_book; ++i) {
          targets_exist.insert(possi[i]);
        }
      }
      delete[] possi;
    }
    if (index[3]) {
      Book *possi = author_book.list(Author, blank_book, max_book);
      if (possi == nullptr) {
        flag = 1;
        targets_exist.clear();
      } else if (flag == 1) {
        for (int i = 0; possi[i] != blank_book; ++i) {
          if (targets_exist.count(possi[i])) {
            targets_temp.insert(possi[i]);
          }
        }
        targets_exist.clear();
        for (auto iter = targets_temp.begin(); iter != targets_temp.end();
             ++iter) {
          targets_exist.insert(*iter);
        }
      } else {
        flag = 1;
        for (int i = 0; possi[i] != blank_book; ++i) {
          targets_exist.insert(possi[i]);
        }
      }
      delete[] possi;
    }
    if (index[4]) {
      MyString *keys = Keywords.words_split('|');
      if (keys[1] != blank_string) {
        delete[] keys;
        std::cout << "Invalid.\n";
        return;
      }
      Book *possi = keywords_book.list(keys[0], blank_book, max_book);
      delete[] keys;
      if (possi == nullptr) {
        flag = 1;
        targets_exist.clear();
      } else if (flag == 1) {
        for (int i = 0; possi[i] != blank_book; ++i) {
          if (targets_exist.count(possi[i])) {
            targets_temp.insert(possi[i]);
          }
        }
        targets_exist.clear();
        for (auto iter = targets_temp.begin(); iter != targets_temp.end();
             ++iter) {
          targets_exist.insert(*iter);
        }
      } else {
        flag = 1;
        for (int i = 0; possi[i] != blank_book; ++i) {
          targets_exist.insert(possi[i]);
        }
      }
      delete[] possi;
    }
    if (targets_exist.empty()) {
      std::cout << '\n';
    }
    for (auto iter = targets_exist.begin(); iter != targets_exist.end();
         ++iter) {
      std::cout << (*iter).ISBN_ << '\t' << (*iter).Book_name_ << '\t'
                << (*iter).Author_ << '\t' << (*iter).Key_string << '\t'
                << std::fixed << std::setprecision(2) << (*iter).price << '\t'
                << (*iter).num_storage << '\n';
    }
  }
  return;
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
  Book target = ISBN_book.search(ISBN, blank);
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
  ISBN_book.erase(ISBN_book.create(ISBN, target));
  ISBN_book.insert(ISBN_book.create(ISBN, target));
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
    if ((totalcost[i] < '0' || totalcost[i] > '9') && totalcost[i] != '.') {
      std::cout << "Invalid\n";
      return;
    }
  }
  origin.num_storage += num_quantity;
  ISBN_book.erase(ISBN_book.create(origin.ISBN_, origin));
  ISBN_book.insert(ISBN_book.create(origin.ISBN_, origin));
  return;
}