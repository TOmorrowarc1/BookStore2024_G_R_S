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

std::ostream& operator<<(std::ostream& output,const String& object){
  for(int i=0;object.content[i]!=0;++i){
    output<<object.content[i];
  }
  return output;
}
std::istream& operator>>(std::istream& input, String& object){
  std::string temp;
  input>>temp;
  object = temp;
  return input;
}