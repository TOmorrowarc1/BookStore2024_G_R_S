#include "String.hpp"

MyString::MyString(const std::string &target) {
  std::strcpy(content, &target[0]);
}
std::string MyString::return_content() {
  std::string result;
  std::strcpy(&result[0], content);
  return result;
}
bool MyString::operator>(const MyString &B) const {
  int result = std::strcmp((*this).content, B.content);
  return result > 0;
}
bool MyString::operator==(const MyString &B) const {
  int result = std::strcmp((*this).content, B.content);
  return result == 0;
}
bool MyString::operator!=(const MyString &B) const {
  int result = std::strcmp((*this).content, B.content);
  return result != 0;
}
bool MyString::operator>=(const MyString &B) const {
  int result = std::strcmp((*this).content, B.content);
  return result >= 0;
}
bool MyString::operator<(const MyString &B) const {
  int result = std::strcmp((*this).content, B.content);
  return result < 0;
}
bool MyString::operator<=(const MyString &B) const {
  int result = std::strcmp((*this).content, B.content);
  return result <= 0;
}
void MyString::operator=(const MyString &B) {
  std::strcpy(content, B.content);
  for (int i = strlen(content); i < 65; ++i) {
    content[i] = 0;
  }
  return;
}
void MyString::operator=(const std::string &B) {
  std::strcpy(content, &B[0]);
  for (int i = strlen(content); i < 65; ++i) {
    content[i] = 0;
  }
  return;
}

MyString *MyString::words_split(char signal) {
  int sign_place[10] = {0};
  char content_fix[65] = {0};
  int flag = 1;
  for (int i = 0; content[i] != 0; ++i) {
    content_fix[i] = content[i];
  }
  for (int i = 0; content[i] != 0; ++i) {
    if (content_fix[i] == signal) {
      sign_place[flag] = i + 1;
      ++flag;
      content_fix[i] = 0;
    }
  }
  --flag;
  MyString *address = new MyString[flag + 1];
  for (int i = 0; i < flag; ++i) {
    std::string target;
    target = &content_fix[sign_place[i]];
    address[i] = target;
  }
  return address;
}

std::ostream &operator<<(std::ostream &output, const MyString &object) {
  for (int i = 0; object.content[i] != 0; ++i) {
    output << object.content[i];
  }
  return output;
}
std::istream &operator>>(std::istream &input, MyString &object) {
  std::string temp;
  input >> temp;
  object = temp;
  return input;
}