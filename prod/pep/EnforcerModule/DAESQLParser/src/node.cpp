#include "node.h"
#include "math.h"
#include <stdarg.h>
#include <regex>
#include <stack>
//#include "sqlparser_sql2003_bison.h"
#include <assert.h>
#include "serialize_format.h"
int g_QuestMarkId;
std::string NodeTypeToString(NodeType tp) {
  return str_arr[tp];
}

ParseResult::ParseResult() : result_tree_(nullptr), accept(false),
    errFirstLine(0), errFirstColumn(0), type_(EMDB_DB_SQLSERVER){}

ParseResult::~ParseResult() {
    delete(result_tree_);
    result_tree_ = nullptr;
}

Node* Node::makeTerminalNode(NodeType tp, const std::string& yytext) {
    Node* ret = new Node;
    ret->nodeType_ = tp;
    ret->isTerminalToken = true;
    ret->terminalToken_.yytex = yytext;
    switch (tp) {
        case E_IDENTIFIER:
        case E_STRING: {
            ret->terminalToken_.str = yytext;
        } break;
        default:
            break;
    }
    return ret;
}

Node* Node::makeNonTerminalNode(NodeType tp, int num, ...) {
    Node* ret = new Node;
    ret->nodeType_ = tp;
    ret->isTerminalToken = false;

    va_list va;
#ifdef NODE_CHILDREN_DEBUG
   // ret->children_.resize(num);
    ret->children_.clear();
#else
    ret->children_ = new Node*[num];
#endif
    va_start(va, num);
    for (size_t i = 0; i < (size_t)num; ++i) {
        Node* child = va_arg(va, Node*);
        if (child)
            child->parent_ = ret;
        //ret->children_[i] = child;
        ret->children_.push_back(child);
    }
    ret->childrenCount_ = ret->children_.size();
    va_end(va);

    return ret;
}

bool Node::IsList(Node* root)
{
    bool ret = false;
    if (!root)
        return ret;
    switch (root->nodeType_) {
        case E_STMT_LIST:
        case E_SORT_LIST:
        case E_SELECT_EXPR_LIST:
        case E_FROM_LIST:
        case E_EXPR_LIST:
        case E_WHEN_LIST:
        case E_SIMPLE_IDENT_LIST:
        case E_WITH_LIST:
        case E_TABLE_HINT_LIST:
        case E_QUERY_HINT_LIST:
        case E_RELATION_FACTOR_LIST:
        case E_UPDATE_ELEM_LIST:
        case E_DML_SELECT_LIST:
        case E_OPT_DERIVED_COLUMN_LIST:
        case E_NAME_CHAIN:
        case E_SQL_ARGUMENT_LIST:
        case E_ROW_EXPR_AS_LABEL_LIST:
        case E_NAME_TYPE_LIST:
        case E_DATA_TYPE_LIST:
        case E_CASE_JOIN_WHEN_LIST:
        case E_SPECIAL_CLAUSE_LIST:{
            ret = true;
        } break;
        default:
            ret = false;
    }
    return ret;
}

int Node::ListLength(Node* root) {
    if (!root)
        return 0;
    if (!IsList(root))
        return 1;
    int cnt = 0;
    for (size_t i = 0; i < root->childrenCount_; ++i) {
        auto child = root->children_[i];
        if (child->nodeType_ == root->nodeType_) {
            cnt += ListLength(child);
        }
        else {
            cnt++;
        }
    }
    return cnt;
}

int Node::ListLengthNonRecursive(Node* root) {
    if (!root)
        return 0;
    int ret = 0;
    std::stack<Node*> stack;
    stack.push(root);
    Node *lpNode;

    while(!stack.empty()) {
        lpNode = stack.top();
        stack.pop();

        if(!lpNode) {
            continue;
        }
        else if (!IsList(lpNode)) {
            ret++;
            continue;
        }

        for (size_t i = 0; i < lpNode->childrenCount_; i++) {
            stack.push(lpNode->children_[i]);
        }
    }
    return ret;
}


void Node::ToList(Node* root, std::list<Node*>& ret)
{
    if (!root)
        return;
    if (!IsList(root))
        return ret.push_back(root);
    for (size_t i = 0; i < root->childrenCount_; ++i) {
        auto child = root->children_[i];
        if (!child) continue;
        if (child->nodeType_ == root->nodeType_) {
            ToList(child, ret);
        }
        else {
            ret.push_back(child);
        }
    }
}

void Node::ToListNonRecursive(Node* root, std::list<Node*>& ret) {
    if (!root)
        return;
    std::stack<Node*> stack;
    stack.push(root);
    Node *lpNode;

    while(!stack.empty()) {
        lpNode = stack.top();
        stack.pop();

        if(!lpNode) {
            continue;
        }
        else if (!IsList(lpNode)) {
            ret.push_back(lpNode);
            continue;
        }
        if (lpNode->childrenCount_ >= 1) {
            int i = (int)(lpNode->childrenCount_ - 1);
            for (; i >= 0; i--) {
                stack.push(lpNode->children_[i]);
            }
        }

    }
}

Node::Node() : nodeType_(E_NODETYPE_BEGIN), isTerminalToken(true), serialize_format(nullptr),
#ifdef NODE_CHILDREN_DEBUG
#else
        children_(nullptr),
#endif
questmarkid_(0),
childrenCount_(0),
parent_(nullptr) { }

Node::~Node() {
    //for (auto& nd : children_)
    for (size_t i = 0; i < childrenCount_; ++i) {
        auto& nd = children_[i];
        delete(nd);
        nd = nullptr;
    }
#ifdef NODE_CHILDREN_DEBUG
    children_.clear();
#else
    delete[] children_;
    children_ = nullptr;
#endif
    childrenCount_ = 0;
}

void Node::operator=(const Node & node) {
    assert(node.isTerminalToken);//only support TerminalToken
    nodeType_ = node.nodeType_;
    isTerminalToken = node.isTerminalToken;
    terminalToken_ = node.terminalToken_;
    serialize_format = node.serialize_format;
    //questmarkid_ = node.questmarkid_;
    //children_ = node.children_;
    //childrenCount_ = node.childrenCount_;
    //parent_ = node.parent_;
}

void Node::print(Node* root, size_t lvl /*= 0*/) {
    for (int i = 0; i < lvl; ++i) {
        fprintf(stderr, "    ");
    }
    if (!root) {
        fprintf(stderr, "|-> NIL\n");
        return;
    }
    fprintf(stderr, "|-> %s", NodeTypeToString(root->nodeType_).c_str());
    if (root->isTerminalToken) {
        fprintf(stderr , " : %s\n", root->terminalToken_.yytex.c_str());
    }
    else {   fprintf(stderr, "\n");
        for (size_t i = 0; i < root->childrenCount_; ++i) {
            print(root->getChild((int)i),  lvl+1);
        }
    }
}


std::string Node::SerializeNonRecursive(Node* root) {
    if (!root)
        return "";
    struct Item {
        Node* node=nullptr;
        std::string str;
    };

    std::string ret = "";
    std::stack<Item> stack;
    Item lpNode;

    Item tt;
    tt.node = root;
    stack.push(tt);
    while (stack.size() > 0) {
        lpNode = stack.top();
        stack.pop();
        std::string tmp = "";
        if (lpNode.node) {
            if (lpNode.node->isTerminalToken) {
                tmp = lpNode.node->terminalToken_.yytex;
            }
            else {
                auto serialize_format_tmp = lpNode.node->serialize_format->compact_;
                for (auto rit = serialize_format_tmp->rbegin(); rit != serialize_format_tmp->rend(); ++rit) {
                    if (rit->is_simple == 1) {
                        Item item;
                        item.node = nullptr;
                        item.str = rit->s0;
                        stack.push(item);
                    }
                    else {
                        Node* child = lpNode.node->getChild(rit->key);
                        if (child) {
                            if (rit->s2.length() > 0) {
                                Item item;
                                item.node = nullptr;
                                item.str = rit->s2;
                                stack.push(item);
                            }

                            Item item;
                            item.node = child;
                            item.str = "";
                            stack.push(item);

                            if (rit->s0.length() > 0) {
                                Item item;
                                item.node = nullptr;
                                item.str = rit->s0;
                                stack.push(item);
                            }
                        }
                    }
                }
            }
        }
        else {
            tmp = lpNode.str;
        }
        if (tmp.length() > 0) {
            ret += tmp;
        }
    }
    return ret;
}

std::string Node::serialize() {
    std::stringstream buf{};
    serialize(buf);
    return buf.str();
}

void Node::serialize(std::stringstream& buf) {
    if (!isTerminalToken) {
        for (auto info : *(serialize_format->compact_)) {
            if (info.is_simple == 1) {
                buf << info.s0;
            }
            else {
                Node* child = getChild(info.key);
                if (child) {
                    buf << info.s0;
                    child->serialize(buf);
                    buf << info.s2;
                }
            }
        }
    }
    else {
        //ret = terminalToken_.yytex;
        buf << terminalToken_.yytex;
    }
}

Node* Node::getParent() {
    return parent_;
}

Node* Node::setParent(Node* p) {
    assert(nullptr != p);
    parent_ = p;
    return this;
}

Node* Node::getChild(int key) {
    if (0 <= (size_t)key && (size_t)key < childrenCount_)
        return children_[key];
    else
        return nullptr;
}

Node** Node::getChildRef(int key) {
    if (0 <= (size_t)key && (size_t)key < childrenCount_)
        return &children_[key];
    else
        return nullptr;
}

bool Node::setChild(int key, Node* newchild) {
    if (0 <= (size_t)key && (size_t)key < childrenCount_) {
        children_[key] = newchild;
        if (newchild)
            newchild->setParent(this);
        return true;
    }
    else
        return false;
}

size_t Node::getChildrenCount() const {
    return childrenCount_;
}

std::string Node::text() {
    if(isTerminalToken)
        return terminalToken_.yytex;
    else
        return serialize();
}

void Node::set_text(const std::string& new_text) {
    assert(isTerminalToken);
    terminalToken_.yytex = new_text;
}

void Node::swap(Node* src){
    assert(src);
    NodeType t_tp =  this->nodeType_;
    bool t_ist = this->isTerminalToken;
    TerminalToken t_tt = this->terminalToken_;
    const _FmCB * t_sf = this->serialize_format;
    int t_q = this->questmarkid_;
    std::vector<Node*> t_vec = this->children_;
    size_t t_cc = this->childrenCount_;
    Node* t_parent = this->parent_;

    this->nodeType_ = src->nodeType_;
    this->isTerminalToken = src->isTerminalToken;
    this->terminalToken_ = src->terminalToken_;
    this->serialize_format = src->serialize_format;
    this->questmarkid_ = src->questmarkid_;
    this->children_ = src->children_;
    this->childrenCount_ = src->childrenCount_;
    this->parent_ = src->parent_;

    src->nodeType_ = t_tp;
    src->isTerminalToken = t_ist;
    src->terminalToken_ = t_tt;
    src->serialize_format = t_sf;
    src->questmarkid_ = t_q;
    src->children_ = t_vec;
    src->childrenCount_ = t_cc;
    src->parent_ = t_parent;
}

int Node::GetKey(const std::string& f) {
    if (f.length() < 3 || f.front() != '{' || f.back() != '}')
        return -1;
    return std::atoi(f.substr(1, f.length() - 2).c_str());
}

#if 1
//High efficiency
bool Node::Divide(const std::string& f, std::vector<std::string>& ret) {
    auto l = f.find("{");
    auto r = f.find("}");
    if (l == std::string::npos || r == std::string::npos)
        return false;
    else {
        ret.push_back(f.substr(0, l));
        ret.push_back(f.substr(l, r - l + 1));
        ret.push_back(f.substr(r + 1));
    }
    return true;
}
#else
// has efficiency problem why ? maybe std::regex
bool Node::Divide(const std::string& f, std::vector<std::string>& ret)
{
    std::string text(f);
    std::regex express("\\{[0-9]+\\}");
    std::match_results<std::string::iterator> results1;
    if(!std::regex_search(text.begin(), text.end(), results1, express) || 1 != results1.size())
        return false;

    auto l = results1.position(0);
    auto r = l + results1.length();
    ret.push_back(f.substr(0, l));
    ret.push_back(results1[0].str());
    ret.push_back(f.substr(r));
    return true;
}
#endif

Node* Node::remove_parens(Node* node) {
    while (is_with_parens(node)) {
        node = node->getChild(0);
    }
    return node;
}

bool Node::is_with_parens(Node* node) {
    assert(node);
    bool ret = false;
    switch (node->nodeType_) {
        case E_SELECT_WITH_PARENS:
        case E_JOINED_TABLE_WITH_PARENS:
        case E_EXPR_LIST_WITH_PARENS:
        case E_SIMPLE_IDENT_LIST_WITH_PARENS:
            ret = true;
            break;
        default:
            ret = false;
    }
    return ret;
}

#if 1
Node* Node::addjust_cross_join(Node* root, Node* cj) {
    assert(cj->nodeType_ == E_JOINED_TABLE);
    assert(cj->getChild(E_JOINED_TABLE_TABLE_FACTOR_R) == nullptr);
    cj->setChild(E_JOINED_TABLE_TABLE_FACTOR_R, root);
    return cj;
}
#else
Node* Node::addjust_cross_join(Node* root, Node* cj)
{
    assert(cj->nodeType_ == E_JOINED_TABLE);
    assert(cj->getChild(E_JOINED_TABLE_TABLE_FACTOR_R) == nullptr);
    if (root->nodeType_ == E_JOINED_TABLE)
    {
        Node* l = root->getChild(E_JOINED_TABLE_TABLE_FACTOR_L);
        assert(l != nullptr);
        root->setChild(E_JOINED_TABLE_TABLE_FACTOR_L, addjust_cross_join(l, cj));
        return root;
    }
    else
    {
        cj->setChild(E_JOINED_TABLE_TABLE_FACTOR_R, root);
        return cj;
    }
}
#endif

Node* Node::check_expr_is_column_alias(Node* root) {
    if (root->nodeType_ == E_OP_EQ) {
        Node* left = root->getChild(E_OP_BINARY_OPERAND_L);
        Node* right = root->getChild(E_OP_BINARY_OPERAND_R);
        assert(left != nullptr && right != nullptr);
        if (left->nodeType_ == E_OP_NAME_FIELD &&
        left->getChild(0) != nullptr && left->getChild(0)->nodeType_ == E_IDENTIFIER &&
        left->getChild(1) == nullptr && left->getChild(2) == nullptr &&
        left->getChild(3) == nullptr && left->getChild(4) == nullptr) {
            // this is a column_alias
            std::string alias_name = left->getChild(0)->terminalToken_.str;
            Node* alias = makeTerminalNode(E_IDENTIFIER, alias_name);
            alias->terminalToken_.str = alias_name;
            Node* ret = makeNonTerminalNode(E_ALIAS, E_ALIAS_PROPERTY_CNT,
                    right, alias, nullptr, nullptr, nullptr);
            ret->serialize_format = &AS_SERIALIZE_FORMAT;
            root->setChild(E_OP_BINARY_OPERAND_R, nullptr);
            delete(root);
            return ret;
        }
        else {
            // maybe here is an grammar error
        }
    }
    return root;
}

bool  Node::check_expr_table_hint(Node* root) {
    if (root->nodeType_ == E_IDENTIFIER) {
        std::string word = root->terminalToken_.str;
        std::transform(word.begin(), word.end(), word.begin(), ::toupper);
        if (TABLE_HINT_WORDS.find(word) == TABLE_HINT_WORDS.end())
            return false;
        return true;
    }
    else if (root->nodeType_ == E_OP_EQ) {
        Node* l = root->getChild(E_OP_BINARY_OPERAND_L);
        assert(l);
        if (l->nodeType_ != E_IDENTIFIER)
            return false;
        std::string word = l->terminalToken_.str;
        std::transform(word.begin(), word.end(), word.begin(), ::toupper);
        if ("INDEX" != word && "SPATIAL_WINDOW_MAX_CELLS" != word)
            return false;
        return true;
    }
    else if (root->nodeType_ == E_FUN_CALL) {
        Node* l = root->getChild(0);
        assert(l && l->nodeType_ == E_IDENTIFIER);
        std::string word = l->terminalToken_.str;
        std::transform(word.begin(), word.end(), word.begin(), ::toupper);
        if ("INDEX" != word)
            return false;
        return true;
    }
    return false;
}

std::string Node::convert_join_hint(int ival)
{
    switch (ival)
    {
        case 0:
            return "";
        case 1:
            return " LOOP";
        case 2:
            return " HASH";
        case 3:
            return " MERGE";
        default:
            return " REMOTE";
            
    }
}

Node* Node::make_query_hint(const std::string& text) {
    return makeTerminalNode(E_STRING, text);
}

Node* Node::make_query_hint(const std::string& text, Node* num) {
    Node* nd = makeTerminalNode(E_STRING, text);
    Node* ret = makeNonTerminalNode(E_QUERY_HINT, E_QUERY_HINT_PROPERTY_CNT, nd, num);
    ret->serialize_format = &DOUBLE_SERIALIZE_FORMAT;
    return ret;
}

bool Node::check_update_item(Node* root, Node*& out_expr) {
    if (root->nodeType_ == E_OP_EQ ||
        root->nodeType_ == E_OP_ASS_BIT_AND ||
        root->nodeType_ == E_OP_ASS_BIT_OR ||
        root->nodeType_ == E_OP_ASS_BIT_XOR ||
        root->nodeType_ == E_OP_ASS_ADD ||
        root->nodeType_ == E_OP_ASS_MINUS ||
        root->nodeType_ == E_OP_ASS_MUL ||
        root->nodeType_ == E_OP_ASS_DIV ||
        root->nodeType_ == E_OP_ASS_REM) {
        // check NAME ... NAME assign expr
        //       @variable assign expr
        Node* l = root->getChild(E_OP_BINARY_OPERAND_L);
        out_expr = root->getChild(E_OP_BINARY_OPERAND_R);
        switch (l->nodeType_) {
            case E_OP_NAME_FIELD: {
                Node* column = l->getChild(E_OP_NAME_FIELD_COLUMN_NAME);
                assert(column != nullptr);
                return column->nodeType_ != E_STAR;
            } break;
            case E_OP_EQ: {
                Node* var = l->getChild(E_OP_BINARY_OPERAND_L);
                Node* column = l->getChild(E_OP_BINARY_OPERAND_R);
                assert(var != nullptr);
                assert(column != nullptr);
                return var->nodeType_ == E_TEMP_VARIABLE &&
                    column->nodeType_ == E_OP_NAME_FIELD;
            } break;
            default:
                return false;
        }
    }
    else if (root->nodeType_ == E_FUN_CALL) {
        // check format like NAME.NAME(...)
        Node* func = root->getChild(0);
        if (func->nodeType_ == E_PROC_IDENT &&
            func->getChild(0) && func->getChild(1) &&
            !func->getChild(2) && func->getChild(3)) {
            return true;
        }
    }
    return false;
}

Node* Node::NAME_CHAIN_TO_TABLE_IDENT(Node* root) {
    assert(root->nodeType_ == E_NAME_CHAIN);
    std::list<Node*> ls;
    ToList(root, ls);
    if (ls.size() > 3)
        return nullptr;
    int i = 0;
    Node* ret = Node::makeNonTerminalNode(E_TABLE_IDENT, E_TABLE_IDENT_PROPERTY_CNT, nullptr, nullptr, nullptr, nullptr);
    for (auto it = ls.rbegin(); it != ls.rend(); ++it,++i) {
        ret->setChild(i, *it);
    }
    return nullptr;
}

Node* Node::NAME_CHAIN_STAR_TO_NAME_FIELD(Node* root) {
    return nullptr;
}

Node* Node::NAME_CHAIN_TO_NAME_FIELD(Node* root) {
    return nullptr;
}

bool Node::CHECK_FUNCTION_CALL_WITH_STAR(Node* node) {
    assert(node->isTerminalToken);
    std::string word = node->terminalToken_.yytex;
    std::transform(word.begin(), word.end(), word.begin(), ::toupper);
    return word == "COUNT";
}

NodeType Node::comp_all_some_any_op_form(NodeType op, int all_some_any) {
    NodeType ret = op;
    switch (op) {
        case E_OP_LE: {
            switch (all_some_any) {
                case 0: { ret = E_OP_LE_ALL; } break;
                case 1: { ret = E_OP_LE_SOME; } break;
                case 2: { ret = E_OP_LE_ANY; } break;
                default:break;
            }
        } break;
        case E_OP_LT: {
            switch (all_some_any) {
                case 0: { ret = E_OP_LT_ALL; } break;
                case 1: { ret = E_OP_LT_SOME; } break;
                case 2: { ret = E_OP_LT_ANY; } break;
                default:break;
            }
        } break;
        case E_OP_EQ: {
            switch (all_some_any) {
                case 0: { ret = E_OP_EQ_ALL; } break;
                case 1: { ret = E_OP_EQ_SOME; } break;
                case 2: { ret = E_OP_EQ_ANY; } break;
                default:break;
            }
        } break;
        case E_OP_NE: {
            switch (all_some_any) {
                case 0: { ret = E_OP_NE_ALL; } break;
                case 1: { ret = E_OP_NE_SOME; } break;
                case 2: { ret = E_OP_NE_ANY; } break;
                default:break;
            }
        } break;
        case E_OP_GE: {
            switch (all_some_any) {
                case 0: { ret = E_OP_GE_ALL; } break;
                case 1: { ret = E_OP_GE_SOME; } break;
                case 2: { ret = E_OP_GE_ANY; } break;
                default:break;
            }
        } break;
        case E_OP_GT: {
            switch (all_some_any) {
                case 0: { ret = E_OP_GT_ALL; } break;
                case 1: { ret = E_OP_GT_SOME; } break;
                case 2: { ret = E_OP_GT_ANY; } break;
                default:break;
            }
        } break;
        default:
            assert(false);
    }
    return ret;
}

const _FmCB *Node::op_serialize_format(NodeType tp) {
    const _FmCB *ret = nullptr;
    switch (tp) {
        case E_OP_LE: { ret = &OP_LE_SERIALIZE_FORMAT; } break;
        case E_OP_LT: { ret = &OP_LT_SERIALIZE_FORMAT; } break;
        case E_OP_EQ: { ret = &OP_EQ_SERIALIZE_FORMAT; } break;
        case E_OP_GE: { ret = &OP_GE_SERIALIZE_FORMAT; } break;
        case E_OP_GT: { ret = &OP_GT_SERIALIZE_FORMAT; } break;
        case E_OP_NE: { ret = &OP_NE_SERIALIZE_FORMAT; } break;
        case E_OP_LE_ALL: { ret = &OP_LE_ALL_SERIALIZE_FORMAT; } break;
        case E_OP_LT_ALL: { ret = &OP_LT_ALL_SERIALIZE_FORMAT; } break;
        case E_OP_EQ_ALL: { ret = &OP_EQ_ALL_SERIALIZE_FORMAT; } break;
        case E_OP_GE_ALL: { ret = &OP_GE_ALL_SERIALIZE_FORMAT; } break;
        case E_OP_GT_ALL: { ret = &OP_GT_ALL_SERIALIZE_FORMAT; } break;
        case E_OP_NE_ALL: { ret = &OP_NE_ALL_SERIALIZE_FORMAT; } break;
        case E_OP_LE_SOME: { ret = &OP_LE_SOME_SERIALIZE_FORMAT; } break;
        case E_OP_LT_SOME: { ret = &OP_LT_SOME_SERIALIZE_FORMAT; } break;
        case E_OP_EQ_SOME: { ret = &OP_EQ_SOME_SERIALIZE_FORMAT; } break;
        case E_OP_GE_SOME: { ret = &OP_GE_SOME_SERIALIZE_FORMAT; } break;
        case E_OP_GT_SOME: { ret = &OP_GT_SOME_SERIALIZE_FORMAT; } break;
        case E_OP_NE_SOME: { ret = &OP_NE_SOME_SERIALIZE_FORMAT; } break;
        case E_OP_LE_ANY: { ret = &OP_LE_ANY_SERIALIZE_FORMAT; } break;
        case E_OP_LT_ANY: { ret = &OP_LT_ANY_SERIALIZE_FORMAT; } break;
        case E_OP_EQ_ANY: { ret = &OP_EQ_ANY_SERIALIZE_FORMAT; } break;
        case E_OP_GE_ANY: { ret = &OP_GE_ANY_SERIALIZE_FORMAT; } break;
        case E_OP_GT_ANY: { ret = &OP_GT_ANY_SERIALIZE_FORMAT; } break;
        case E_OP_NE_ANY: { ret = &OP_NE_ANY_SERIALIZE_FORMAT; } break;
        case E_OP_OR: { ret = &OP_OR_SERIALIZE_FORMAT; } break;
        case E_OP_AND: { ret = &OP_AND_SERIALIZE_FORMAT; } break;
        case E_OP_NOT: { ret = &OP_NOT_SERIALIZE_FORMAT; } break;
        case E_OP_IS: { ret = &OP_IS_SERIALIZE_FORMAT; } break;
        case E_OP_IS_NOT: { ret = &OP_IS_NOT_SERIALIZE_FORMAT; } break;
        case E_OP_BTW: { ret = &OP_BETWEEN_SERIALIZE_FORMAT; } break;
        case E_OP_NOT_BTW: { ret = &OP_NOT_BETWEEN_SERIALIZE_FORMAT; } break;
        case E_OP_LIKE: { ret = &OP_LIKE_SERIALIZE_FORMAT; } break;
        case E_OP_NOT_LIKE: { ret = &OP_NOT_LIKE_SERIALIZE_FORMAT; } break;
        case E_OP_IN: { ret = &OP_IN_SERIALIZE_FORMAT; } break;
        case E_OP_NOT_IN: { ret = &OP_NOT_IN_SERIALIZE_FORMAT; } break;
        case E_OP_EXISTS: { ret = &OP_EXISTS_SERIALIZE_FORMAT; } break;
        case E_OP_CNN: { ret = &OP_CNN_SERIALIZE_FORMAT; } break;
        case E_OP_POW: { ret = &OP_POW_SERIALIZE_FORMAT; } break;
        case E_OP_POS: { ret = &OP_POS_SERIALIZE_FORMAT; } break;
        case E_OP_NEG: { ret = &OP_NEG_SERIALIZE_FORMAT; } break;
        case E_OP_ADD: { ret = &OP_ADD_SERIALIZE_FORMAT; } break;
        case E_OP_MINUS: { ret = &OP_MINUS_SERIALIZE_FORMAT; } break;
        case E_OP_MUL: { ret = &OP_MUL_SERIALIZE_FORMAT; } break;
        case E_OP_DIV: { ret = &OP_DIV_SERIALIZE_FORMAT; } break;
        case E_OP_REM: { ret = &OP_REM_SERIALIZE_FORMAT; } break;
        case E_OP_MOD: { ret = &OP_MOD_SERIALIZE_FORMAT; } break;
        case E_CASE: { ret = &CASE_SERIALIZE_FORMAT; } break;
        case E_BIT_OP_AND: { ret = &OP_BIT_AND_SERIALIZE_FORMAT ; }  break;    //BIT OPERATOR
        case E_BIT_OP_OR: { ret = &OP_BIT_OR_SERIALIZE_FORMAT ; }  break;
        case E_BIT_OP_NOT: { ret = &OP_BIT_NOT_SERIALIZE_FORMAT ; }  break;
        case E_BIT_OP_RS: { ret = &OP_BIT_RS_SERIALIZE_FORMAT ; }  break;
        case E_BIT_OP_LS: { ret = &OP_BIT_LS_SERIALIZE_FORMAT ; }  break;
        case E_OP_MEMBER:{ ret = &OP_MEMBER_SERIALIZE_FORMAT ;} break;
        case E_OP_NOT_MEMBER: { ret = &OP_NOT_MEMBER_SERIALIZE_FORMAT ;} break;
        case  E_OP_LIKE_REGEXPR: { ret = &OP_LIKE_REGEXPR_SERIALIZE_FORMAT ;} break;
        case E_OP_AND_SPC: { ret = &OP_AND_SPC_SERIALIZE_FORMAT ;} break;
        default:
            assert(false/* to do */);
    }
    return ret;
}

char * Node::strupr(char *str)
{
    char *ptr = str;
    while (*ptr != '\0') {
        if (islower(*ptr))
            *ptr = (char)toupper(*ptr);
        ptr++;
    }
    return str;
}

void parser_db_field(Node* pnode, std::string & db, std::string & schema, std::string & tb, std::string & field){
    if (pnode == nullptr) return;
    if (pnode->nodeType_ == E_OP_NAME_FIELD) { ///E_OP_NAME_FIELD
        //ret._type = pnode->nodeType_;
        Node * pfield = pnode->getChild(0);
        Node * ptbname = pnode->getChild(1);
        Node * pschema = pnode->getChild(2);
        Node * pdbname = pnode->getChild(3);

        if(pdbname){
            db = pdbname->terminalToken_.str;
        }
        if(pschema){
            schema = pschema->terminalToken_.str;
        }
        if(ptbname){
            tb = ptbname->terminalToken_.str;
        }
        if (pfield){
            if (pfield->nodeType_ == E_IDENTIFIER) {
                field = pfield->terminalToken_.str;
            } else if(pfield->nodeType_ == E_STAR){// E_STAR *
                field = "*";
            }
        }

    }
}
