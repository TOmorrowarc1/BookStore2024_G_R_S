#include<iostream>
#include<fstream>
#include<string>
#include<cstring>
#include<algorithm>

class Memory;
class Atom;

class Atom {
    //当做结构体用，只是为了重载对应运算符用的class。
private:
    char key[65] = { 0 };
    int value = 0;
public:
    Atom() = default;
    Atom(const std::string&, int);
    bool operator>(const Atom&) const;
    bool operator== (const Atom&) const;
    bool operator< (const Atom&) const;
    bool operator>= (const Atom&) const;
    bool operator<= (const Atom&) const;
    bool operator!=(const Atom&) const;
    friend class Head;
    friend class Memory;
};
//初始化：对最小值进行约定。
const Atom blank_min = {
    "\0" ,-1
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
    void insert(const std::string&, const Atom&, const Memory&);
    //在已知Atom属于该头所引领的体时，删除，改变对应的体以及自身（不裂块）。
    void erase(const std::string&, const Atom&, const Memory&);
    //在已知Atom属于该头所引领的体时，列举，改变对应的体以及自身（不裂块）。
    bool list(const std::string&, const Atom&, const Atom&, const Memory&);
    friend class Atom;
    friend class Memory;
};

//Memory,一个巨大的？？？（头表示一部分元素的地址范围）,需要提前定义元素的序结构。
// 需要储存头的文件，头的结构，头的个数以及单个头的大小。
// 体作为数组存在，需要储存体的文件，单个体的大小，所有元素的总个数。
//Memory拥有寻找，添加（含裂块），减少（含并块），寻找（锁定同一个key）四个函数。
class Memory {
private:
    std::string head_file, body_file;
    std::fstream file;
    int num_of_heads;
    int num_of_atoms;
    const int size_of_atom = sizeof(Atom);
    const int size_of_head = sizeof(Head);
    const int size_of_body;
public:
    //构造函数：三个参数分别是头文件名，体文件名，单个体中元素数目。
    Memory(const std::string&, const std::string&, int);
    void initialise();
    //传递参数：储存元素。
    //寻找元素对应的头的位置。
    int find(const Atom&, const Head*);
    //添加（含裂块）
    void insert(const Atom&);
    //减少（含并块）
    void erase(const Atom&);
    //传递参数：key。输出value。
    void list(const std::string&);
    friend class Head;
};


Atom::Atom(const std::string& key1, int value1) {
    std::strcpy(key, &key1[0]);
    value = value1;
}

bool Atom::operator>(const Atom& B) const {
    const Atom& A = (*this);
    int str_cmp = std::strcmp(A.key, B.key);
    if (str_cmp > 0) {
        return 1;
    }
    else if (str_cmp < 0) {
        return 0;
    }
    else {
        if (A.value > B.value) {
            return 1;
        }
        else {
            return 0;
        }
    }
}
bool Atom::operator== (const Atom& B) const {
    const Atom& A = (*this);
    return (std::strcmp(A.key, B.key) == 0 && A.value == B.value);
}
bool Atom::operator< (const Atom& B) const {
    const Atom& A = (*this);
    return !(A > B || A == B);
}
bool Atom::operator>= (const Atom& B) const {
    const Atom& A = (*this);
    return (A > B || A == B);
}
bool Atom::operator<= (const Atom& B) const {
    const Atom& A = (*this);
    return (A < B || A == B);
}
bool Atom::operator!=(const Atom& B) const {
    const Atom& A = (*this);
    return !(A == B);
}



Head::Head() {
    min = blank_min;
    max = blank_min;
    begin = 0;
    number = 0;
}
//在已知Atom属于该头所引领的体时，将Atom插入，改变对应的体以及自身（不裂块）。
void Head::insert(const std::string& file_name, const Atom& target, const Memory& environment) {
    Atom* temp = new Atom[number + 1];
    std::fstream file;
    file.open(file_name, std::fstream::in | std::fstream::out);
    if (!file) {
        std::cerr << "Cannot open the file.\n";
        return;
    }
    int start = begin * environment.size_of_atom;
    file.seekg(start);
    file.read(reinterpret_cast<char*>(temp), number * environment.size_of_atom);
    Atom* touch = std::lower_bound(temp, temp + number, target);
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
        if (target < min || min == blank_min) {
            min = target;
        }
        if (target > max) {
            max = target;
        }
        file.seekp(start);
        file.write(reinterpret_cast<char*>(temp), number * environment.size_of_atom);
        file.close();
        delete[] temp;
        return;
    }
}
//在已知Atom属于该头所引领的体时，删除，改变对应的体以及自身（不裂块,也不销毁空块）。
void Head::erase(const std::string& file_name, const Atom& target, const Memory& environment) {
    Atom* temp = new Atom[number + 1];
    std::fstream file;
    file.open(file_name, std::fstream::in | std::fstream::out);
    if (!file) {
        std::cerr << "Cannot open the file.\n";
        return;
    }
    int start = begin * environment.size_of_atom;
    file.seekg(start);
    file.read(reinterpret_cast<char*>(temp), number * environment.size_of_atom);
    Atom* touch = std::lower_bound(temp, temp + number, target);
    if (number == 0 || (*touch) != target) {
        delete[] temp;
        file.close();
        return;
    }
    else {
        --number;
        if (number == 0) {
            min = blank_min;
            max = blank_min;
        }
        else {
            if (touch == temp) {
                min = *(touch + 1);
            }
            if (touch == temp + number + 1) {
                max = *(touch - 1);
            }
        }
        for (int i = (touch - temp);i < number;++i) {
            temp[i] = temp[i + 1];
        }
        Atom new_atom;
        temp[number] = new_atom;
        //写入整个数组。
        file.seekp(start);
        file.write(reinterpret_cast<char*>(temp), (number + 1) * environment.size_of_atom);
        delete[] temp;
        file.close();
        return;
    }
}
//在已知Atom属于该头所引领的体时，列举，不改变对应的体以及自身。
bool Head::list(const std::string& file_name, const Atom& front, const Atom& back, const Memory& environment) {
    Atom* temp = new Atom[number + 1];
    std::fstream file;
    file.open(file_name, std::fstream::in | std::fstream::out);
    if (!file) {
        std::cerr << "Cannot open the file.\n";
        return 0;
    }
    int start = begin * environment.size_of_atom;
    file.seekg(start);
    file.read(reinterpret_cast<char*>(temp), number * environment.size_of_atom);
    Atom* a1 = std::lower_bound(temp, temp + number, front);
    Atom* a2 = std::upper_bound(temp, temp + number, back);
    bool flag = 0;
    for (auto iter = a1;iter < a2;++iter) {
        std::cout << (*iter).value << ' ';
        flag = 1;
    }
    file.close();
    delete[] temp;
    return flag;
}




Memory::Memory(const std::string& file1, const std::string& file2, int size_of_body1) :size_of_body(size_of_body1) {
    head_file = file1;
    body_file = file2;
    num_of_heads = 0;
    num_of_atoms = 0;
    file.open(head_file, std::fstream::in|std::fstream::ate);
    if(!file){
        file.close();
        file.open(head_file,std::fstream::out);
        file.close();
    }
    else{
        num_of_heads = file.tellp() / size_of_head;
        file.close();
    }
    file.open(body_file, std::fstream::in|std::fstream::ate);
    if(!file){
        file.close();
        file.open(body_file, std::fstream::out);
        file.close();
    }
    else{
        num_of_atoms = num_of_heads * (size_of_body + 1);
        file.close();
    }
    if(num_of_heads == 0){
        initialise();
    }
}
void Memory::initialise() {
    file.open(head_file, std::fstream::in | std::fstream::out);
    Head new_head;
    file.write(reinterpret_cast<char*>(&new_head), size_of_head);
    file.close();
    ++num_of_heads;
    num_of_atoms += size_of_body + 1;
}

//传递参数：储存元素。
//寻找元素对应的头的位置。
int Memory::find(const Atom& target, const Head* temp) {
    int result = 0;
    if (temp[0].min > target && temp[0].number != 0) {
        result = 0;
    }
    else {
        bool flag = 1;
        for (int i = 0;i < num_of_heads - 1;++i) {
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
void Memory::insert(const Atom& target) {
    Head* temp = new Head[num_of_heads + 1];
    file.open(head_file, std::fstream::in | std::fstream::out);
    if (!file) {
        std::cerr << "Cannot open the file.\n";
        return;
    }
    file.seekg(0);
    file.read(reinterpret_cast<char*>(temp), num_of_heads * size_of_head);
    file.close();
    int place = find(target, temp);
    temp[place].insert(body_file, target, *this);
    //裂块。
    if (temp[place].number > size_of_body) {
        ++num_of_heads;
        //处理新块。
        Atom* new_block=new Atom[size_of_body + 1];
        file.open(body_file, std::fstream::in | std::fstream::out);
        file.seekg((temp[place].begin + size_of_body / 2) * size_of_atom);
        file.read(reinterpret_cast<char*>(new_block), size_of_atom);
        file.seekg((temp[place].begin + size_of_body / 2 + 1) * size_of_atom);
        file.read(reinterpret_cast<char*>(new_block + size_of_body / 2 + 1), size_of_body / 2 *size_of_atom);
        file.seekp((temp[place].begin + size_of_body / 2 + 1) * size_of_atom);
        file.write(reinterpret_cast<char*>(new_block + 1),size_of_body / 2 * size_of_atom);
        file.seekp(num_of_atoms * size_of_atom);
        file.write(reinterpret_cast<char*>(new_block + size_of_body / 2 + 1),size_of_body / 2 * size_of_atom);
        file.close();
        Head new_head;
        new_head.max = temp[place].max;
        new_head.begin = num_of_atoms;
        new_head.number = size_of_body / 2;
        new_head.min = new_block[size_of_body / 2 + 1];
        temp[place].max = new_block[0];
        temp[place].number = size_of_body / 2 + 1;
        //temp数组加载。
        delete[] new_block;
        for (int i = num_of_heads - 1;i > place + 1;--i) {
            temp[i] = temp[i - 1];
        }
        temp[place + 1] = new_head;
        num_of_atoms += size_of_body + 1;
    }
    file.open(head_file, std::fstream::in | std::fstream::out);
    file.seekp(0);
    file.write(reinterpret_cast<char*>(temp), num_of_heads * size_of_head);
    file.close();
    delete[] temp;
    return;
}

//减少（含并块）
void Memory::erase(const Atom& target) {
    Head* temp = new Head[num_of_heads + 1];
    file.open(head_file, std::fstream::in | std::fstream::out);
    if (!file) {
        std::cerr << "Cannot open the file.\n";
        return;
    }
    file.seekg(0);
    file.read(reinterpret_cast<char*>(temp), num_of_heads * size_of_head);
    file.close();
    int place = find(target, temp);
    temp[place].erase(body_file, target, *this);
    //销毁空块。
    if (temp[place].number == 0) {
        for (int i = place;i < num_of_heads;++i) {
            temp[i] = temp[i + 1];
        }
        --num_of_heads;
    }
    //并块。
    else if (temp[place].number < size_of_body / 2 && temp[place + 1].number < size_of_body / 2 && temp[place + 1].number != 0) {
        Atom* new_block=new Atom[size_of_body + 1];
        file.open(body_file,std::fstream::in | std::fstream::out);
        file.seekg(temp[place + 1].begin * size_of_atom);
        file.read(reinterpret_cast<char*>(new_block),temp[place + 1].number * size_of_atom);
        file.seekp((temp[place].begin + temp[place].number) * size_of_atom);
        file.write(reinterpret_cast<char*>(new_block),temp[place + 1].number * size_of_atom);
        file.close();
        delete[] new_block;
        temp[place].number += temp[place + 1].number;
        temp[place].max = temp[place + 1].max;
        for (int i = place + 1;i < num_of_heads;++i) {
            temp[i] = temp[i + 1];
        }
        --num_of_heads;
    }
    file.open(head_file, std::fstream::in | std::fstream::out);
    file.seekp(0);
    file.write(reinterpret_cast<char*>(temp), (num_of_heads + 1) * size_of_head);
    file.close();
    delete[] temp;
    return;
}

//传递参数：key。输出value。
void Memory::list(const std::string& key) {
    Atom front(key, 0);
    Atom end(key, 2147483647);
    Head* temp = new Head[num_of_heads + 1];
    file.open(head_file, std::fstream::in | std::fstream::out);
    if (!file) {
        std::cerr << "Cannot open the file.\n";
        return;
    }
    file.seekg(0);
    file.read(reinterpret_cast<char*>(temp), num_of_heads * size_of_head);
    file.close();
    int start = find(front, temp);
    int finish = find(end, temp);
    bool flag = 0;
    for (int i = start;i <= finish;++i) {
        flag += temp[i].list(body_file, front, end, *this);
    }
    if(flag == 0){
        std::cout << "null";
    }
    std::cout << "\r\n";
    delete[] temp;
    return;
}

int main() {
    int num_of_orders = 0;
    std::cin >> num_of_orders;
    std::string order, name;
    int value = 0;
    Memory memory("key_file", "value_file", 1000);
    for (int i = 0;i < num_of_orders;++i) {
        std::cin >> order;
        if (order == "insert") {
            std::getchar();
            std::cin >> name;
            std::getchar();
            std::cin >> value;
            Atom target(name, value);
            memory.insert(target);
        }
        else if (order == "delete") {
            std::getchar();
            std::cin >> name;
            std::getchar();
            std::cin >> value;
            Atom target(name, value);
            memory.erase(target);
        }
        else if (order == "find") {
            std::getchar();
            std::cin >> name;
            std::getchar();
            memory.list(name);
        }
        value = 0;
    }
    return 0;
}
