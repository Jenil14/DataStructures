#include <queue>
#include "iostream"
#include "AVL.h"


AVLNode::AVLNode(int key) :

        parent_(std::weak_ptr<AVLNode>()),
        left_(nullptr),
        right_(nullptr),
        key_(key){}

AVLNode::AVLNode(int key, std::weak_ptr<AVLNode> parent) :

        parent_(parent),
        left_(nullptr),
        right_(nullptr),
        key_(key){}
bool AVLNode::IsLeaf() const {
    return left_ == nullptr && right_ == nullptr;
}

bool AVLNode::HasLeftChild() const {
    return left_ != nullptr;
}

bool AVLNode::HasRightChild() const {
    return right_ != nullptr;
}

void AVLNode::DeleteChild(const std::shared_ptr<AVLNode>& v) {
    if (left_ == v) {
        left_ = nullptr;
    } else if (right_ == v) {
        right_ = nullptr;
    } else {
        std::cerr << "AVLnode::DeleteChild Error: non-child passed as argument\n";
        exit(EXIT_FAILURE);
    }
}

void AVLNode::ReplaceChild(const std::shared_ptr<AVLNode>& v, const std::shared_ptr<AVLNode>& u) {
    if (left_ == u || right_ == u) {
        std::cerr << "AVLnode::ReplaceChild Error: child passed as replacement\n";
    }
    if (left_ == v) {
        left_ = u;
        u->parent_ = v->parent_;
    } else if (right_ == v) {
        right_ = u;
        u->parent_ = v->parent_;
    } else {
        std::cerr << "AVLnode::ReplaceChild Error: non-child passed as argument\n";
        exit(EXIT_FAILURE);
    }
}

AVL::AVL() : root_(nullptr), size_(0) {}

void AVL::Insert(int key) {
    if (root_ == nullptr) {
        root_ = std::make_shared<AVLNode>(key);
        size_++;
        return;
    }
    std::shared_ptr<AVLNode> currentNode = root_, lastNode = nullptr;
    while (currentNode != nullptr) {
        lastNode = currentNode;
        currentNode = (key < currentNode->key_) ?
                      currentNode->left_ : currentNode->right_;
    }
    if (key < lastNode->key_) {
        lastNode->left_ = std::make_shared<AVLNode>(key, lastNode);
    } else {
        lastNode->right_ = std::make_shared<AVLNode>(key, lastNode);
    }
    size_++;
}
int max(int a, int b) {
    return (a > b)? a : b;}

int parent_key(const std::shared_ptr<AVLNode> &h){

    if(h->parent_.lock() != nullptr){
        return h->parent_.lock()->getkey();}
    else
        return 0;

}

int height(const std::shared_ptr<AVLNode> &h) {
    if (h == nullptr)
        return 0;
    else{
        h->calc_height = max(height (h->left_), height(h->right_))+1;
    }
    //std::cout << h->calc_height;
    return h->calc_height;

}

int balance_factor(const std::shared_ptr<AVLNode> &N) {
    if (N == nullptr)
        return 0;
    return height(N->left_) - height(N->right_);
}


AVLNode::AVLNode() {

}

int AVLNode::getkey(){
    return key_;
}

void AVLNode::setkey(int val){
    key_ = val;
}

AVLNode * newNode(int key,std::shared_ptr<AVLNode> sharedPtr) {
    auto *ptr = new AVLNode();
    ptr->setkey(key);
    ptr->left_ = nullptr;
    ptr->right_ = nullptr;
    if(sharedPtr != NULL)
        ptr->parent_ = sharedPtr;
    return ptr;
}

std::shared_ptr<AVLNode> AVL::rightRotate(const std::shared_ptr<AVLNode> &y) {
    std::shared_ptr<AVLNode> x = y->left_;
    std::shared_ptr<AVLNode> T2 = x->right_;

    std::weak_ptr<AVLNode> pOld = y->parent_;
    x->right_ = y;
    y->left_ = T2;
    y->parent_ = x; //assign weak ptr to shared ptr, we don't need lock
    x->parent_ = pOld;// removed the parent of b
    if(T2 != nullptr){
        T2->parent_ = y;}

    // Update heights
    y->calc_height = height(y);
    x->calc_height = height(y);
    x->balance_factor = balance_factor(x);
    y->balance_factor = balance_factor(y);
    if(T2 != nullptr){
        T2->balance_factor = balance_factor(T2);}

    // Return new root
    return x;
}

std::shared_ptr<AVLNode> AVL::leftRotate(const std::shared_ptr<AVLNode> &y) {
    //y=a, x=b, T2=b-
    std::shared_ptr<AVLNode> x = y->right_;
    std::shared_ptr<AVLNode> T2 = x->left_;
    std::weak_ptr<AVLNode> pOld = y->parent_;


    //perform rotation
    x->left_ = y;
    y->right_ = T2;

    y->parent_ = x;
    //x->parent_.reset();
    x->parent_=pOld;
    if(T2 != nullptr){
        T2->parent_ = y;

    }

    //Update heights

    y->calc_height = height(y);
    x->calc_height = height(x);
    x->balance_factor = balance_factor(x);
    y->balance_factor = balance_factor(y);
    if(T2 != nullptr){
        T2->balance_factor = balance_factor(T2);}

    // Return new root
    return x;

}


std::shared_ptr<AVLNode> AVL::insert(std::shared_ptr<AVLNode> node, int key, std::shared_ptr<AVLNode> parent_node) {
    /* 1. Perform the normal BST insertion */
    if (node == nullptr)
        return std::shared_ptr<AVLNode>(newNode(key,std::move(parent_node)));

    if (key < node->getkey())
        node->left_ = static_cast<const std::shared_ptr<AVLNode>>(insert(node->left_, key,node));
    else if (key > node->getkey())
        node->right_ = static_cast<const std::shared_ptr<AVLNode>>(insert(node->right_, key,node));
    else // Equal keys are not allowed in BST
        return node;

    /* 2. Update height of this ancestor node */
    node->calc_height = 1 + max(height(node->left_),
                                height(node->right_));

    /* 3. Get the balance factor of this ancestor
        node to check whether this node became
        unbalanced */
    int balance = balance_factor(node);
    node->balance_factor = balance;

    // Left Left Case
    if (balance > 1 && key < node->left_->getkey())
        return rightRotate(node);

    // Right Right Case
    if (balance < -1 && key > node->right_->getkey())
        return leftRotate(node);

    // Left Right Case
    if (balance > 1 && key > node->left_->getkey())
    {
        node->left_ = leftRotate(node->left_);
        return rightRotate(node);
    }

    // Right Left Case
    if (balance < -1 && key < node->right_->getkey())
    {
        node->right_ = rightRotate(node->right_);
        return leftRotate(node);
    }
    //node->parent_key=parent_key(node);
    /// int parent_val = node->parent_.lock()->getkey();


    //  node->parent_.lock()->getkey();

    /* return the (unchanged) node pointer */
    return node;
}
