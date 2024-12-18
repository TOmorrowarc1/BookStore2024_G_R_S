#include"String.hpp"

class String;

String::String(const std::string& target) { 
  std::strcpy(content, &target[0]); 
}
bool String::operator>(const String &B)const {
  int result = std::strcmp((*this).content, B.content);
  return result > 0;
}
bool String::operator==(const String &B)const{
  int result = std::strcmp((*this).content, B.content);
  return result == 0;
}
bool String::operator!=(const String &B) const{
  int result = std::strcmp((*this).content, B.content);
  return result != 0;
}
bool String::operator>=(const String &B) const{
  int result = std::strcmp((*this).content, B.content);
  return result >= 0;
}
bool String::operator<(const String &B) const{
  int result = std::strcmp((*this).content, B.content);
  return result < 0;
}
bool String::operator<=(const String &B) const{
  int result = std::strcmp((*this).content, B.content);
  return result <= 0;
}
void String::operator=(const String &B) { 
  std::strcpy(content, B.content); 
  for(int i= strlen(content);i<65;++i){
    content[i]=0;
  }
  return;
}
void String::operator=(const std::string& B){
  std::strcpy(content, &B[0]); 
  for(int i= strlen(content);i<65;++i){
    content[i]=0;
  }
  return;
}