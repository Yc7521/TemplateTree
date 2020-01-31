#pragma once
#include "BTree.hpp"

namespace my {

template <class Node>
class BSTree_traits;
template <class Node>
class _BSTree_traits;

template <class T>
class BTreeNode_Parent;

template <class Node, class traits = BSTree_traits<Node>>
class BSTree_iterator;
template <class Node, class traits = BSTree_traits<Node>>
class BSTree_const_iterator;
template <class Node, class traits = _BSTree_traits<Node>>
class _BSTree_iterator;
template <class Node, class traits = _BSTree_traits<Node>>
class _BSTree_const_iterator;

template <class T, class less = std::less<T>, class Node = BTreeNode_Parent<T>,
          class traits = BSTree_traits<Node>>
class BSTree;

template <class T>
class BTreeNode_Parent {
   public:
    using value_type = T;
    using node_type = BTreeNode_Parent;

   public:
    BTreeNode_Parent()
        : _value(), _left(nullptr), _right(nullptr), _parent(nullptr) {}
    explicit BTreeNode_Parent(T value, node_type* left = nullptr,
                              node_type* right = nullptr,
                              node_type* parent = nullptr)
        : _value(value), _left(left), _right(right), _parent(parent) {}
    ~BTreeNode_Parent() = default;

   public:
    T& value(T value) {
        this->_value = value;
        return this->_value;
    }
    T& value() { return this->_value; }
    const T& value() const { return this->_value; }

    node_type*& left(node_type* left) {
        this->_left = left;
        // this->left()->parent(this);
        return this->_left;
    }
    node_type*& left() { return this->_left; }
    node_type* const& left() const { return this->_left; }

    node_type*& right(node_type* right) {
        this->_right = right;
        // this->right()->parent(this);
        return this->_right;
    }
    node_type*& right() { return this->_right; }
    node_type* const& right() const { return this->_right; }

    node_type*& parent(node_type* parent) {
        this->_parent = parent;
        return this->_parent;
    }
    node_type*& parent() { return this->_parent; }
    node_type* const& parent() const { return this->_parent; }

   protected:
    T _value;
    node_type* _left;
    node_type* _right;
    node_type* _parent;
};

template <class Node>
class _BSTree_traits {
   public:
    using node_type = Node;
    using value_type = typename node_type::value_type;
    using iterator = _BSTree_iterator<node_type>;
    using const_iterator = _BSTree_const_iterator<node_type>;
};

template <class Node>
class BSTree_traits {
   public:
    using node_type = Node;
    using value_type = typename node_type::value_type;
    using iterator = BSTree_iterator<node_type>;
    using const_iterator = BSTree_const_iterator<node_type>;
};

template <class Node, class traits>
class _BSTree_const_iterator : public BTree_const_iterator<Node, traits> {
   private:
    using __base = Tree_iterator_base<Node, traits>;

   public:
    using __base::node_type, __base::value_type, __base::iterator,
        __base::const_iterator;

   public:
    explicit _BSTree_const_iterator(node_type* node = nullptr)
        : BTree_const_iterator<node_type, traits>(node) {}
    _BSTree_const_iterator(const _BSTree_const_iterator& node)
        : BTree_const_iterator<node_type, traits>(node._node) {}
    _BSTree_const_iterator(_BSTree_const_iterator&& node)
        : BTree_const_iterator<node_type, traits>(node._node) {}
    iterator parent() const { return iterator(this->_node->parent()); }

    _BSTree_const_iterator& operator=(_BSTree_const_iterator it) {
        this->_node = it._node;
        return *this;
    }
    bool operator==(const _BSTree_const_iterator& it) const {
        return this->_node == it._node;
    }
    bool operator!=(const _BSTree_const_iterator& it) const {
        return this->_node != it._node;
    }
    _BSTree_const_iterator& operator++() {
        if (this->_node->right() ==
            nullptr) {  // climb looking for right subtree
            node_type* _Pnode;
            while ((_Pnode = this->_node->parent()) != nullptr &&
                   this->_node == _Pnode->right()) {
                this->_node = _Pnode;  // ==> parent while right subtree
            }
            this->_node = _Pnode;  // ==> parent (head if end())
        } else {
            // ==> smallest of right subtree
            this->_node = utility::Left_Most(this->_node->right());
        }
        return *this;
    }
    _BSTree_const_iterator operator++(int) {
        iterator temp(*this);
        ++(*this);
        return temp;
    }
    _BSTree_const_iterator& operator--() {
        if (this->_node == nullptr) {
            // end() ==> nullptr (rightmost if have head node)
            // this->_node = this->_node->right();
        } else if (this->_node->left() == nullptr) {
            // climb looking for left subtree
            node_type* _Pnode;
            while ((_Pnode = this->_node->parent()) != nullptr &&
                   this->_node == _Pnode->left()) {
                this->_node = _Pnode;  // ==> parent while left subtree
            }
            if (this->_node != nullptr) {  // decrement non-begin()
                this->_node = _Pnode;      // ==> parent if not head
            }
        } else {
            // ==> largest(rightmost) of left subtree
            this->_node = utility::Right_Most(this->_node->left());
        }
        return *this;
    }
    _BSTree_const_iterator operator--(int) {
        iterator temp(*this);
        --(*this);
        return temp;
    }
    node_type*& ptr() { return this->_node; }
};

template <class Node, class traits>
class BSTree_const_iterator : public Tree_iterator_base<Node, traits> {
   private:
    using __base = Tree_iterator_base<Node, traits>;

   public:
    using __base::node_type, __base::value_type, __base::iterator,
        __base::const_iterator;

   public:
    BSTree_const_iterator(node_type* node = nullptr) : _iterator(node) {}
    BSTree_const_iterator(const BSTree_const_iterator& it)
        : _iterator(it._iterator) {}
    BSTree_const_iterator(BSTree_const_iterator&& it)
        : _iterator(it._iterator) {}

    BSTree_const_iterator& operator=(BSTree_const_iterator it) {
        _iterator = it._iterator;
        return *this;
    }
    const value_type& operator*() const { return *_iterator; }
    const value_type* operator->() const { return &(*_iterator); }
    bool operator==(const BSTree_const_iterator& it) const {
        return (_iterator == it._iterator);
    }
    bool operator!=(const BSTree_const_iterator& it) const {
        return (_iterator != it._iterator);
    }
    BSTree_const_iterator& operator++() {
        ++_iterator;
        return *this;
    }
    BSTree_const_iterator operator++(int) {
        auto temp = *this;
        ++_iterator;
        return temp;
    }
    BSTree_const_iterator& operator--() {
        --_iterator;
        return *this;
    }
    BSTree_const_iterator operator--(int) {
        auto temp = *this;
        --_iterator;
        return temp;
    }
    node_type*& ptr() { return _iterator._node; }

   public:
    _BSTree_const_iterator<node_type, _BSTree_traits<node_type>> _iterator;
};

template <class Node, class traits>
class _BSTree_iterator : public _BSTree_const_iterator<Node, traits> {
   private:
    using __base = _BSTree_const_iterator<Node, traits>;

   public:
    using __base::_node, __base::node_type, __base::value_type,
        __base::iterator, __base::const_iterator;

   public:
    explicit _BSTree_iterator(node_type* node = nullptr) : __base(node) {}
    _BSTree_iterator(const _BSTree_iterator& node) : __base(node._node) {}
    _BSTree_iterator(_BSTree_iterator&& node) : __base(node._node) {}

    iterator parent() { return iterator(this->_node->left()); }
    iterator parent(value_type value) {
        if (this->_node->parent() == nullptr)
            this->_node->parent() = new node_type(value);
        else
            this->_node->parent()->value() = value;
        return parent();
    }
    _BSTree_iterator& operator=(_BSTree_iterator it) {
        this->_node = it._node;
        return *this;
    }
    value_type& operator*() { return _node->value(); }
    value_type* operator->() { return &_node->value(); }
    _BSTree_iterator& operator++() {
        __base::operator++();
        return *this;
    }
    _BSTree_iterator operator++(int) {
        iterator temp(*this);
        ++(*this);
        return temp;
    }
    _BSTree_iterator& operator--() {
        __base::operator--();
        return *this;
    }
    _BSTree_iterator operator--(int) {
        iterator temp(*this);
        --(*this);
        return temp;
    }
    node_type*& ptr() { return this->_node; }
};

template <class Node, class traits>
class BSTree_iterator : public Tree_iterator_base<Node, traits> {
   private:
    using __base = Tree_iterator_base<Node, traits>;

   public:
    using __base::node_type, __base::value_type, __base::iterator,
        __base::const_iterator;

   public:
    BSTree_iterator(node_type* node = nullptr) : _iterator(node) {}
    BSTree_iterator(const BSTree_iterator& it) : _iterator(it._iterator) {}
    BSTree_iterator(BSTree_iterator&& it) : _iterator(it._iterator) {}

    BSTree_iterator& operator=(BSTree_iterator it) {
        _iterator = it._iterator;
        return *this;
    }
    value_type& operator*() { return *_iterator; }
    value_type* operator->() { return &(*_iterator); }
    bool operator==(const BSTree_iterator& it) const {
        return (_iterator == it._iterator);
    }
    bool operator!=(const BSTree_iterator& it) const {
        return (_iterator != it._iterator);
    }
    BSTree_iterator& operator++() {
        ++_iterator;
        return *this;
    }
    BSTree_iterator operator++(int) {
        auto temp = *this;
        ++_iterator;
        return temp;
    }
    BSTree_iterator& operator--() {
        --_iterator;
        return *this;
    }
    BSTree_iterator operator--(int) {
        auto temp = *this;
        --_iterator;
        return temp;
    }
    node_type*& ptr() { return _iterator._node; }

   public:
    _BSTree_iterator<node_type, _BSTree_traits<node_type>> _iterator;
};

template <class T, class less, class Node, class traits>
class BSTree : public BTree<typename traits::value_type,
                            typename traits::node_type, traits> {
   public:
    using node_type = typename traits::node_type;
    using value_type = typename traits::value_type;
    using iterator = typename traits::iterator;
    using const_iterator = typename traits::const_iterator;
    using base = typename BTree<value_type, node_type, traits>;
    using base::_root, base::root;

   protected:
    inline void _Copy(node_type* const& source, node_type*& dest) {
        if (dest == nullptr) {
            dest = new node_type(source->value());
        }
    }
    void Copy(node_type* const& source, node_type*& dest) {
        if (source != nullptr) {
            _Copy(source, dest);
            if (source->left() != nullptr) {
                _Copy(source->left(), dest->left());
                dest->left()->parent(dest);
                Copy(source->left(), dest->left());
            }
            if (source->right() != nullptr) {
                _Copy(source->right(), dest->right());
                dest->right()->parent(dest);
                Copy(source->right(), dest->right());
            }
        }
    }

   public:
    BSTree() : base::BTree(nullptr) {}
    BSTree(const BSTree& other) : base::BTree(nullptr) {
        Copy(other._root, _root);
    }
    BSTree(BSTree&& other) : base::BTree(other) {}
    ~BSTree() {
        if (_root != nullptr) {
            delete _root;
            _root = nullptr;
        }
    }

    void push(const T& item) {
        // TODO : may some bug here
        if (_root == nullptr) {
            _root = new node_type(item);
            return;
        }
        auto it = root().ptr();
        while (true) {
            while (less(item, it->value())) {
                if (it->left() == nullptr) {
                    it->left(new node_type(item));
                    it->left()->parent(it);
                    return;
                }
                it = it->left();
            }
            // insert the right node if item is same as *it
            while (!less(item, it->value())) {
                if (it->right() == nullptr) {
                    it->right(new node_type(item));
                    it->right()->parent(it);
                    return;
                }
                it = it->right();
            }
        }
    }
    void clear() {
        if (_root != nullptr) {
            delete _root;
            _root = nullptr;
        }
    }
    iterator find(const T& item) {
        return static_cast<const BSTree*>(this)->find(item).ptr();
    }
    const_iterator find(const T& item) const {
        // TODO : may some bug here
        if (_root == nullptr) {
            return end();
        }
        auto it = root().ptr();
        while (true) {
            bool has_down = false;
            if (less(item, it->value())) {
                if (it->left() != nullptr) {
                    it = it->left();
                    has_down = true;
                } else {
                    return end();
                }
            }
            if (less(it->value(), item)) {
                if (it->right() != nullptr) {
                    it = it->right();
                    has_down = true;
                } else {
                    return end();
                }
            }
            if (!has_down) {
                return it;
            }
        }
    }
    iterator remove(iterator where) {
        auto __root = root().ptr();
        if (__root == nullptr ||
            (__root->left() == nullptr && __root->right() == nullptr)) {
            delete _root;
            _root = nullptr;
            return end();
        }
        auto it = where.ptr();
        auto temp = it;
        auto parent = it->parent();
        auto left = it->left();
        auto right = it->right();
        // take the left node of this node
        if (left != nullptr) {
            // take the right_most node of left,
            //   and place the position of the deleted node
            temp = utility::Right_Most(left);
            // set right node
            temp->right(right);
            if (right != nullptr) {
                right->parent(temp);
            }
            // set parent node
            temp->parent(parent);
            if (parent != nullptr) {
                if (parent->left() == it) {
                    parent->left(temp);
                } else {
                    parent->right(temp);
                }
            }
            // set left node if left != temp(left has right)
            if (left->right() != nullptr) {
                // give parent its left node
                temp->parent()->right(temp->left());
                // set left node
                temp->left(left);
                left->parent(temp);
            }
            delete it;
            return ++iterator{temp};
        } else {
            // link parent and right if left == nullptr
            temp = right;
            if (parent != nullptr) {
                if (parent->left() == it) {
                    parent->left(temp);
                } else {
                    parent->right(temp);
                }
            }
            if (temp != nullptr) {
                temp->parent(parent);
            }
            delete it;
            return iterator{temp};
        }
    }
    BSTree& operator=(const BSTree& other) {
        clear();
        Copy(other._root, _root);
    }
    BSTree& operator=(BSTree&& other) {
        clear();
        _root = other._root;
        other._root = nullptr;
    }

    iterator begin() { return iterator(utility::Left_Most(root().ptr())); }
    const_iterator begin() const {
        return const_iterator(utility::Left_Most(root().ptr()));
    }
    iterator end() { return iterator(); }
    const_iterator end() const { return const_iterator(); }

   protected:
    const std::less<T> less = std::less<T>();
};

}  // namespace my
