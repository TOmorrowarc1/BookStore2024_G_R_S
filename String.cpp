#include"String.hpp"

String::String(std::string target) { std::strcpy(content, &target[0]); }
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
  return;
}
void String::operator=(const std::string& B){
  std::strcpy(content, &B[0]); 
  return;
}