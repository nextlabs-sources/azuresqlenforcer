#ifndef JOIN_H
#define JOIN_H

#include "Interface.h"
#include <vector>
#include "serialize_format.h"

namespace resolve {
    class JoinedTable: public IJoinedTable {
    public:
        struct JoinNode {
            NodeType join_type_;
            uint64_t id_;
            Node * join_predicate_node;
        };

        void add_table(NodeType e, uint64_t tid, Node *join_predicate_node = nullptr) { join_nodes_.push_back({e, tid, join_predicate_node}); }
        void add_join_op(NodeType e, uint64_t eid) { join_nodes_.push_back({e, eid}); }
        void set_join_id(uint64_t jid) { join_id_ = jid; }
        const std::vector<JoinNode> & get_all_table_id() { return  join_nodes_;}

        bool IsJoinedTable(uint64_t table_id) override {
            for (auto &node: join_nodes_)
            {
              if (node.id_ == table_id)
              {
                return true;
              }
            }

            return false;
        }

        bool ResetJoinPredicate(uint64_t table_id, std::string filter_cond) override
        {
            for (auto join_node: join_nodes_)
            {
                if (join_node.join_type_ == E_CASE_JOIN_WHEN && join_node.id_ == table_id) {
                  auto join_predicate_node_parent = join_node.join_predicate_node->getParent();
                  auto ori_cond = join_node.join_predicate_node->serialize();

                  Node* ori_expr = join_node.join_predicate_node;
                  Node* ori_expr_paren = Node::makeNonTerminalNode(E_EXPR_LIST_WITH_PARENS, E_PARENS_PROPERTY_CNT, ori_expr);
                  ori_expr_paren->serialize_format = &SINGLE_WITH_PARENS_SERIALIZE_FORMAT;
                  Node* cond = Node::makeTerminalNode(E_STRING, filter_cond);
                  Node* new_expr = Node::makeNonTerminalNode(E_OP_AND, E_OP_BINARY_PROPERTY_CNT, ori_expr_paren, cond);
                  new_expr->serialize_format = &OP_AND_SERIALIZE_FORMAT;
                  join_predicate_node_parent->setChild(2, new_expr);

                  return true;
                }
            }
            return false;
        }

    private:
        std::vector<JoinNode> join_nodes_;
        uint64_t join_id_=0;
    };
}

#endif