#ifndef AVL_AVL_H
#define AVL_AVL_H
#include <memory>
#include <string>
#include <memory>
#include <string>

class AVL;

class AVLNode {
public:
    AVLNode(int key);
    //AVLNode * newNode(int key,std::shared_ptr<AVLNode> sharedPtr);
    AVLNode(int key, std::weak_ptr<AVLNode> parent);
    bool IsLeaf() const;
    bool IsMissingChild() const;
    bool HasLeftChild() const;
    bool HasRightChild() const;
    void DeleteChild(const std::shared_ptr<AVLNode>& v);
    void ReplaceChild(const std::shared_ptr<AVLNode>& v, const std::shared_ptr<AVLNode>& u);
    //static int height(const std::shared_ptr<AVLNode>& h);
    //static int balance_factor(const std::shared_ptr<AVLNode>& N);
    // static int max(int a, int b);

    //int height(std::shared_ptr<AVLNode> h); //calculates height of a particular node
    // h and returns it
    AVLNode();
    // static int height(const std::shared_ptr<AVLNode> &h);

    int calc_height;
    std::weak_ptr<AVLNode> parent_;
    std::shared_ptr<AVLNode> left_;
    std::shared_ptr<AVLNode> right_;
    int getkey();    //returns key_
    void setkey(int val); // key_=val;
    int balance_factor;
    int parent_key;    //int balance_factor(const std::shared_ptr<AVLNode> &N);


    //AVLNode* root = NULL;
    //static int height(AVLNode* Node1); I am not sure about this. Check this
    // int newNode(int element);
private:
    int key_{};


    friend AVL;
}; // class AVlNode

//// CHECK THIS
//struct avlnode{t
//public:
//    int height;
//    int value;
//    std::shared_ptr<avlnode>leftchild;
//    std::shared_ptr<avlnode> rightchild;
//};
//


class AVL {
public:
    AVL();

    static std::shared_ptr<AVLNode> insert(std::shared_ptr<AVLNode> node, int key, std::shared_ptr<AVLNode> parent_node);
//    static std::shared_ptr<AVLNode> insert(std::shared_ptr<AVLNode> node, int key);
    void Insert(int key);
    bool Delete(int key);
    bool Find(int key) const;
    std::string JSON() const;
    size_t size() const;
    bool empty() const;
    int DeleteMin();

    static std::shared_ptr<AVLNode> rightRotate(const std::shared_ptr<AVLNode>& y);
    static std::shared_ptr<AVLNode> leftRotate(const std::shared_ptr<AVLNode>& y);


private:
    void DeleteLeaf(std::shared_ptr<AVLNode> currentNode);
    int DeleteMin(std::shared_ptr<AVLNode> currentNode);

    std::shared_ptr<AVLNode> root_;
    size_t size_;
}; // class BST
#endif //AVL_AVL_H
