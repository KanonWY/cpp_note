// #include "Tree_basic.h"
// #include <gtest/gtest.h>
// #ifdef __APPLE__
// #include <malloc/_malloc.h>
// #endif
// #include <vector>

// //              1
// //          2      3
// //      4     5  6     7
// //
// TEST(TreeTravel, example__1) {
//     TreeNode *root = GetTree({1, 2, 3, 4, 5, 6, 7});
//     auto pre = std::vector<int>{1, 2, 4, 5, 3, 6, 7};

//     EXPECT_EQ(PreTravel(root), pre);
//     freeTree(root);
// }

// //              1
// //          2      3
// //      4     5  6     7
// //

// TEST(TreeTravel, example__2) {
//     TreeNode *root = GetTree({1, 2, 3, 4, 5, 6, 7});
//     auto mid = std::vector<int>{4, 2, 5, 1, 6, 3, 7};
//     EXPECT_EQ(midtravel(root), mid);
//     freeTree(root);
// }

// //              1
// //          2      3
// //      4     5  6     7
// //

// TEST(TreeTravel, example__3) {
//     TreeNode *root = GetTree({1, 2, 3, 4, 5, 6, 7});
//     auto after = std::vector<int>{4, 5, 2, 6, 7, 3, 1};
//     EXPECT_EQ(aftertravel(root), after);
//     freeTree(root);
// }

// TEST(TreeTravel, example__level) {
//     TreeNode *root = GetTree({1, 2, 3, 4, 5, 6, 7});
//     auto level = std::vector<std::vector<int>>{{1}, {2, 3}, {4, 5, 6, 7}};
//     for (const auto &item : levelOrder(root)) {
//         for (const auto &it : item) {
//             std::cout << it << " ";
//         }
//         std::cout << std::endl;
//     }
//     // EXPECT_EQ(levelOrder(root), level);
//     freeTree(root);
// }

// TEST(Treeequip, example__4) {
//     TreeNode *p = GetTree({1, 2, 3, 4, 5, 6, 7});
//     TreeNode *q = GetTree({1, 2, 3, 4, 5, 6, 7});

//     isSameTree(p, q);

//     EXPECT_EQ(isSameTree(p, q), true);

//     freeTree(p);
//     freeTree(q);
// }

// //

// TEST(Treeeinvert, example__5) {
//     TreeNode *p = GetTree({4, 2, 7, 1, 3, 6, 9});
//     invertTree(p);
//     auto res = std::vector<int>{4, 7, 2, 9, 6, 3, 1};
//     EXPECT_EQ(levelOderToV1(p), res);

//     freeTree(p);
// }

// TEST(Test_averageOfLevels, averageOfLevels) {
//     TreeNode *p = GetTree({1, 2, 3, 4, 5, 6, 7});
//     std::vector<double> res = {1, 2.5, 5.5};
//     EXPECT_EQ(averageOfLevels(p), res);

//     freeTree(p);
// }

// TEST(Test_averageOfLevels___, averageOfLevels___) {
//     TreeNode *p = GetTree({1, 2, 3, 4, 5, 6, 7});

//     Codec code;
//     std::string res = code.serialize(p);
//     std::cout << "res == " << res << std::endl;

//     TreeNode *p2 = code.deserialize("1,2,3,null,null,null,6");

//     auto res2 = midtravel(p2);
//     for (const auto &item : res2) {
//         std::cout << "item = " << item << std::endl;
//     }

//     freeTree(p2);
// }