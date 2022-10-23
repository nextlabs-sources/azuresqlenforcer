#ifndef HANDLE_H
#define HANDLE_H

#include <string>
#include <set>
#include <map>
#include <vector>
#include <algorithm>
#include "StructDefine.h"
#include "tool.h"


class Handle {
public:
    virtual ~Handle() {}
    virtual POLICY_ENGINE_HANDLE_TYPE GetHandleType() const = 0;
};

class StringList : public Handle {
public:
    static StringList *MakeStringList(const std::set<std::string>& strings) {
        StringList *list = nullptr;
        for (auto it : strings) {
            list = new StringList(it, list);
        }
        return list;
    }
    static void FreeStringList(StringList *list) {
        for (StringList *it = list; it != nullptr; ) {
            StringList *tmp = it->_next;
            delete (it);
            it = tmp;
        }
    }
public:
    StringList(const std::string& cur, StringList *next) : _current(cur), _next(next) {}
    virtual POLICY_ENGINE_HANDLE_TYPE GetHandleType() const override { return POLICY_ENGINE_HANDLE_TYPE::PE_STRING_LIST; }
    const std::string& GetCurrent() { return _current; }
    StringList *GetNext() { return _next; }
private:
    std::string _current;
    StringList *_next;
};



#endif