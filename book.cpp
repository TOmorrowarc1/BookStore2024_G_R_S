#include "book.hpp"
#include "diary.cpp"
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
    std::cout << "Invalid\n";
    return;
  }
  MyString ISBN = order.next_token();
  Book blank, target;
  target = ISBN_book.search(ISBN, blank);
  Account_record temp = Account_selection.top();
  if (target.ISBN_ != ISBN) {
    blank.ISBN_ = ISBN;
    ISBN_book.insert(ISBN_book.create(ISBN, blank));
    temp.selection_now = blank;
  } else {
    temp.selection_now = target;
  }
  Account_selection.pop();
  Account_selection.push(temp);
  return;
}

// 2.修改图书信息
void Book_manage::modify(Token_scanner &order) {
  bool change[6] = {0};
  MyString ISBN, Book_name, Author, Keywords, blank_string;
  MyString max_string("~~~~~~~~~");
  MyString isbn("-ISBN=");
  MyString name("-name=");
  MyString author("-author=");
  MyString keywords("-keyword=");
  MyString price("-price=");
  Book blank;
  double Price = 0;
  if (order.count_string() == 0 || Account_system::rank_now < 3) {
    std::cout << "Invalid\n";
    return;
  }
  //读取对应信息方法：锁定=.
  int total_string = order.count_string();
  for (int i = 0; i < total_string; ++i) {
    MyString token = order.next_token();
    int equal = 1, end = 0;
    for (int i = 1; token[i - 1] != '='; ++i) {
      ++equal;
    }
    MyString type_string(token, 0, equal);
    end = equal;
    while (token[end] != 0) {
      ++end;
    }
    MyString message(token, equal, end);
    if (type_string == isbn) {
      if (change[1] == 1) {
        std::cout << "Invalid\n";
        return;
      }
      change[1] = 1;
      ISBN = message;
    } else if (type_string == name) {
      if (change[2] == 1) {
        std::cout << "Invalid.\n";
        return;
      }
      change[2] = 1;
      Book_name = message;
      Book_name.delete_quote();
    } else if (type_string == author) {
      if (change[3] == 1) {
        std::cout << "Invalid\n";
        return;
      }
      change[3] = 1;
      Author = message;
      Author.delete_quote();
    } else if (type_string == keywords) {
      if (change[4] == 1) {
        std::cout << "Invalid\n";
        return;
      }
      change[4] = 1;
      Keywords = message;
      Keywords.delete_quote();
    } else if (type_string == price) {
      if (change[5] == 1) {
        std::cout << "Invalid\n";
        return;
      }
      change[5] = 1;
      Price = std::atof(&message[0]);
    } else {
      std::cout << "Invalid\n";
      return;
    }
  }
  //以上为输入部分，接下来修改对应文件。
  //找出原来书籍信息。
  Account_record temp = Account_selection.top();
  Book origin = temp.selection_now;
  if (origin == blank) {
    std::cout << "Invalid\n";
    return;
  }
  Book now(origin);
  //根据输入内容修改本书信息。
  if (change[1]) {
    if (ISBN == origin.ISBN_) {
      std::cout << "Invalid\n";
      return;
    }
    now.ISBN_ = ISBN;
    Book judge = ISBN_book.search(ISBN, blank);
    if (judge.ISBN_ == ISBN) {
      std::cout << "Invalid\n";
      return;
    }
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
          std::cout << "Invalid\n";
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
  //检查栈中是否有用户选择相同书籍并更改。
  std::stack<Account_record> account_holder;
  while (!Account_selection.empty()) {
    Account_record check = Account_selection.top();
    if (check.selection_now == origin) {
      check.selection_now = now;
    }
    account_holder.push(check);
    Account_selection.pop();
  }
  while (!account_holder.empty()) {
    Account_selection.push(account_holder.top());
    account_holder.pop();
  }
  return;
}

void Book_manage::show(Token_scanner &order) {
  MyString ISBN, Book_name, Author, Keywords, blank_string;
  MyString max_string("~~~~~~~~~");
  MyString isbn("-ISBN=");
  MyString name("-name=");
  MyString author("-author=");
  MyString keywords("-keyword=");
  MyString price("-price=");
  double Price = 0;
  if (Account_system::rank_now == 0) {
    std::cout << "Invalid\n";
    return;
  }
  if (order.count_string() == 0) {
    Book blank;
    Book *all = ISBN_book.all();
    for (int i = 0; all[i] != blank; ++i) {
      std::cout << all[i].ISBN_ << '\t' << all[i].Book_name_ << '\t'
                << all[i].Author_ << '\t' << all[i].Key_string << '\t'
                << std::fixed << std::setprecision(2) << all[i].price << '\t'
                << all[i].num_storage << '\n';
    }
    delete[] all;
    return;
  }
  if (order.count_string() != 1) {
    std::cout << "Invalid\n";
    return;
  }
  //读取对应信息方法：锁定=.
  MyString token = order.next_token();
  int equal = 1, end = 0;
  for (int i = 1; token[i - 1] != '='; ++i) {
    ++equal;
  }
  MyString type_string(token, 0, equal);
  end = equal;
  while (token[end] != 0) {
    ++end;
  }
  MyString message(token, equal, end);
  if (type_string == isbn) {
    ISBN = message;
  } else if (type_string == name) {
    Book_name = message;
    Book_name.delete_quote();
  } else if (type_string == author) {
    Author = message;
    Author.delete_quote();
  } else if (type_string == keywords) {
    Keywords = message;
    Keywords.delete_quote();
  } else {
    std::cout << "Invalid\n";
    return;
  }
  //信息读取完毕，接下来开始查找,取各存在组之间的交集。
  Book blank_book;
  Book *possi = nullptr;
  Book max_book(max_string, blank_string, blank_string, blank_string, 0, 0);
  if (type_string == isbn) {
    Book target = ISBN_book.search(ISBN, blank_book);
    if (target.ISBN_ != ISBN) {
      std::cout << '\n';
      return;
    } else {
      std::cout << target.ISBN_ << '\t' << target.Book_name_ << '\t'
                << target.Author_ << '\t' << target.Key_string << '\t'
                << std::fixed << std::setprecision(2) << target.price << '\t'
                << target.num_storage << '\n';
      return;
    }
  } else if (type_string == name) {
    possi = bookname_book.list(Book_name, blank_book, max_book);
  } else if (type_string == author) {
    possi = author_book.list(Author, blank_book, max_book);
  } else if (type_string == keywords) {
    MyString *keys = Keywords.words_split('|');
    if (keys[1] != blank_string) {
      delete[] keys;
      std::cout << "Invalid\n";
      return;
    }
    possi = keywords_book.list(keys[0], blank_book, max_book);
    delete[] keys;
  }
  if (possi == nullptr) {
    std::cout << '\n';
  } else {
    for (int i = 0; possi[i] != blank_book; ++i) {
      std::cout << possi[i].ISBN_ << '\t' << possi[i].Book_name_ << '\t'
                << possi[i].Author_ << '\t' << possi[i].Key_string << '\t'
                << std::fixed << std::setprecision(2) << possi[i].price << '\t'
                << possi[i].num_storage << '\n';
    }
  }
  delete[] possi;
  return;
}

void Book_manage::sell(Token_scanner &order) {
  if (order.count_string() != 2 || Account_system::rank_now < 1) {
    std::cout << "Invalid\n";
    return;
  }
  MyString ISBN = order.next_token();
  MyString number = order.next_token();
  Book blank;
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
  if (num > target.num_storage || num == 0) {
    std::cout << "Invalid\n";
    return;
  }
  std::cout << std::fixed << std::setprecision(2) << (num * target.price)
            << '\n';
  target.num_storage -= num;
  MyString blank_string;
  ISBN_book.erase(ISBN_book.create(ISBN, target));
  ISBN_book.insert(ISBN_book.create(ISBN, target));
  bookname_book.erase(bookname_book.create(target.Book_name_, target));
  bookname_book.insert(bookname_book.create(target.Book_name_, target));
  author_book.erase(author_book.create(target.Author_, target));
  author_book.insert(author_book.create(target.Author_, target));
  MyString *catcher = target.Key_string.words_split('|');
  for (int i = 0; catcher[i] != blank_string; ++i) {
    keywords_book.erase(keywords_book.create(catcher[i], target));
    keywords_book.insert(keywords_book.create(catcher[i], target));
  }
  delete[] catcher;
  //检测栈中是否存在相同书籍并更改。
  std::stack<Account_record> account_holder;
  while (!Account_selection.empty()) {
    Account_record check = Account_selection.top();
    if (check.selection_now == target) {
      check.selection_now = target;
    }
    account_holder.push(check);
    Account_selection.pop();
  }
  while (!account_holder.empty()) {
    Account_selection.push(account_holder.top());
    account_holder.pop();
  }
  //日志系统：储存增量。
  Diary_system::Trade new_trade(Diary_system::count, num * target.price,
                                blank_string, target.ISBN_, -num);
  Diary_system::Diary_storage.insert(
      Diary_system::Diary_storage.create(0, new_trade));
  ++Diary_system::count;
  return;
}

void Book_manage::import(Token_scanner &order) {
  if (order.count_string() != 2 || Account_system::rank_now < 3) {
    std::cout << "Invalid\n";
    return;
  }
  Book blank;
  Account_record temp = Account_selection.top();
  Book target = temp.selection_now;
  //找出原来书籍信息。
  if (target == blank) {
    std::cout << "Invalid\n";
    return;
  }
  MyString quantity, totalcost;
  int num_quantity = 0;
  double cost = 0;
  quantity = order.next_token();
  totalcost = order.next_token();
  for (int i = 0; quantity[i] != 0; ++i) {
    if (quantity[i] < '0' || quantity[i] > '9') {
      std::cout << "Invalid\n";
      return;
    }
  }
  num_quantity = std::atoi(&quantity[0]);
  if (num_quantity == 0) {
    std::cout << "Invalid\n";
    return;
  }
  for (int i = 0; totalcost[i] != 0; ++i) {
    if ((totalcost[i] < '0' || totalcost[i] > '9') && totalcost[i] != '.') {
      std::cout << "Invalid\n";
      return;
    }
  }
  cost = std::atof(&totalcost[0]);
  if (cost == 0) {
    std::cout << "Invalid\n";
    return;
  }
  target.num_storage += num_quantity;
  MyString blank_string;
  ISBN_book.erase(ISBN_book.create(target.ISBN_, target));
  ISBN_book.insert(ISBN_book.create(target.ISBN_, target));
  bookname_book.erase(bookname_book.create(target.Book_name_, target));
  bookname_book.insert(bookname_book.create(target.Book_name_, target));
  author_book.erase(author_book.create(target.Author_, target));
  author_book.insert(author_book.create(target.Author_, target));
  MyString *catcher = target.Key_string.words_split('|');
  for (int i = 0; catcher[i] != blank_string; ++i) {
    keywords_book.erase(keywords_book.create(catcher[i], target));
    keywords_book.insert(keywords_book.create(catcher[i], target));
  }
  delete[] catcher;
  //检测栈中是否存在相同书籍并更改。
  std::stack<Account_record> account_holder;
  while (!Account_selection.empty()) {
    Account_record check = Account_selection.top();
    if (check.selection_now == target) {
      check.selection_now = target;
    }
    account_holder.push(check);
    Account_selection.pop();
  }
  while (!account_holder.empty()) {
    Account_selection.push(account_holder.top());
    account_holder.pop();
  }
  //日志系统：储存增量。
  Diary_system::Trade new_trade(Diary_system::count, -cost, blank_string,
                                target.ISBN_, num_quantity);
  Diary_system::Diary_storage.insert(
      Diary_system::Diary_storage.create(0, new_trade));
  ++Diary_system::count;
  return;
}