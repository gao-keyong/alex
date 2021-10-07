#ifndef TOKEN_H
#define TOKEN_H

#include <ostream>

class Token
{
private:
    std::string type_;  // 标记类型
    unsigned line_;     // 标记所在行
    unsigned column_;   // 标记所在列
    std::string token_; // 标记内容
    std::string error_; // 错误类型，仅对出错标记有效

public:
    Token();
    Token(std::string type_, unsigned line, unsigned column, std::string str);
    ~Token();
    // 获取当前标记类型
    std::string getType() const;
    // 设置当前标记类型
    void setType(std::string type_);
    // 设置当前标记内容
    void setToken(std::string token);
    // 设置当前标记错误类型，该方法仅对出错标记有效
    void setError(std::string error);
    // 格式化输出标记
    friend std::ostream &operator<<(std::ostream &os, const Token &token);
};

#endif