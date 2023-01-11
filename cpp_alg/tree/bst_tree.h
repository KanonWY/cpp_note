#ifndef __KARBTREE_H__
#define __KARBTREE_H__

/**
 * 5个条件：
 *  1、每一个节点不是红色就是黑色
 *  2、根节点是黑色
 *  3、如果一个节点是红色，则它的孩子节点为黑色（红色节点不连续）
 *  4、每一个节点，该节点到叶子节点路径上的包含相同的黑色节点（黑高相同）
 *  5、每一个叶子节点都是黑色的
 *
 */

#include <queue>
#include <stack>
#include <vector>

//二叉搜索树
template <typename T> class BSTree {
    struct Node {
        T data;
        Node *_left;
        Node *_right;
    };

  public:
    Node *makeClear(Node *root) {
        if (!root) {
            return nullptr;
        }
        makeClear(root->_left);
        makeClear(root->_right);
        delete root;
        return nullptr;
    }

    Node *insert_recursion(Node *root, T value) {
        if (!root) {
            root = new Node;
            root->data = value;
            root->_left = nullptr;
            root->_right = nullptr;
        } else if (value < root->data) {
            root->_left = insert_recursion(root->_left, value);
        } else if (value > root->data) {
            root->_right = insert_recursion(root->_right, value);
        }
        return root;
    }

    Node *insert_no_recursion(T value) {
        if (!_root) {
            _root = new Node;
            _root->_left = nullptr;
            _root->_right = nullptr;
            _root->data = value;
            return _root;
        }
        Node *cur = _root;
        Node *parent = nullptr;
        while (cur) {
            if (cur->data < value) {
                parent = cur;
                cur = cur->_right;
            } else if (cur->data > value) {
                parent = cur;
                cur = cur->_left;
            } else {
                return cur;
            }
        }
        cur = new Node;
        cur->_left = nullptr;
        cur->_right = nullptr;
        cur->data = value;
        if (parent->data < value) {
            parent->_right = cur;
        } else {
            parent->_left = cur;
        }
        return cur;
    }

    //查找最小节点
    Node *findMinNode_recursion(Node *node) {
        if (!node || !node->_left) {
            return node;
        }
        return findMinNode_recursion(node->_left);
    }

    Node *findMinNode_norecursion(Node *node) {
        Node *cur = node;
        Node *parent = nullptr;
        while (cur) {
            parent = cur;
            cur = cur->_left;
        }
        return parent;
    }

    //删除一个节点
    bool remove_norecursion(T val) {
        if (!_root) {
            return false;
        }
        Node *parent = nullptr;
        Node *cur = _root;
        while (cur) {
            if (cur->data < val) {
                parent = cur;
                cur = cur->_right;
            } else if (cur->data > val) {
                parent = cur;
                cur = cur->_left;
            } else {                         // find
                if (cur->_left == nullptr) { //当前节点的左节点为空
                    if (cur == _root) {
                        _root = cur->_right;
                    } else {
                        if (cur == parent->_left) {
                            parent->_left = cur->_right;
                        } else {
                            parent->_right = cur->_right;
                        }
                    }
                    delete cur;
                } else if (cur->_right == nullptr) { //当前节点的右节点为空
                    if (cur == _root) {
                        _root = cur->_left;
                    } else {
                        if (cur == parent->_right) {
                            parent->_right = cur->_left;
                        } else {
                            parent->_right = cur->_left;
                        }
                    }
                    delete cur;
                } else { //左右节点都存在
                    Node *RightMinNode = findMinNode_norecursion(cur->_right);
                    if (RightMinNode == cur->_right) {
                        cur->_right = cur->_right->_right;
                        delete RightMinNode;
                    } else {
                        delete RightMinNode;
                    }
                }
                return true;
            }
        }
        return false;
    }

    Node *remove_recursion(Node *node, T value) {
        Node *tmp;

        if (node == nullptr) {
            return nullptr;
        } else if (value < node->data) {
            node->_left = remove_recursion(node->_left, value);
        } else if (value > node->data) {
            node->_right = remove_recursion(node->_right, value);
        } else if (node->_left && node->_right) {
            tmp = findMinNode_recursion(node->_right);
            node->data = tmp->data;
            node->_right = remove_recursion(node->_right, node->data);
            delete tmp;
        } else {
            tmp = node;
            if (node->_left == nullptr) {
                node = node->_right;
            } else if (node->_right == nullptr) {
                node = node->_left;
            }
            delete tmp;
        }
        return node;
    }

    //查找某一个值
    Node *findValue_norecursion(Node *node, T value) {
        Node *cur = node;
        while (cur) {
            if (cur->data < value) {
                cur = cur->_left;
            } else if (cur->data > value) {
                cur = cur->_right;
            } else {
                return cur;
            }
        }
        return cur;
    }

    Node *findValue_recursion(Node *node, T value) {
        if (node == nullptr) {
            return nullptr;
        }
        if (value < node->data) {
            return findValue_recursion(node->_left, value);
        }
        if (value > node->data) {
            return findValue_recursion(node->_right, value);
        }
        return node;
    }

    //前序非递归
    std::vector<T> PreTravel() {
        std::vector<T> res;
        if (!_root) {
            return res;
        }
        std::stack<Node *> inner_stack;
        inner_stack.push(_root);
        while (!inner_stack.empty()) {
            Node *tmp = inner_stack.top();
            inner_stack.pop();
            res.push_back(tmp->data);
            if (tmp->_right) {
                inner_stack.push(tmp->_right);
            }
            if (tmp->_left) {
                inner_stack.push(tmp->_left);
            }
        }
        return res;
    }

    //前序遍历递归实现
    void PreTravel_recursion(Node *node, std::vector<T> &output) {
        if (!node) {
            return;
        }
        output.push_back(node->data);
        PreTravel_recursion(node->_left, output);
        PreTravel_recursion(node->_right, output);
    }

    //中序遍历递归实现
    void MidTravel_recursion(Node *node, std::vector<T> &output) {
        if (node == nullptr) {
            return;
        }
        MidTravel_recursion(node->_left, output);
        output.push_back(node->data);
        MidTravel_recursion(node->_right, output);
    }

    //后续遍历递归实现
    void AfterTravel_recursion(Node *node, std::vector<T> &output) {
        if (node == nullptr) {
            return;
        }
        AfterTravel_recursion(node->_left, output);
        AfterTravel_recursion(node->_right, output);
        output.push_back(node->data);
    }

    //中序遍历非递归实现
    std::vector<T> MidTravel(Node *node) {
        std::vector<T> res;
        if (node == nullptr) {
            return res;
        }
        std::stack<Node *> inner_stack;
        Node *cur = node;
        while (!inner_stack.empty() || cur != nullptr) {
            while (cur != nullptr) {
                inner_stack.push(cur);
                cur = cur->_left;
            }
            Node *top = inner_stack.top();
            res.push_back(top->data);
            inner_stack.pop();
            if (top->_right != nullptr) {
                cur = top->_right;
            }
        }
        return res;
    }

    //后序遍历非递归
    std::vector<T> AfterTravel(Node *node) {
        std::vector<T> res;
        if (node == nullptr) {
            return res;
        }
        std::stack<Node *> st1;
        std::stack<Node *> st2;
        st1.push(node);
        while (!st1.empty()) {
            Node *tmp = st1.top();
            st1.pop();
            st2.push(tmp);
            if (tmp->_left) {
                st1.push(tmp->_left);
            }
            if (tmp->_right) {
                st1.push(tmp->_right);
            }
        }

        while (!st2.empty()) {
            res.push_back(st2.top());
            st2.pop();
        }
        return res;
    }

    std::vector<T> layerTravel(Node *node) {
        std::vector<T> res;
        if (node == nullptr) {
            return res;
        }
        std::queue<Node *> inner_queue;
        inner_queue.push(node);
        while (!inner_queue.empty()) {
            Node *frontNode = inner_queue.front();
            inner_queue.pop();
            res.push_back(frontNode->data);
            if (frontNode->_left) {
                inner_queue.push(frontNode->_left);
            }
            if (frontNode->_right) {
                inner_queue.push(frontNode->_right);
            }
        }
        return res;
    }

  public:
    BSTree() : _root(nullptr) {}
    ~BSTree() { makeClear(_root); }

    void Insert_no_recursion(T value) { insert_no_recursion(value); }

    void Insert_recursion(T value) { _root = insert_recursion(_root, value); }

    void Remove_no_recursion(T value) { remove_norecursion(value); }

    void Remove_recursion(T value) { remove_recursion(_root, value); }

    std::vector<T> Mid_travel() { return MidTravel(_root); }

    std::vector<T> Layer_travel() { return layerTravel(_root); }

    std::vector<T> Mid_travle_2() {
        std::vector<T> res;
        MidTravel_recursion(_root, res);
        return res;
    }

  private:
    Node *_root;
};

#endif