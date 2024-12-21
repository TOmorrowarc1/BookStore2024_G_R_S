/*账户管理系统
此系统实现的功能如下：
1.定义Account_info类，储存用户ID，账户密码，用户名，用户权限等级。
2.实例化Memory<String,Account_info>，作为账户存储接口。
实现如下操作对应函数：
2.1
su [UserID] ([Password])?
读入su之后跳转到此函数，继续读入ID与密码。
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
register [UserID] [Password] [Username]
先访问，返回值若相同再压栈并改变全局变量。
*/
#ifndef A_C_C_O_U_N_T
#define A_C_C_O_U_N_T

#include<iostream>
#include<stack>
#include<cstring>
#include"storage.hpp"
#include"String.hpp"
#include"String.cpp"
#include"tokenscanner.hpp"
#include"tokenscanner.cpp"

namespace Account_system{

    class Account_info{
    private:
        MyString User_ID_;
        MyString Password_;
        MyString User_name_;
        int User_rank_;
    public:
        Account_info()=default;
        Account_info(const MyString&,const MyString&,const MyString&,int);
        void operator=(const Account_info&);
        //只比较Key,或者说User_ID大小(单Key)。
        bool operator==(const Account_info&)const;
        bool operator!=(const Account_info&)const;
        bool operator>(const Account_info&)const;
        bool operator<(const Account_info&)const;
        bool operator>=(const Account_info&)const;
        bool operator<=(const Account_info&)const;
        MyString user_id();
        friend void start();
        friend void Register(Token_scanner&);
        friend void User_add(Token_scanner&);
        friend void Password_change(Token_scanner&);
        friend void Delete_user(Token_scanner&);
        friend void sign_in(Token_scanner&);
        friend void log_out();
    };

    Memory<MyString, Account_info> Account_storage("account_key","account_value",1000);
    
    std::stack<Account_info> Account_record;
    
    int rank_now = 0;

    void start();
    void Register(Token_scanner& order);
    void User_add(Token_scanner& order);
    void Password_change(Token_scanner& order);
    void Delete_user(Token_scanner& order);
    void sign_in(Token_scanner& order);
    void log_out();


}

#endif
