#ifndef PARSER_H
#define PARSER_H

#include <string>
class ParseResult;
struct parser {
    static bool parse_sql2003(const std::string& sql, ParseResult* result);
    static bool compress_sql2003(const std::string& src, std::string& out);     /* will remove comment */
    static bool beautify_sql2003(const std::string& src, std::string& out);     /* will remove comment */
    
    //static bool parse_sqlbigquery(const std::string& sql, ParseResult* result);
    //static bool compress_sqlbigquery(const std::string& src, std::string& out);     /* will remove comment */
    //static bool beautify_sqlbigquery(const std::string& src, std::string& out);     /* will remove comment */

    //static bool parse_sqloracle(const std::string& sql, ParseResult* result);
    //static bool compress_sqloracle(const std::string& src, std::string& out);     /* will remove comment */
    //static bool beautify_sqloracle(const std::string& src, std::string& out);     /* will remove comment */

    //static bool parse_sqlhana(const std::string& sql, ParseResult* result);
    //static bool compress_sqlhana(const std::string& src, std::string& out);     /* will remove comment */
    //static bool beautify_sqlhana(const std::string& src, std::string& out);     /* will remove comment */

    //static bool parse_sqldb2(const std::string& sql, ParseResult* result);
    //static bool compress_sqldb2(const std::string& src, std::string& out);     /* will remove comment */
    //static bool beautify_sqldb2(const std::string& src, std::string& out);     /* will remove comment */
};

#include <sstream>
struct _Buf {
    _Buf() : line_(0), col_(0) {}
    /* no \n */
    void append(const std::string& ap) {
        s_ << ap;
        col_ += ap.length();
    }
    void new_line() {
        s_ << "\n";
        line_++;
        col_ = 0;
    }
    void adjust_col(int n) {
        append(std::string(n, ' '));
    }
    size_t get_col() const { return col_; }
    std::string get_str() const { return s_.str(); }
private:
    std::stringstream s_;
    size_t line_;
    size_t col_;
};
class Node;
void _beautify_sql(_Buf *buf, Node *node);


#endif