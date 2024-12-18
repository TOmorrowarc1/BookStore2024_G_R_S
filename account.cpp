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

#include"account.hpp"

Account_system::Account_info::Account_info(const String& user_ID,const String& user_name,const String& password,int user_rank){
    User_ID_=user_ID;
    User_name_=user_name;
    Password_=password;
    User_rank_=user_rank;
}

void Account_system::Account_info::operator=(const Account_info& target){
    User_ID_=target.User_ID_;
    User_name_=target.User_name_;
    Password_=target.Password_;
    User_rank_=target.User_rank_;
    return;
}
//作为单Key-Value元素，不可能在文件中出现两个相同的User_ID_。
bool Account_system::Account_info::operator==(const Account_info& B)const{
    return User_ID_==B.User_ID_;
}
bool Account_system::Account_info::operator!=(const Account_info& B)const{
    return User_ID_!=B.User_ID_;
}
bool Account_system::Account_info::operator>(const Account_info& B)const{
    return User_ID_>B.User_ID_;
}
bool Account_system::Account_info::operator<(const Account_info& B)const{
    return User_ID_<B.User_ID_;
}
bool Account_system::Account_info::operator>=(const Account_info& B)const{
    return User_ID_>=B.User_ID_;
}
bool Account_system::Account_info::operator<=(const Account_info& B)const{
    return User_ID_<=B.User_ID_;
}


//下面是函数的正式实现：
/*{0} register [UserID] [Password] [Username]
注册信息如指令格式所示，权限等级为 {1} 的帐户。
如果 [UserID] 与已注册帐户重复则操作失败。*/
void Account_system::Register(){
    String User_ID,Password,User_name;
    std::cin>>User_ID>>Password>>User_name;
    Account_info target(User_ID,User_name,Password,1);
    Account_info temp=Account_storage.search(User_ID,target);
    if(temp!=target){
        Account_storage.insert(Account_storage.create(User_ID,target));
    }
    return;
}
/*{3} useradd [UserID] [Password] [Privilege] [Username]
创建信息如指令格式所示的帐户。
如果待创建帐户的权限等级大于等于当前帐户权限等级则操作失败；
如果 [UserID] 与已注册帐户重复则操作失败*/
void Account_system::User_add(){
    String User_ID,User_name,Password,blank;
    int Privilege=0;
    std::cin>>User_ID>>User_name>>Privilege>>Password;
    if(rank_now>Privilege&&rank_now>=3){
        Account_info target(User_ID,User_name,Password,Privilege);
        Account_info temp=Account_storage.search(User_ID,target);
        if(temp!=target){
            Account_storage.insert(Account_storage.create(User_ID,target));
        }
    }
    return;
}
/*{1} passwd [UserID] ([CurrentPassword])? [NewPassword]
修改指定帐户的密码。
如果该帐户不存在则操作失败；
如果密码错误则操作失败；
如果当前帐户权限等级为 {7} 则可以省略 [CurrentPassword]。*/
void Account_system::Password_change(){
    String User_ID,CurrentPassword,NewPassword,blank;
    std::cin>>User_ID;
    if(rank_now==7){
        std::cin>>CurrentPassword;
        std::cin>>NewPassword;
    }
    else{
        std::cin>>CurrentPassword;
        std::cin>>NewPassword;
    }
    Account_info target(User_ID,blank,CurrentPassword,0);
    Account_info temp=Account_storage.search(User_ID,target);
    if(temp.Password_!=CurrentPassword){
        return;
    }
    else{
        temp.Password_=NewPassword;
        Account_storage.insert(Account_storage.create(User_ID,temp));
        return;
    }
}
/*{7} delete [UserID]
删除指定帐户。
如果待删除帐户不存在则操作失败；
如果待删除帐户已登录则操作失败。*/
void Account_system::Delete_user(){
    String user_id,blank;
    std::cin>>user_id;
    if(rank_now==7){
        Account_info target(user_id,blank,blank,0);
        Account_info temp=Account_storage.search(user_id,target);
        if(temp==target){
            Account_storage.erase(Account_storage.create(user_id,temp));
        }
    }
    return;
}