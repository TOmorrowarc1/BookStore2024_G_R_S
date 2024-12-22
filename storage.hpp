#pragma once
#ifndef STORAGE_HPP
#define STORAGE_HPP

#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>

template <typename Tk, typename Tv> class Memory;

// Memory,一个巨大的有序二维数组（头表示一部分元素的地址范围），储存key-value结构。
// 需要提前定义元素的序结构，并重载比较(const)与赋值运算符。
// 需要储存头的文件，头的结构，头的个数以及单个头的大小。
// 体作为数组存在，需要储存体的文件，单个体的大小，所有元素的总个数。
// Memory拥有创建新元素，寻找，添加（含裂块），减少（含并块），搜寻（单key单value），列举（单Key多value）数个函数。
template <typename Tk, typename Tv> class Memory {
private:
  class Atom;
  class Head;

  std::string head_file, body_file;
  std::fstream file;
  int num_of_heads;
  int num_of_atoms;
  const int size_of_atom = sizeof(Atom);
  const int size_of_head = sizeof(Head);
  const int size_of_body;

  class Atom {
    //内部含有Key_,Value_两个元素，作为体文件中基础的储存元素。
    //要求两个类重载所有比较(const)与赋值运算符,同时要求Tk,Tv构造为最小值。
  private:
    Tk Key_;
    Tv Value_;

  public:
    Atom() = default;
    Atom(const Tk &, const Tv &);
    bool operator>(const Atom &) const;
    bool operator==(const Atom &) const;
    bool operator<(const Atom &) const;
    bool operator>=(const Atom &) const;
    bool operator<=(const Atom &) const;
    bool operator!=(const Atom &) const;
    friend class Head;
    friend class Memory;
  };

  class Head {
  private:
    Atom max;
    Atom min;
    int begin = 0;
    int number = 0;

  public:
    Head();
    //在已知Atom属于该头所引领的体时，将Atom插入，改变对应的体以及自身（不裂块）。
    void insert(const std::string &, const Atom &, const Memory &);
    //在已知Atom属于该头所引领的体时，删除，改变对应的体以及自身（不裂块）。
    void erase(const std::string &, const Atom &, const Memory &);
    //在已知Atom属于该头所引领的体时，列举所有的Atom。
    Atom *list(const std::string &, const Atom &, const Atom &, const Memory &,
               const Atom *);
    //列举头中所有元素。
    Atom *all(const std::string &, const Memory &, const Atom *);
    //在已知Atom属于该头所引领的体时，寻找Key对应的Atom。
    Atom search(const std::string &, const Atom &, const Memory &);
    friend class Atom;
    friend class Memory;
  };

public:
  //构造函数：三个参数分别是头文件名，体文件名，单个体中元素数目。
  Memory(const std::string &, const std::string &, int);
  void initialise();
  //传递参数：储存元素。
  //寻找元素对应的头的位置。
  Atom create(const Tk &, const Tv &);
  int find(const Atom &, const Head *);
  //添加（含裂块）
  void insert(const Atom &);
  //减少（含并块）
  void erase(const Atom &);
  //传递参数：Key_。输出Value_到控制台。
  Tv *list(const Tk &Key_, const Tv &min, const Tv &max);
  //返回所有value值。
  Tv *all();
  //寻找对应Key的value(只应用于单Key单Value)，并直接返回value。
  Tv search(const Tk &Key_, const Tv &min);
  friend class Head;
};

//以下是Memory::Atom的实现。

template <typename Tk, typename Tv>
Memory<Tk, Tv>::Atom::Atom(const Tk &Key_1, const Tv &Value_1) {
  Key_ = const_cast<Tk &>(Key_1);
  Value_ = const_cast<Tv &>(Value_1);
}

//作比较，要求对应Tk类型,Tv类型重载比较运算符。
template <typename Tk, typename Tv>
bool Memory<Tk, Tv>::Atom::operator>(const Atom &B) const {
  const Atom &A = const_cast<Atom &>(*this);
  int compare = -1;
  if (A.Key_ == B.Key_) {
    compare = 0;
  } else if (A.Key_ > B.Key_) {
    compare = 1;
  }
  if (compare > 0) {
    return 1;
  } else if (compare < 0) {
    return 0;
  } else {
    if (A.Value_ > B.Value_) {
      return 1;
    } else {
      return 0;
    }
  }
}
template <typename Tk, typename Tv>
bool Memory<Tk, Tv>::Atom::operator==(const Atom &B) const {
  const Atom &A = (*this);
  return (A.Key_ == B.Key_ && A.Value_ == B.Value_);
}
template <typename Tk, typename Tv>
bool Memory<Tk, Tv>::Atom::operator<(const Atom &B) const {
  const Atom &A = (*this);
  return !(A > B || A == B);
}
template <typename Tk, typename Tv>
bool Memory<Tk, Tv>::Atom::operator>=(const Atom &B) const {
  const Atom &A = (*this);
  return (A > B || A == B);
}
template <typename Tk, typename Tv>
bool Memory<Tk, Tv>::Atom::operator<=(const Atom &B) const {
  const Atom &A = (*this);
  return (A < B || A == B);
}
template <typename Tk, typename Tv>
bool Memory<Tk, Tv>::Atom::operator!=(const Atom &B) const {
  const Atom &A = (*this);
  return !(A == B);
}

template <typename Tk, typename Tv> Memory<Tk, Tv>::Head::Head() {
  begin = 0;
  number = 0;
}
template <typename Tk, typename Tv>
//在已知Atom属于该头所引领的体时，将Atom插入，改变对应的体以及自身（不裂块）。
void Memory<Tk, Tv>::Head::insert(const std::string &file_name,
                                  const Atom &target,
                                  const Memory &environment) {
  Atom *temp = new Atom[number + 1];
  std::fstream file;
  file.open(file_name, std::fstream::in | std::fstream::out);
  if (!file) {
    std::cerr << "Cannot open the file.\n";
    return;
  }
  int start = begin * environment.size_of_atom;
  file.seekg(start);
  file.read(reinterpret_cast<char *>(temp), number * environment.size_of_atom);
  Atom *touch = std::lower_bound(temp, temp + number, target);
  if ((*touch) == target) {
    delete[] temp;
    file.close();
    return;
  }
  //先将Atom插入数组，后输入文件。
  else {
    for (int i = number; i > (touch - temp); --i) {
      temp[i] = temp[i - 1];
    }
    *touch = target;
    ++number;
    if (number == 1 || target < min) {
      min = target;
    }
    if (target > max) {
      max = target;
    }
    file.seekp(start);
    file.write(reinterpret_cast<char *>(temp),
               number * environment.size_of_atom);
    file.close();
    delete[] temp;
    return;
  }
}
//在已知Atom属于该头所引领的体时，删除，改变对应的体以及自身（不裂块,也不销毁空块）。
template <typename Tk, typename Tv>
void Memory<Tk, Tv>::Head::erase(const std::string &file_name,
                                 const Atom &target,
                                 const Memory &environment) {
  Atom *temp = new Atom[number + 1];
  std::fstream file;
  file.open(file_name, std::fstream::in | std::fstream::out);
  if (!file) {
    std::cerr << "Cannot open the file.\n";
    return;
  }
  int start = begin * environment.size_of_atom;
  file.seekg(start);
  file.read(reinterpret_cast<char *>(temp), number * environment.size_of_atom);
  Atom *touch = std::lower_bound(temp, temp + number, target);
  if (number == 0 || (*touch) != target) {
    delete[] temp;
    file.close();
    return;
  } else {
    --number;
    if (number == 0) {
      Atom blank;
      min = blank;
      max = blank;
    } else {
      if (touch == temp) {
        min = *(touch + 1);
      }
      if (touch == temp + number + 1) {
        max = *(touch - 1);
      }
    }
    for (int i = (touch - temp); i < number; ++i) {
      temp[i] = temp[i + 1];
    }
    Atom new_atom;
    temp[number] = new_atom;
    //写入整个数组。
    file.seekp(start);
    file.write(reinterpret_cast<char *>(temp),
               (number + 1) * environment.size_of_atom);
    delete[] temp;
    file.close();
    return;
  }
}
//在已知Atom属于该头所引领的体时，列举，不改变对应的体以及自身。
template <typename Tk, typename Tv>
typename Memory<Tk, Tv>::Atom *
Memory<Tk, Tv>::Head::list(const std::string &file_name, const Atom &front,
                           const Atom &back, const Memory &environment,
                           const Atom *place) {
  Atom *temp = new Atom[number + 1];
  Atom *platform = const_cast<Atom *>(place);
  std::fstream file;
  file.open(file_name, std::fstream::in | std::fstream::out);
  if (!file) {
    std::cerr << "Cannot open the file.\n";
    return platform;
  }
  int start = begin * environment.size_of_atom;
  file.seekg(start);
  file.read(reinterpret_cast<char *>(temp), number * environment.size_of_atom);
  Atom *a1 = std::lower_bound(temp, temp + number, front);
  Atom *a2 = std::upper_bound(temp, temp + number, back);
  for (auto iter = a1; iter < a2; ++iter) {
    (*platform) = (*iter);
    ++platform;
  }
  file.close();
  delete[] temp;
  return platform;
}

template <typename Tk, typename Tv>
typename Memory<Tk, Tv>::Atom *
Memory<Tk, Tv>::Head::all(const std::string &file_name,
                          const Memory &environment, const Atom *place) {
  Atom *temp = new Atom[number + 1];
  Atom *platform = const_cast<Atom *>(place);
  std::fstream file;
  file.open(file_name, std::fstream::in | std::fstream::out);
  if (!file) {
    std::cerr << "Cannot open the file.\n";
    return platform;
  }
  int start = begin * environment.size_of_atom;
  file.seekg(start);
  file.read(reinterpret_cast<char *>(temp), number * environment.size_of_atom);
  for (int i = 0; i < number; ++i) {
    (*platform) = temp[i];
    ++platform;
  }
  file.close();
  delete[] temp;
  return platform;
}

template <typename Tk, typename Tv>
typename Memory<Tk, Tv>::Atom
Memory<Tk, Tv>::Head::search(const std::string &file_name, const Atom &Key_min,
                             const Memory &environment) {
  Atom *temp = new Atom[number + 1];
  std::fstream file;
  file.open(file_name, std::fstream::in | std::fstream::out);
  if (!file) {
    std::cerr << "Cannot open the file.\n";
    return *temp;
  }
  int start = begin * environment.size_of_atom;
  file.seekg(start);
  file.read(reinterpret_cast<char *>(temp), number * environment.size_of_atom);
  Atom *target = std::lower_bound(temp, temp + number, Key_min);
  Atom target_catch;
  if (target->Key_ == Key_min.Key_) {
    target_catch = (*target);
  }
  file.close();
  delete[] temp;
  return target_catch;
}

template <typename Tk, typename Tv>
Memory<Tk, Tv>::Memory(const std::string &file1, const std::string &file2,
                       int size_of_body1)
    : size_of_body(size_of_body1) {
  head_file = file1;
  body_file = file2;
  num_of_heads = 0;
  num_of_atoms = 0;
  file.open(head_file, std::fstream::in | std::fstream::ate);
  if (!file) {
    file.close();
    file.open(head_file, std::fstream::out);
    file.close();
  } else {
    num_of_heads = file.tellp() / size_of_head;
    file.close();
  }
  file.open(body_file, std::fstream::in | std::fstream::ate);
  if (!file) {
    file.close();
    file.open(body_file, std::fstream::out);
    file.close();
  } else {
    num_of_atoms = num_of_heads * (size_of_body + 1);
    file.close();
  }
  if (num_of_heads == 0) {
    initialise();
  }
}
template <typename Tk, typename Tv> void Memory<Tk, Tv>::initialise() {
  file.open(head_file, std::fstream::in | std::fstream::out);
  Head new_head;
  file.write(reinterpret_cast<char *>(&new_head), size_of_head);
  file.close();
  ++num_of_heads;
  num_of_atoms += size_of_body + 1;
}
//根据传入参数构造Atom.
template <typename Tk, typename Tv>
typename Memory<Tk, Tv>::Atom Memory<Tk, Tv>::create(const Tk &Key_,
                                                     const Tv &Value_) {
  Memory<Tk, Tv>::Atom target(Key_, Value_);
  return target;
}
//传递参数：储存元素。
//寻找元素对应的头的位置。
template <typename Tk, typename Tv>
int Memory<Tk, Tv>::find(const Atom &target, const Head *temp) {
  int result = 0;
  if (temp[0].min > target && temp[0].number != 0) {
    result = 0;
  } else {
    bool flag = 1;
    for (int i = 0; i < num_of_heads - 1; ++i) {
      //夹在块内。
      if (temp[i].max >= target && temp[i].min <= target) {
        result = i;
        flag = 0;
        break;
      }
      //夹在两块之间。
      if (temp[i].max < target && temp[i + 1].min > target) {
        result = i;
        flag = 0;
        break;
      }
      //大于最后的最大值。
      //涉及到0的问题--销毁空块。
      if (temp[i + 1].number == 0) {
        result = i;
        flag = 0;
        break;
      }
    }
    if (flag == 1) {
      result = num_of_heads - 1;
    }
  }
  return result;
}

//添加（含裂块）
template <typename Tk, typename Tv>
void Memory<Tk, Tv>::insert(const Atom &target) {
  Head *temp = new Head[num_of_heads + 1];
  file.open(head_file, std::fstream::in | std::fstream::out);
  if (!file) {
    std::cerr << "Cannot open the file.\n";
    return;
  }
  file.seekg(0);
  file.read(reinterpret_cast<char *>(temp), num_of_heads * size_of_head);
  file.close();
  int place = find(target, temp);
  temp[place].insert(body_file, target, *this);
  //裂块。
  if (temp[place].number > size_of_body) {
    ++num_of_heads;
    //处理新块。
    Atom *new_block = new Atom[size_of_body + 1];
    file.open(body_file, std::fstream::in | std::fstream::out);
    file.seekg((temp[place].begin + size_of_body / 2) * size_of_atom);
    file.read(reinterpret_cast<char *>(new_block), size_of_atom);
    file.seekg((temp[place].begin + size_of_body / 2 + 1) * size_of_atom);
    file.read(reinterpret_cast<char *>(new_block + size_of_body / 2 + 1),
              size_of_body / 2 * size_of_atom);
    file.seekp((temp[place].begin + size_of_body / 2 + 1) * size_of_atom);
    file.write(reinterpret_cast<char *>(new_block + 1),
               size_of_body / 2 * size_of_atom);
    file.seekp(num_of_atoms * size_of_atom);
    file.write(reinterpret_cast<char *>(new_block + size_of_body / 2 + 1),
               size_of_body / 2 * size_of_atom);
    file.close();
    Head new_head;
    new_head.max = temp[place].max;
    new_head.begin = num_of_atoms;
    new_head.number = size_of_body / 2;
    new_head.min = new_block[size_of_body / 2 + 1];
    temp[place].max = new_block[0];
    temp[place].number = size_of_body / 2 + 1;
    // temp数组加载。
    delete[] new_block;
    for (int i = num_of_heads - 1; i > place + 1; --i) {
      temp[i] = temp[i - 1];
    }
    temp[place + 1] = new_head;
    num_of_atoms += size_of_body + 1;
  }
  file.open(head_file, std::fstream::in | std::fstream::out);
  file.seekp(0);
  file.write(reinterpret_cast<char *>(temp), num_of_heads * size_of_head);
  file.close();
  delete[] temp;
  return;
}

//减少（含并块）
template <typename Tk, typename Tv>
void Memory<Tk, Tv>::erase(const Atom &target) {
  Head *temp = new Head[num_of_heads + 1];
  file.open(head_file, std::fstream::in | std::fstream::out);
  if (!file) {
    std::cerr << "Cannot open the file.\n";
    return;
  }
  file.seekg(0);
  file.read(reinterpret_cast<char *>(temp), num_of_heads * size_of_head);
  file.close();
  int place = find(target, temp);
  temp[place].erase(body_file, target, *this);
  //销毁空块。
  if (temp[place].number == 0 && place != 0) {
    for (int i = place; i < num_of_heads; ++i) {
      temp[i] = temp[i + 1];
    }
    --num_of_heads;
  }
  //并块。
  else if (temp[place].number < size_of_body / 2 &&
           temp[place + 1].number < size_of_body / 2 &&
           temp[place + 1].number != 0) {
    Atom *new_block = new Atom[size_of_body + 1];
    file.open(body_file, std::fstream::in | std::fstream::out);
    file.seekg(temp[place + 1].begin * size_of_atom);
    file.read(reinterpret_cast<char *>(new_block),
              temp[place + 1].number * size_of_atom);
    file.seekp((temp[place].begin + temp[place].number) * size_of_atom);
    file.write(reinterpret_cast<char *>(new_block),
               temp[place + 1].number * size_of_atom);
    file.close();
    delete[] new_block;
    temp[place].number += temp[place + 1].number;
    temp[place].max = temp[place + 1].max;
    for (int i = place + 1; i < num_of_heads; ++i) {
      temp[i] = temp[i + 1];
    }
    --num_of_heads;
  }
  file.open(head_file, std::fstream::in | std::fstream::out);
  file.seekp(0);
  file.write(reinterpret_cast<char *>(temp), (num_of_heads + 1) * size_of_head);
  file.close();
  delete[] temp;
  return;
}

//传递参数：Key_。输出Value_。
template <typename Tk, typename Tv>
Tv *Memory<Tk, Tv>::list(const Tk &Key_, const Tv &min, const Tv &max) {
  std::set<Tv> storage;
  Atom front(Key_, min);
  Atom end(Key_, max);
  Atom blank;
  Head *temp = new Head[num_of_heads + 1];
  file.open(head_file, std::fstream::in | std::fstream::out);
  if (!file) {
    std::cerr << "Cannot open the file.\n";
    return nullptr;
  }
  file.seekg(0);
  file.read(reinterpret_cast<char *>(temp), num_of_heads * size_of_head);
  file.close();
  int start = find(front, temp);
  int finish = find(end, temp);
  for (int i = start; i <= finish; ++i) {
    Atom *place = new Atom[size_of_body];
    Atom *flag = temp[i].list(body_file, front, end, *this, place);
    for (auto iter = place; iter != flag; ++iter) {
      storage.insert((*iter).Value_);
    }
    delete[] place;
  }
  delete[] temp;
  if (storage.empty()) {
    return nullptr;
  } else {
    Tv *storage_ = new Tv[storage.size()];
    int pointer = 0;
    for (auto iter = storage.begin(); iter != storage.end(); ++iter) {
      storage_[pointer] = (*iter);
      ++pointer;
    }
    //记得留空白位。
    storage_[pointer]=blank.Value_;
    return storage_;
  }
}

template <typename Tk, typename Tv> Tv *Memory<Tk, Tv>::all() {
  std::set<Tv> storage;
  Atom blank;
  Head *temp = new Head[num_of_heads + 1];
  file.open(head_file, std::fstream::in | std::fstream::out);
  if (!file) {
    std::cerr << "Cannot open the file.\n";
    return nullptr;
  }
  file.seekg(0);
  file.read(reinterpret_cast<char *>(temp), num_of_heads * size_of_head);
  file.close();
  for (int i = 0; i < num_of_heads; ++i) {
    Atom *place = new Atom[size_of_body];
    Atom *flag = temp[i].all(body_file, *this, place);
    for (auto iter = place; iter != flag; ++iter) {
      storage.insert((*iter).Value_);
    }
    delete[] place;
  }
  delete[] temp;
  Tv *storage_ = new Tv[storage.size() + 1];
  int pointer = 0;
  for (auto iter = storage.begin(); iter != storage.end(); ++iter) {
    storage_[pointer] = (*iter);
    ++pointer;
  }
  storage_[pointer]=blank.Value_;
  return storage_;
}

template <typename Tk, typename Tv>
Tv Memory<Tk, Tv>::search(const Tk &Key_, const Tv &min) {
  Atom key_min(Key_, min);
  Atom target_catcher, blank;
  Head *temp = new Head[num_of_heads + 1];
  file.open(head_file, std::fstream::in | std::fstream::out);
  if (!file) {
    std::cerr << "Cannot open the file.\n";
    return blank.Value_;
  }
  file.seekg(0);
  file.read(reinterpret_cast<char *>(temp), num_of_heads * size_of_head);
  file.close();
  int start = find(key_min, temp);
  target_catcher = temp[start].search(body_file, key_min, *this);
  if (target_catcher == blank) {
    start += 1;
    target_catcher = temp[start].search(body_file, key_min, *this);
  }
  delete[] temp;
  return target_catcher.Value_;
}

#endif