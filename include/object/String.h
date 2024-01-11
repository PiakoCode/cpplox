#include "../object.h"
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
namespace obj {

class String : public Obj {
public:
    explicit String(const char* str = nullptr);
    String(const String& str);
    ~String();
    String& operator=(const String& str);
    bool operator==(const String& str) const;
    friend String operator+(const String& str1, const String& str2);
    char* c_str() const;
    size_t length() const;
    static String copyString(const std::string& str);
    static uint32_t hashString(const char* str);
    static uint32_t hashString(const std::string& str);
    // member

    char* m_data { nullptr };
    size_t m_size { 0 };
    uint32_t hash;
};

// 构造函数
inline String::String(const char* str)
    : Obj(ObjType::OBJ_STRING)
{
    delete[] m_data;
    if (str == nullptr) {
        m_data = new char[1];
        m_data[0] = '\0';
        m_size = 0;
    } else {
        m_size = strlen(str);
        m_data = new char[m_size + 1];
        strcpy(m_data, str);
    }
    hash = hashString(m_data);
}

inline String::String(const String& str)
    : Obj(ObjType::OBJ_STRING)
{
    m_size = str.m_size;
    delete[] m_data;
    m_data = new char[m_size + 1];
    strcpy(m_data, str.m_data);
    hash = hashString(m_data);
}

inline String::~String() { delete[] m_data; }

inline String& String::operator=(const String& str)
{
    if (this != &str) {
        String str_temp(str);
        char* p_temp = str_temp.m_data;
        str_temp.m_data = m_data;
        m_data = p_temp;
        m_size = str_temp.m_size;
        hash = str_temp.hash;
    }
    this->type = OBJ_STRING;
    return *this;
}

inline char* String::c_str() const
{
    return m_data;
}

inline size_t String::length() const
{
    return this->m_size;
}
/**
 * @brief 
 *  String 类比较
 * @param str 
 * @return true 
 * @return false 
 */
inline bool String::operator==(const String& str) const
{
    if (this == &str) {
        return true;
    }
    if (this->length() == str.length()) {
        if (strcmp(this->m_data, str.m_data) == 0) {
            return true;
        }
    }
    return false;
}

// 哈希计算

/**
 * @brief 
 * 获得字符串的哈希值，算法为 FNV-1a
 * @param str 需要计算的字符串
 * @return uint32_t 哈希值 
 */
uint32_t String::hashString(const char* str) {
    uint32_t hash = 2166136261U;
    size_t length = strlen(str);
    for (int i = 0; i < length; i++) {
        hash ^= (uint8_t)str[i];
        hash *= 16777619;
    }
    return hash;
}

/**
 * @brief 
 * 获得字符串的哈希值，算法为 FNV-1a
 * @param str 需要计算的字符串
 * @return uint32_t 哈希值 
 */
uint32_t String::hashString(const std::string& str)
{
    uint32_t hash = 2166136261U;
    for (char i : str) {
        hash ^= (uint8_t)i;
        hash *= 16777619;
    }
    return hash;
}
inline String operator+(const String& str1, const String& str2)
{
    String new_str;
    delete[] new_str.m_data;
    new_str.m_size = str1.length() + str2.length();
    new_str.m_data = new char[new_str.m_size + 1];
    std::copy(str1.m_data, str1.m_data + str1.length(), new_str.m_data);
    std::copy(str2.m_data, str2.m_data + str2.length(), new_str.m_data + str1.length());

    new_str.m_data[new_str.m_size] = '\0';
    return new_str;
}

inline String String::copyString(const std::string& str)
{
    auto hash = hashString(str);
    return String(str.c_str());
}

} // namespace obj