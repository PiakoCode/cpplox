#include "common.h"

namespace lox {
class String {
public:
  // 默认构造函数
  String(const char *str = nullptr);
  // 拷贝构造函数
  String(const String &str);
  // 析构函数
  ~String();
  // 字符串赋值函数
  String &operator=(const String &str);

private:
  char *m_data;
  int m_size;
};

// 构造函数
String::String(const char *str) {
  if (str == nullptr) // 加分点：对m_data加NULL 判断
  {
    m_data = new char[1]; // 得分点：对空字符串自动申请存放结束标志'\0'的
    m_data[0] = '\0';
    m_size = 0;
  } else {
    m_size = strlen(str);
    m_data = new char[m_size + 1];
    strcpy(m_data, str);
  }
}

// 拷贝构造函数
String::String(const String &str) // 得分点：输入参数为const型
{
  m_size = str.m_size;
  m_data = new char[m_size + 1]; // 加分点：对m_data加NULL 判断
  strcpy(m_data, str.m_data);
}

// 析构函数
String::~String() { delete[] m_data; }

// 字符串赋值函数（推荐使用）
// 保证了异常安全性
String &String::operator=(const String &str) {
  if (this != &str) {
    String str_temp(str);

    char *p_temp = str_temp.m_data;
    str_temp.m_data = m_data;
    m_data = p_temp;
  }
  return *this;
}
} // namespace lox