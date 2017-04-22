#include <set>
#include <vector>
#include <functional>
#include "util.h"
#include "scoped_set.h"
#include "test_util.h"

USING_NAMESPACE_ZH;

struct TreeNode
{
    typedef const detail::_tree_node<int>* NodePtr;

    int index;
    NodePtr ptr;
};

struct TreeLine
{
    typedef std::vector<TreeNode> NodeVec;
    NodeVec nodes;
};

class TreePrinter
{
public:
    typedef detail::_tree_val<int> ValType;
    typedef typename ValType::_node_ptr NodePtr;
    typedef std::vector<TreeLine> LineVec;
protected:
     TreePrinter() {}

     void Print()
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
                 printf("%2d:%c", node.ptr->val,
                     node.ptr->color == detail::_rb_color::red ? 'r' : 'b');
                 _print_space((int)((space - node_width) * 0.5));
             }
             _print_space(tolal_width - (int)line.nodes.size() * space);

             printf("\n");
         }
     }

     void Build(NodePtr root)
     {
         std::function<void(NodePtr, int, int)> visitor;
         visitor = [&](NodePtr ptr, int depth, int idx) {
             if (ptr->nil) return;

             TreeLine& line = _getline(depth);
             line.nodes.push_back(TreeNode());
             line.nodes.back().ptr = ptr;
             line.nodes.back().index = idx;

             visitor(ptr->left, depth + 1, idx * 2);
             visitor(ptr->right, depth + 1, idx * 2 + 1);
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

public:
    static bool Print(const ValType* pVal)
    {
        test::log(test::Tab::tab++, "test print tree begin");
        TreePrinter printer;
        printer.Build(pVal->_head->parent);
        printer.Print();
        test::log(--test::Tab::tab, "test print tree end");
        return true;
    }
};

void scoped_set_test()
{
    int count = 12;
    scoped_set<int> s = util::set<int>(count);
    assert(s.max_size() == count);

    for (int i = 0; i < count; ++i)
        s.insert(i);

    TreePrinter::Print(s.begin()._Val());
}
