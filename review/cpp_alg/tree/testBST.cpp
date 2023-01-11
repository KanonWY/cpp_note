#include "bst_tree.h"
#include <iostream>
#include <vector>

int Insert_recursion_test() {
    std::cout << "test tree" << std::endl;
    BSTree<int> bstree;
    bstree.Insert_recursion(10);
    bstree.Insert_recursion(20);
    bstree.Insert_recursion(30);
    bstree.Insert_recursion(40);
    bstree.Insert_recursion(2);
    bstree.Insert_recursion(22);
    bstree.Insert_recursion(32);

    auto res = bstree.Mid_travel();

    for (const auto &item : res) {
        std::cout << item << std::endl;
    }

    return 0;
}

int Insert_no_recursion_test() {
    std::cout << "test tree" << std::endl;
    BSTree<int> bstree;
    bstree.Insert_no_recursion(10);
    bstree.Insert_no_recursion(20);
    bstree.Insert_no_recursion(30);
    bstree.Insert_no_recursion(40);
    bstree.Insert_no_recursion(50);
    bstree.Insert_no_recursion(60);
    bstree.Insert_no_recursion(2);
    bstree.Insert_no_recursion(22);
    bstree.Insert_no_recursion(34);

    auto res = bstree.Mid_travle_2();

    for (const auto &item : res) {
        std::cout << item << std::endl;
    }
    return 0;
}

void test_remove_norecursion() {
    std::cout << "test tree" << std::endl;
    BSTree<int> bstree;
    bstree.Insert_no_recursion(10);
    bstree.Insert_no_recursion(20);
    bstree.Insert_no_recursion(30);
    bstree.Insert_no_recursion(40);
    bstree.Insert_no_recursion(50);
    bstree.Insert_no_recursion(60);
    bstree.Insert_no_recursion(2);
    bstree.Insert_no_recursion(22);
    bstree.Insert_no_recursion(34);

    auto res = bstree.Mid_travle_2();
    for (const auto &item : res) {
        std::cout << item << std::endl;
    }
    bstree.Remove_no_recursion(22);
    bstree.Remove_no_recursion(50);
    std::cout
        << "================================================================="
        << std::endl;
    res = bstree.Mid_travle_2();
    for (const auto &item : res) {
        std::cout << item << std::endl;
    }
}

void test_remove_recursion() {
    std::cout << "test tree" << std::endl;
    BSTree<int> bstree;
    bstree.Insert_no_recursion(10);
    bstree.Insert_no_recursion(20);
    bstree.Insert_no_recursion(30);
    bstree.Insert_no_recursion(40);
    bstree.Insert_no_recursion(50);
    bstree.Insert_no_recursion(60);
    bstree.Insert_no_recursion(2);
    bstree.Insert_no_recursion(22);
    bstree.Insert_no_recursion(34);

    auto res = bstree.Mid_travle_2();
    for (const auto &item : res) {
        std::cout << item << std::endl;
    }
    bstree.Remove_recursion(22);
    bstree.Remove_recursion(50);
    std::cout
        << "================================================================="
        << std::endl;
    res = bstree.Mid_travle_2();
    for (const auto &item : res) {
        std::cout << item << std::endl;
    }
}

void test_layer_travel() {
    std::cout << "test tree" << std::endl;
    BSTree<int> bstree;
    bstree.Insert_no_recursion(10);
    bstree.Insert_no_recursion(20);
    bstree.Insert_no_recursion(30);
    bstree.Insert_no_recursion(40);
    bstree.Insert_no_recursion(50);
    bstree.Insert_no_recursion(60);
    bstree.Insert_no_recursion(2);
    bstree.Insert_no_recursion(22);
    bstree.Insert_no_recursion(34);
    auto res = bstree.Layer_travel();
    for (const auto &item : res) {
        std::cout << item << std::endl;
    }
}

int main() {
    test_layer_travel();
    return 0;
}