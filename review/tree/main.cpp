#include "Tree_basic.h"


int main()
{
    Codec coder;
    TreeNode *p2 = Codec::deserialize("1,2,3,null,null,null,6");

    auto res2 = midtravel(p2);
    for (const auto &item : res2) {
        std::cout << "item = " << item << std::endl;
    }

    freeTree(p2);
}