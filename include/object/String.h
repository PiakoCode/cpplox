
#pragma once

#include "../object.h"

namespace obj {

class String : public Obj {
public:
    explicit String(const char* str = nullptr);
    String(const String& str);
    ~String() override;
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

} // namespace obj