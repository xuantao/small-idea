#include <set>
#include <vector>
#include <functional>
#include <time.h>
#include "../scoped.h"
#include "../scoped_set.h"
#include "test_util.h"

USING_NAMESPACE_ZH;

struct TreeNode
{
    typedef const detail::_tree_node<int>* NodePtr;

    int index;
    char color;
    int val;

    bool operator == (const TreeNode& other) const
    {
        return index == other.index
            && color == other.color
            && val == other.val;
    }

    bool operator != (const TreeNode& other) const
    {
        return !(*this == other);
    }
};

struct TreeLine
{
    typedef std::vector<TreeNode> NodeVec;
    NodeVec nodes;
};

class TreeTester
{
public:
    typedef detail::_tree_val<int> ValType;
    typedef std::vector<TreeLine> LineVec;
    typedef typename ValType::_node_ptr NodePtr;
    typedef const std::set<int>::_Node* StdNodePtr;

public:
    TreeTester() {}

    TreeTester(const scoped_set<int>& s)
    {
        Build(s.cbegin()._Node());
    }

    TreeTester(const std::set<int>& s)
    {
        Build(s.cbegin()._Mynode());
    }

public:
    void print()
    {
        int tolal_width = 100;
        int node_width = 4;
        for (int depth = 0; depth < (int)_lines.size(); ++depth)
        {
            int count = (int)std::pow(2, depth);
            int space = tolal_width / count;
            if (space <= node_width)
                break;

            TreeLine& line = _lines[depth];
            for (int idx = 0; idx < (int)line.nodes.size(); ++idx)
            {
                const TreeNode& node = line.nodes[idx];
                _print_space((int)((space - node_width) * 0.5));
                printf("%2d:%c", node.val, node.color);
                _print_space((int)((space - node_width) * 0.5));
            }
            _print_space(tolal_width - (int)line.nodes.size() * space);

            printf("\n");
        }
    }

    bool operator == (const TreeTester& other) const
    {
        if (_lines.size() != other._lines.size())
            return false;

        for (size_t i = 0; i < _lines.size(); ++i)
        {
            if (_lines[i].nodes.size() != other._lines[i].nodes.size())
                return false;

            for (size_t j = 0; j < _lines[i].nodes.size(); j++)
            {
                if (_lines[i].nodes[j] != other._lines[i].nodes[j])
                    return false;
            }
        }
        return true;
    }

    bool operator != (const TreeTester& other) const
    {
        return !(*this == other);
    }

protected:
     void Build(NodePtr root)
     {
         std::function<void(NodePtr, int, int)> visitor;
         visitor = [&](NodePtr ptr, int depth, int idx) {
             if (ptr->nil) return;

             TreeLine& line = _getline(depth);
             line.nodes.push_back(TreeNode());
             line.nodes.back().val = ptr->val;
             line.nodes.back().color = ptr->color == detail::_rb_color::red ? 'r' : 'b';
             line.nodes.back().index = idx;

             visitor(ptr->left, depth + 1, idx * 2);
             visitor(ptr->right, depth + 1, idx * 2 + 1);
         };

         visitor(root, 0, 0);
     }

     void Build(StdNodePtr root)
     {
         std::function<void(StdNodePtr, int, int)> visitor;
         visitor = [&](StdNodePtr ptr, int depth, int idx) {
             if (ptr->_Isnil) return;

             TreeLine& line = _getline(depth);
             line.nodes.push_back(TreeNode());
             line.nodes.back().val = ptr->_Myval;
             line.nodes.back().color = ptr->_Color ? 'b' : 'r';
             line.nodes.back().index = idx;

             visitor(ptr->_Left, depth + 1, idx * 2);
             visitor(ptr->_Right, depth + 1, idx * 2 + 1);
         };

         visitor(root, 0, 0);
     }

     void _print_space(int count)
     {
         while (count--) printf("-");
     }

     TreeLine& _getline(int depth)
     {
         while (_lines.size() <= depth) _lines.push_back(TreeLine());
         return _lines[depth];
     }

protected:
     LineVec _lines;
};

static void test_insert_erase()
{
    for (int i = 0; i < 20; ++i)
    {
        int count = 10 + rand() % 1000;
        scoped_set<int> mySet = scoped::set<int>(count);
        std::set<int> stdSet;

        assert(mySet.max_size() == count);
        for (int j = 0; j < count; ++j)
        {
            mySet.insert(j);
            stdSet.insert(j);
        }

        mySet.erase(count + 1);
        stdSet.erase(count + 1);

        TreeTester left(mySet);
        TreeTester right(stdSet);
        if (left != right)
        {
            test::log(test::Tab::tab++, "test insert begin");
            test::log("scoped set");
            left.print();
            test::log("std set");
            right.print();
            test::log(--test::Tab::tab, "test insert end");
        }

        for (int j = 0; j < 3; ++j)
        {
            int er = rand() % count;
            mySet.erase(er);
            stdSet.erase(er);

            TreeTester left(mySet);
            TreeTester right(stdSet);
            if (left != right)
            {
                test::log(test::Tab::tab++, "test erase begin");
                test::log("scoped set");
                left.print();
                test::log("std set");
                right.print();
                test::log(--test::Tab::tab, "test erase end");
            }
        }
    }
}

static void test_iterator()
{
    int count = 10;
    scoped_set<int> mySet = scoped::set<int>(count);
    std::set<int> stdSet;
    for (int i = 0; i < count; ++i)
    {
        mySet.insert(i);
        stdSet.insert(i);
    }

    auto it1 = mySet.begin();
    auto it2 = mySet.begin();
    for (; it1 != mySet.end(); ++it1, ++it2)
        assert(*it1 == *it2);
}

static void test_obj()
{
    scoped_set<test::Obj> set = scoped::set<test::Obj>(10);
    test::Obj obj(0);           // Obj(int)

    set.insert(obj);            // Obj(const Obj&)

    set.insert(test::Obj(1));   // Obj(int)
                                // Obj(Obj&& obj)
                                // ~Obj()

    set.insert(2);              // Obj(int)
                                // Obj(Obj&& obj)
                                // ~Obj()

    set.erase(obj);             // ~Obj()

    // ~Obj()
    // ~Obj()
    // ~Obj()
}

void scoped_set_test()
{
    test::log(test::Tab::tab++, "scoped_set_test begin");
    srand((unsigned)time(nullptr));

    //scoped_set<int> mySet = util::set<int>(1);
    //mySet.erase(mySet.begin());   // assert(false)
    //mySet.erase(mySet.end());     // assert(false)

    test_insert_erase();
    test_iterator();

    test::log(test::Tab::tab++, "scoped set test obj begin");
    test_obj();
    test::log(--test::Tab::tab, "scoped set test obj end");

    test::log(--test::Tab::tab, "scoped_set_test end");
}
