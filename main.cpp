#include <algorithm>
#include <map>
#include <vector>
#include "BSTree.hpp"

using namespace std;
using namespace my;

class A {
   public:
    int a;
    friend ostream& operator<<(ostream& os, A a) {
        os << a.a;
        return os;
    }
    bool operator<(const A& a) const { return this->a < a.a; }
};

void o(const BSTree<A> tree) {
    auto i = tree.begin(), e = tree.end();
    while (i != e) {
        cout << *i << endl;
        ++i;
    }
}

int test() {
    // <!--- a lots of bugs!! ---> nothing here
    BSTree<A> tree;
    tree.push({12});
    tree.push({15});
    tree.push({10});
    tree.push({10});
    cout << "display this tree\n";
    o(tree);
    cout << "---------\n";

    cout << "use find function of BSTree\n";
    cout << "find the node if equal 10\n";
    auto it_10 = tree.find({10});
    cout << (it_10 == end(tree) ? "not found" : "found") << endl;
    cout << "remove this node\n";
    it_10 = tree.remove(it_10);
    cout << "\ndisplay this tree\n";
    o(tree);
    cout << "---------\n";

    cout << "use find function of std\n";
    cout << "find the node if equal 15\n";
    auto it_15 = find_if(begin(tree), end(tree),
                         [](const auto& a) { return a.a == 15; });
    cout << (it_15 == end(tree) ? "not found" : "found") << endl;
    cout << "remove this node\n";
    it_15 = tree.remove(it_15);
    cout << "\ndisplay this tree\n";
    o(tree);
    cout << "---------\n";

    cout << "add 9, 17\n";
    tree.push({17});
    tree.push({9});
    const auto& c_tree = tree;
    auto it = c_tree.begin();
    ++++it;
    it++;
    it--;
    ----it;
    it = c_tree.end();
    cout << "\ndisplay this tree\n";
    o(tree);
    cout << "---------\n";
    return 0;
}

int main() {
    test();

    // system("pause");
    return 0;
}
