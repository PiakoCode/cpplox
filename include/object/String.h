#include "../object.h"
#include <algorithm>
#include <cstddef>
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

    // member

    char* m_data;
    size_t m_size;
};

inline String::String(const char* str)
    : Obj(ObjType::OBJ_STRING)
{
    if (str == nullptr) {
        m_data = new char[1];
        m_data[0] = '\0';
        m_size = 0;
    } else {
        m_size = strlen(str);
        m_data = new char[m_size + 1];
        strcpy(m_data, str);
    }
}

inline String::String(const String& str)
    : Obj(ObjType::OBJ_STRING)
{
    this->type = OBJ_STRING;
    m_size = str.m_size;
    m_data = new char[m_size + 1];
    strcpy(m_data, str.m_data);
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

inline String operator+(const String& str1, const String& str2)
{
    String new_str;
    new_str.m_size = str1.length() + str2.length();
    new_str.m_data = new char[new_str.m_size + 1];
    std::copy(str1.m_data, str1.m_data + str1.length(), new_str.m_data);
    std::copy(str2.m_data, str2.m_data + str2.length(), new_str.m_data + str1.length());

    new_str.m_data[new_str.m_size] = '\0';
    return new_str;
}

inline String String::copyString(const std::string& str)
{
    return String(str.c_str());
}

} // namespace obj