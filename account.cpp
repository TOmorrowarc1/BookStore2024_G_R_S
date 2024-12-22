/*账户管理系统
此系统实现的功能如下：
1.定义Account_info类，储存用户ID，账户密码，用户名，用户权限等级。
2.实例化Memory<String,Account_info>，作为账户存储接口。
实现如下操作对应函数：
2.1
register [UserID] [Password] [Username]
先访问，返回值若相同再压栈并改变全局变量。
2.2
passwd [UserID] ([CurrentPassword])? [NewPassword]
读入passwd之后跳转到此函数，继续读入ID与密码。
2.3
useradd [UserID] [Password] [Privilege] [Username]
读入useradd之后跳转到此函数，继续读入余下内容。
2.4
delete [UserID]
读入delete之后跳转到此函数。
3.定义账户栈，并实现对当前账户权限的修改(当前帐户权限属于全局变量)。
实现如下函数：
3.1
logout
弹栈，改变全局变量。
3.2
su [UserID] ([Password])?
读入su之后跳转到此函数，继续读入ID与密码。
*/

#include "account.hpp"
#include "String.cpp"
#include "book.hpp"
#include "tokenscanner.cpp"

Account_system::Account_info::Account_info(const MyString &user_ID,
                                           const MyString &user_name,
                                           const MyString &password,
                                           int user_rank) {
  User_ID_ = user_ID;
  User_name_ = user_name;
  Password_ = password;
  User_rank_ = user_rank;
}

void Account_system::Account_info::operator=(const Account_info &target) {
  User_ID_ = target.User_ID_;
  User_name_ = target.User_name_;
  Password_ = target.Password_;
  User_rank_ = target.User_rank_;
  return;
}
//作为单Key-Value元素，不可能在文件中出现两个相同的User_ID_。
bool Account_system::Account_info::operator==(const Account_info &B) const {
  return User_ID_ == B.User_ID_;
}
bool Account_system::Account_info::operator!=(const Account_info &B) const {
  return User_ID_ != B.User_ID_;
}
bool Account_system::Account_info::operator>(const Account_info &B) const {
  return User_ID_ > B.User_ID_;
}
bool Account_system::Account_info::operator<(const Account_info &B) const {
  return User_ID_ < B.User_ID_;
}
bool Account_system::Account_info::operator>=(const Account_info &B) const {
  return User_ID_ >= B.User_ID_;
}
bool Account_system::Account_info::operator<=(const Account_info &B) const {
  return User_ID_ <= B.User_ID_;
}
MyString Account_system::Account_info::user_id() { return User_ID_; }

void Account_system::start() {
  MyString root_ID("root");
  MyString Password("sjtu");
  Account_info root(root_ID, root_ID, Password, 7);
  Account_storage.insert(Account_storage.create(root_ID, root));
}
//下面是函数的正式实现：
/*{0} register [UserID] [Password] [Username]
注册信息如指令格式所示，权限等级为 {1} 的帐户。
如果 [UserID] 与已注册帐户重复则操作失败。*/
void Account_system::Register(Token_scanner &order) {
  if (order.count_string() != 3) {
    std::cout << "Invalid\n";
    return;
  }
  std::string token;
  MyString User_ID, User_name, Password;
  token = order.next_token();
  User_ID = token;
  token = order.next_token();
  Password = token;
  token = order.next_token();
  User_name = token;
  Account_info target(User_ID, User_name, Password, 1);
  Account_info temp = Account_storage.search(User_ID, target);
  if (temp != target) {
    Account_storage.insert(Account_storage.create(User_ID, target));
  } else {
    std::cout << "Invalid\n";
  }
  return;
}

/*{3} useradd [UserID] [Password] [Privilege] [Username]
创建信息如指令格式所示的帐户。
如果待创建帐户的权限等级大于等于当前帐户权限等级则操作失败；
如果 [UserID] 与已注册帐户重复则操作失败*/
void Account_system::User_add(Token_scanner &order) {
  if (order.count_string() != 4) {
    std::cout << "Invalid\n";
    return;
  }
  std::string token;
  MyString User_ID, User_name, Password, blank;
  int Privilege = 0;
  token = order.next_token();
  User_ID = token;
  token = order.next_token();
  Password = token;
  token = order.next_token();
  Privilege = token[0] - '0';
  token = order.next_token();
  User_name = token;
  if (rank_now > Privilege && rank_now >= 3 &&
      (Privilege == 3 || Privilege == 1 || Privilege == 0)) {
    Account_info target(User_ID, User_name, Password, Privilege);
    Account_info temp = Account_storage.search(User_ID, target);
    if (temp != target) {
      Account_storage.insert(Account_storage.create(User_ID, target));
    } else {
      std::cout << "Invalid\n";
    }
  } else {
    std::cout << "Invalid\n";
  }
  return;
}

/*{1} passwd [UserID] ([CurrentPassword])? [NewPassword]
修改指定帐户的密码。
如果该帐户不存在则操作失败；
如果密码错误则操作失败；
如果当前帐户权限等级为 {7} 则可以省略 [CurrentPassword]。*/
//尚未完成：{7}的特殊处理。
void Account_system::Password_change(Token_scanner &order) {
  std::string token;
  MyString User_ID, CurrentPassword, NewPassword, blank;
  if (rank_now == 7 &&
      (order.count_string() == 3 || order.count_string() == 2)) {
    if (order.count_string() == 2) {
      token = order.next_token();
      User_ID = token;
      token = order.next_token();
      NewPassword = token;
    } else {
      token = order.next_token();
      User_ID = token;
      token = order.next_token();
      CurrentPassword = token;
      token = order.next_token();
      NewPassword = token;
    }
  } else if (rank_now >= 1 && order.count_string() == 3) {
    token = order.next_token();
    User_ID = token;
    token = order.next_token();
    CurrentPassword = token;
    token = order.next_token();
    NewPassword = token;
  } else {
    std::cout << "Invalid\n";
    return;
  }
  Account_info target(User_ID, blank, CurrentPassword, 0);
  Account_info temp = Account_storage.search(User_ID, target);
  if ((rank_now != 7 || order.count_string() == 3) &&
      temp.Password_ != CurrentPassword) {
    std::cout << "Invalid\n";
    return;
  } else {
    temp.Password_ = NewPassword;
    Account_storage.erase(Account_storage.create(User_ID, temp));
    Account_storage.insert(Account_storage.create(User_ID, temp));
    return;
  }
}

/*{7} delete [UserID]
删除指定帐户。
如果待删除帐户不存在则操作失败；
如果待删除帐户已登录则操作失败。*/
void Account_system::Delete_user(Token_scanner &order) {
  if (order.count_string() != 1) {
    std::cout << "Invalid\n";
    return;
  }
  std::string token;
  MyString User_ID, blank;
  token = order.next_token();
  User_ID = token;
  if (rank_now == 7) {
    Account_info target(User_ID, blank, blank, 0);
    Account_info temp = Account_storage.search(User_ID, target);
    if (temp == target) {
      Account_storage.erase(Account_storage.create(User_ID, temp));
    } else {
      std::cout << "Invalid\n";
    }
  } else {
    std::cout << "Invalid\n";
  }
  return;
}

/*{0} su [UserID] ([Password])?
使登录帐户变为已登录状态，当前帐户变为该帐户。
如果该帐户不存在则操作失败；
如果密码错误则操作失败；
如果当前帐户权限等级高于登录帐户则可以省略密码。*/
void Account_system::sign_in(Token_scanner &order) {
  std::string token;
  MyString User_ID, Password, blank;
  if (order.count_string() == 2) {
    token = order.next_token();
    User_ID = token;
    token = order.next_token();
    Password = token;
  } else if (order.count_string() == 1) {
    token = order.next_token();
    User_ID = token;
  } else {
    std::cout << "Invalid\n";
    return;
  }
  Account_info target(User_ID, blank, blank, 0);
  Account_info temp = Account_storage.search(User_ID, target);
  if (temp.User_rank_ < rank_now) {
    Account_record.push(temp);
    rank_now = temp.User_rank_;
    //考虑如下问题：权限高但是输入密码？
  } else {
    if (temp.Password_ == Password) {
      Account_record.push(temp);
      rank_now = temp.User_rank_;
    } else {
      std::cout << "Invalid\n";
    }
  }
  return;
}

/*{1} logout
撤销最后一次成功执行的 su 指令效果。
如无已登录帐户则操作失败。*/
void Account_system::log_out() {
  if (!Account_record.empty()) {
    std::string user_id = Account_record.top().user_id().return_content();
    if(Book_manage::selection.find(user_id)!=Book_manage::selection.end()){
      Book_manage::selection.erase(user_id);
    }
    Account_record.pop();
    if (Account_record.empty()) {
      rank_now = 0;
    } else {
      rank_now = Account_record.top().User_rank_;
    }
  } else {
    std::cout << "Invalid\n";
  }
  return;
}