#pragma once
#include <functional>
#include <iostream>
#include <string>

/********************************
 * Date: 2019/11/23
 * Author: Xyc
 *
 * namespace my
 *
 *  binary tree base class
 * class BTree<T, Node>
 *
 *  node of binary tree base class
 * class BTreeNode_base<T>
 *
 *  iterator of binary tree base class
 *  not supported begin() end()
 * class BTree_iterator<Node>
 *
 *  const_iterator of binary tree base class
 *  not supported begin() end()
 * class BTree_const_iterator<Node>
 *
 *  traits of binary tree
 * class BTree_traits<Node>;
 *
 * struct tree_out_mode;
 * struct tree_out_mode::left_shown_below;
 * struct tree_out_mode::left_shown_top;
 *
 * function:
 *  template<class Tree>
 *  void out_tree(const Tree tree, const std::string& des = " ",
 * std::ostream& os = std::cout, [tree_out_mode(default: left_shown_below)])
 *
 ********************************/

namespace my {

template <class Node>
class BTree_traits;

template <class T>
class BTreeNode_base;

template <class Node, class traits = BTree_traits<Node>>
class BTree_iterator;
template <class Node, class traits = BTree_traits<Node>>
class BTree_const_iterator;

template <class T, class Node = BTreeNode_base<T>,
          class traits = BTree_traits<Node>>
class BTree;

namespace utility {

template <class iterator>
static iterator Left_Most(iterator node) {
    if (node == nullptr) return nullptr;
    while (node.left() != nullptr) {
        node = node.left();
    }
    return node;
}
template <class node_type>
static node_type* Left_Most(node_type* node) {
    if (node == nullptr) return nullptr;
    while (node->left() != nullptr) {
        node = node->left();
    }
    return node;
}
template <class iterator>
static iterator Right_Most(iterator node) {
    if (node == nullptr) return nullptr;
    while (node.right() != nullptr) {
        node = node.right();
    }
    return node;
}
template <class node_type>
static node_type* Right_Most(node_type* node) {
    if (node == nullptr) return nullptr;
    while (node->right() != nullptr) {
        node = node->right();
    }
    return node;
}
}  // namespace utility

template <class Node>
class BTree_traits {
   public:
    using node_type = Node;
    using value_type = typename node_type::value_type;
    using iterator = BTree_iterator<node_type>;
    using const_iterator = BTree_const_iterator<node_type>;
};

template <class T>
class BTreeNode_base {
   public:
    using value_type = T;
    using node_type = BTreeNode_base;

   public:
    BTreeNode_base() : _value(), _left(nullptr), _right(nullptr) {}
    explicit BTreeNode_base(T value, node_type* left = nullptr,
                            node_type* right = nullptr)
        : _value(value), _left(left), _right(right) {}
    ~BTreeNode_base() = default;

   public:
    T& value(T value) {
        this->_value = value;
        return this->_value;
    }
    T& value() { return this->_value; }
    const T& value() const { return this->_value; }

    node_type*& left(node_type* left) {
        this->_left = left;
        return this->_left;
    }
    node_type*& left() { return this->_left; }
    node_type* const& left() const { return this->_left; }

    node_type*& right(node_type* right) {
        this->_right = right;
        return this->_right;
    }
    node_type*& right() { return this->_right; }
    node_type* const& right() const { return this->_right; }

   protected:
    T _value;
    node_type* _left;
    node_type* _right;
};

template <class Node, class traits>
class Tree_iterator_base {
   public:
    using iterator_category = std::input_iterator_tag;
    using value_type = typename traits::value_type;
    using difference_type = ptrdiff_t;
    using pointer = typename value_type*;
    using reference = typename value_type&;
    using node_type = typename traits::node_type;
    using iterator = typename traits::iterator;
    using const_iterator = typename traits::const_iterator;
};

template <class Node, class traits>
class BTree_const_iterator : public Tree_iterator_base<Node, traits> {
   private:
    using __base = Tree_iterator_base<Node, traits>;

   public:
    using __base::node_type, __base::value_type, __base::iterator,
        __base::const_iterator;

   public:
    explicit BTree_const_iterator(node_type* const node = nullptr)
        : _node(node) {}
    BTree_const_iterator(const BTree_const_iterator& node)
        : _node(node._node) {}
    BTree_const_iterator(BTree_const_iterator&& node) : _node(node._node) {}
    iterator left() const { return iterator(_node->left()); }
    iterator right() const { return iterator(_node->right()); }
    const value_type& operator*() const { return _node->value(); }
    const value_type* operator->() const { return &_node->value(); }
    iterator& operator=(iterator it) {
        _node = it._node;
        return *this;
    }
    iterator& operator=(node_type* it) {
        _node = it;
        return *this;
    }
    bool operator==(const iterator& it) const { return _node == it._node; }
    bool operator!=(const iterator& it) const { return !(*this == it); }

   public:
    node_type* _node;
    node_type*& ptr() { return _node; }
};

template <class Node, class traits>
class BTree_iterator : public BTree_const_iterator<Node, traits> {
   private:
    using __base = BTree_const_iterator<Node, traits>;

   public:
    using __base::_node, __base::node_type, __base::value_type,
        __base::iterator, __base::const_iterator;

   public:
    explicit BTree_iterator(node_type* node = nullptr) : __base(node) {}
    BTree_iterator(const BTree_iterator& node) : __base(node._node) {}
    BTree_iterator(BTree_iterator&& node) : __base(node._node) {}
    iterator left(value_type value) {
        if (_node->left() == nullptr)
            _node->left() = new node_type(value);
        else
            _node->left()->value() = value;
        return left();
    }
    iterator right(value_type value) {
        if (_node->right() == nullptr)
            _node->right() = new node_type(value);
        else
            _node->right()->value() = value;
        return right();
    }
    value_type& operator*() { return _node->value(); }
    value_type* operator->() { return &_node->value(); }
    iterator& operator=(iterator it) {
        _node = it._node;
        return *this;
    }
    iterator& operator=(node_type* it) {
        _node = it;
        return *this;
    }
};

template <class T, class Node, class traits>
class BTree {
   public:
    using node_type = typename traits::node_type;
    using value_type = typename traits::value_type;
    using iterator = typename traits::iterator;
    using const_iterator = typename traits::const_iterator;

   protected:
    BTree(node_type* node) : _root(node) {}
    void Destroy(node_type*& node) {
        if (node == nullptr) return;
        if (node->left() != nullptr) {
            Destroy(node->left());
            node->left(nullptr);
        }
        if (node->right() != nullptr) {
            Destroy(node->right());
            node->right(nullptr);
        }
        delete node;
    }
    void Copy(node_type* const& source, node_type*& dest) {
        if (source != nullptr) {
            if (dest == nullptr) {
                dest = new node_type(source->value());
            }
            dest->value() = source->value();
            if (source->left() != nullptr) {
                Copy(source->left(), dest->left());
            }
            if (source->right() != nullptr) {
                Copy(source->right(), dest->right());
            }
        }
    }

   public:
    BTree() { _root = new node_type; }
    explicit BTree(T value) { _root = new node_type(value); }
    BTree(BTree&& other) : _root(other._root) { other._root = nullptr; }
    BTree(const BTree& other) : _root(nullptr) { Copy(other._root, _root); }
    ~BTree() {
        Destroy(_root);
        _root = nullptr;
    }

    iterator end() { return iterator(); }
    const_iterator end() const { return const_iterator(); }

    void clear() {
        Destroy(_root);
        _root = nullptr;
    }

    BTree& operator=(const BTree& other) {
        clear();
        Copy(other._root, _root);
    }
    BTree& operator=(BTree&& other) {
        clear();
        _root = other._root;
        other._root = nullptr;
    }

   protected:
    iterator root() { return iterator(_root); }
    const_iterator root() const { return const_iterator(_root); }
    iterator left() { return iterator(_root->left()); }
    const_iterator left() const { return const_iterator(_root->left()); }
    iterator left(value_type value) {
        if (_root->left() == nullptr)
            _root->left() = new node_type(value);
        else
            _root->left()->value() = value;
        return iterator(_root->left());
    }
    const_iterator left(value_type value) const {
        if (_root->left() == nullptr)
            _root->left() = new node_type(value);
        else
            _root->left()->value() = value;
        return const_iterator(_root->left());
    }
    iterator right() { return iterator(_root->right()); }
    const_iterator right() const { return const_iterator(_root->right()); }
    iterator right(value_type value) {
        if (_root->right() == nullptr)
            _root->right() = new node_type(value);
        else
            _root->right()->value() = value;
        return iterator(_root->right());
    }
    const_iterator right(value_type value) const {
        if (_root->right() == nullptr)
            _root->right() = new node_type(value);
        else
            _root->right()->value() = value;
        return const_iterator(_root->right());
    }

   protected:
    node_type* _root;
};

struct tree_out_mode {
    struct left_shown_below {};
    struct left_shown_top {};
};

template <class Tree>
void out_tree(const Tree& tree, const std::string& des = " ",
              std::ostream& os = std::cout) {
    out_tree(tree, des, os, tree_out_mode::left_shown_below());
}

template <class Tree>
void out_tree(const Tree& tree, const std::string& sep, std::ostream& os,
              tree_out_mode::left_shown_below) {
    _out_tree(tree.root(), sep, os);
}

template <class Tree>
void out_tree(const Tree& tree, const std::string& sep, std::ostream& os,
              tree_out_mode::left_shown_top) {
    _out_tree2(tree.root(), sep, os);
}

template <class TreeIt>
void _out_tree(const TreeIt tree, const std::string& sep, std::ostream& os,
               size_t depth = 0) {
    if (tree.right() != TreeIt()) {
        _out_tree(tree.right(), sep, os, depth + 1);
    }
    for (size_t i = 0; i < depth; i++) {
        os << sep;
    }
    os << *tree << std::endl;
    if (tree.left() != TreeIt()) {
        _out_tree(tree.left(), sep, os, depth + 1);
    }
}

template <class TreeIt>
void _out_tree2(const TreeIt tree, const std::string& sep, std::ostream& os,
                size_t depth = 0) {
    if (tree.left() != TreeIt()) {
        _out_tree2(tree.left(), sep, os, depth + 1);
    }
    for (size_t i = 0; i < depth; i++) {
        os << sep;
    }
    os << *tree << std::endl;
    if (tree.right() != TreeIt()) {
        _out_tree2(tree.right(), sep, os, depth + 1);
    }
}

}  // namespace my
