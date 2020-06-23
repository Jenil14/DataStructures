#include <iostream>
#include <string>
#include "AVL.h"
#include <fstream>
#include "json.hpp"

using namespace std;


void traverse(std::shared_ptr<AVLNode> sharedPtr);

std::string getKey(int i, int numOps)
{
    unsigned int count = std::to_string(numOps).size();
    std::string result = std::to_string(i);
    while (result.size() < count)
    {
        result = "0" + result;
    }
    return result;
}


nlohmann::json resultJSON;
int avlSize = 0;
int maxHeight = -1;

int main(int argc, char **argv){

    shared_ptr<AVLNode> root = nullptr;

    nlohmann::json json1;
    if (argc <= 2)
    {
        std::ifstream file;
        file.open(argv[1]);
        if (file.is_open())
        {
            file >> json1;
        }
    }
    try {
        int numOps = json1["metadata"]["numOps"];
        for (int i = 1; i <= numOps; i++)
        {
            int key = json1[getKey(i, numOps)]["key"];
            root = AVL::insert(root, key, nullptr);
            if(root->parent_.lock() != nullptr)
                cout<< "The parent of root" <<root->getkey() << "is"<<root->parent_.lock()->getkey();
        }


        traverse(root);
        resultJSON["height"] = maxHeight;
        resultJSON["size"] = avlSize;
        cout << resultJSON.dump(2);
    }
    catch (nlohmann::detail::type_error &e)
    {
        std::cout << e.what();
    }
    return 0;
}

void traverse(shared_ptr<AVLNode> currentNode)
{
    if(currentNode!= nullptr) {
        int balance = (currentNode->balance_factor) * (-1);
        int height = (currentNode->calc_height) - 1;
        string key = to_string(currentNode->getkey());
        int key1 = (currentNode -> getkey());
        avlSize++;
        if(balance <= 1 || balance >=-1){
            resultJSON[key]["balance factor"] = balance;
        }
        if(balance < -1){
            balance = 0;
            resultJSON[key]["balance factor"] = balance;
        }
        resultJSON[key]["height"] = height;
        if (currentNode->left_ != nullptr)
            resultJSON[key]["left"] = currentNode->left_->getkey();
        if(currentNode->right_ != nullptr)
            resultJSON[key]["right"] = currentNode->right_->getkey();
        if ((currentNode->parent_.lock()) != nullptr) {
            resultJSON[key]["parent"] = currentNode->parent_.lock()->getkey();
        } else {
            if(currentNode->parent_.lock() == nullptr){
                resultJSON[key]["root"] = true;
                resultJSON["root"] = key1;}
        }
        if(height > maxHeight) {
            maxHeight = height;
        }
        traverse(currentNode->left_);
        traverse(currentNode->right_);
    }
}
