#include"String.hpp"

MyString::MyString(const std::string& target) { 
  std::strcpy(content, &target[0]); 
}
bool MyString::operator>(const MyString &B)const {
  int result = std::strcmp((*this).content, B.content);
  return result > 0;
}
bool MyString::operator==(const MyString &B)const{
  int result = std::strcmp((*this).content, B.content);
  return result == 0;
}
bool MyString::operator!=(const MyString &B) const{
  int result = std::strcmp((*this).content, B.content);
  return result != 0;
}
bool MyString::operator>=(const MyString &B) const{
  int result = std::strcmp((*this).content, B.content);
  return result >= 0;
}
bool MyString::operator<(const MyString &B) const{
  int result = std::strcmp((*this).content, B.content);
  return result < 0;
}
bool MyString::operator<=(const MyString &B) const{
  int result = std::strcmp((*this).content, B.content);
  return result <= 0;
}
void MyString::operator=(const MyString &B) { 
  std::strcpy(content, B.content); 
  for(int i= strlen(content);i<65;++i){
    content[i]=0;
  }
  return;
}
void MyString::operator=(const std::string& B){
  std::strcpy(content, &B[0]); 
  for(int i= strlen(content);i<65;++i){
    content[i]=0;
  }
  return;
}

std::ostream& operator<<(std::ostream& output,const MyString& object){
  for(int i=0;object.content[i]!=0;++i){
    output<<object.content[i];
  }
  return output;
}
std::istream& operator>>(std::istream& input, MyString& object){
  std::string temp;
  input>>temp;
  object = temp;
  return input;
}