
#include "../../include/object/String.h"
#include "../../include/vm.h"
#include "../../include/table.h"
namespace obj {

// 构造函数
String::String(const char* str)
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

String::String(const String& str)
    : Obj(ObjType::OBJ_STRING)
{
    m_size = str.m_size;
    delete[] m_data;
    m_data = new char[m_size + 1];
    strcpy(m_data, str.m_data);
    hash = hashString(m_data);
    tableSet(vm.strings, this, Value(nullptr));
}

String::~String() { delete[] m_data; }

String& String::operator=(const String& str)
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

char* String::c_str() const
{
    return m_data;
}

size_t String::length() const
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
bool String::operator==(const String& str) const
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
uint32_t String::hashString(const char* str)
{
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
String operator+(const String& str1, const String& str2)
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
// TODO: FINISH HASH_TABLE
String String::copyString(const std::string& str)
{
    auto hash = hashString(str);
    // String interned = tableFindString(vm.strings, str, hash);
    // if (&interned != nullptr) {
    //     return interned;
    // }
    return String(str.c_str());
}
} // namespace obj