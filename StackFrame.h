#ifndef __STACK_FRAME_H__
#define __STACK_FRAME_H__

#include <string>
#include <stack>
using namespace std;

/*
StackFrame declaration
*/
class AVL;
class operand;
class StackFrame {
    int opStackMaxSize; // max size of operand stack
    int localVarSpaceSize; // size of local variable array
public:
    /*
    Constructor of StackFrame
    */
    operand* b;
    AVL* c;
    StackFrame();
    ~StackFrame();
    void doing(string*, int);

    /*
    Run the method written in the testcase
    @param filename name of the file
    */
    void run(std::string filename);
};
class operand {
private: 
    int countNode;
    int maxsize;
    friend class StackFrame;
    class Node {
    private:
        Node* next;
        int ivalue;
        float fvalue;
        int type;
    public:
        Node() {
            this->ivalue = 0;
            this->type = 5;
            this->fvalue = 0.0; 
            this->next = nullptr;
        }
        Node(Node* a) {
            this->ivalue = a->ivalue; 
            this->type = a->type; 
            this->fvalue = a->fvalue;
            this->next = a->next;
        }
        ~Node() {
            this->ivalue = 0;
            this->fvalue = 0;
            this->type = 0; 
            this->next = nullptr;
        }
        friend class operand;
    };
public: 
    Node* head; 
    Node* tail; 
    operand(int max) {
        head = tail = nullptr; 
        countNode = 0; 
        maxsize = max;
    }
    ~operand() {
        head = tail = nullptr; 
        countNode = maxsize = 0;
    }
    void push(Node *tmp); 
    float top(); 
    void pop(); 
    bool full();
    bool empty(); 
    int getSize(); 
    int checktype(); 
    int iconst(string h); 
    int imul();
    int fconst(string g);
    int fsub();
    int iadd();
    int fadd();
    int isub();
    int idiv();
    int fdiv();
    int irem();
    int ineg();
    int fneg();
    int iand();
    int ior();
    int ieq();
    int feq();
    int ineq();
    int fneq();
    int ilt();
    int flt();
    int igt();
    int fgt();
    int ibnot();
    int fmul();
    int i2f();
    int f2i();
};
class treeNode {
public:
    treeNode* left;
    treeNode* right;
    int type;
    float fdata;
    int idata;
    string key;
    int height;
public:
    treeNode() {
        this->idata = 0;
        this->type = 5;
        this->fdata = 0.0;
        this->key = ' ';
        this->left = nullptr;
        this->right = nullptr;
    }
    treeNode(int idata, int height, int type, float fdata, string key) {
        this->idata = idata;
        this->type = type;
        this->fdata = fdata;
        this->height = 1;
        this->key = key;
        this->left = nullptr;
        this->right = nullptr;
    }
    ~treeNode() {
        this->idata = 0;
        this->fdata = 0;
        this->type = 0;

        this->left = nullptr;
        this->right = nullptr;
    }
    friend class AVL;
};
class AVL {
private:
    int countTreeNode;
    int maxsize;
    bool found = false;
    int iprint;
    float fprint;
public:
    stack<string> parents;
    treeNode* root;
    AVL(int max) {
        this->maxsize = max;
        this->countTreeNode = 0;
        this->root = nullptr;
        this-> found = false;
        this->iprint = 0;
        this->fprint = 0;
    }
    ~AVL() {
        this->maxsize = 0;
        this->countTreeNode = 0;
        this->root = nullptr;
        this-> found = false;
        this->iprint = 0;
        this->fprint = 0;
        clear_stack();
    }
    
    treeNode* Insert(treeNode *root, int idata, int type, float fdata, string key);
    treeNode* RotateRight(treeNode* x);
    treeNode* RotateLeft(treeNode* y);
    int getBal(treeNode* tmp);
    int getHeight(treeNode* tmp);
    int stringcmp(string a, string b);
    int getTreeSize();
    void search2(treeNode* tmp, string a);
    treeNode* search(treeNode* tmp, string a);
    void search3(treeNode* tmp, string a);
    void clear_stack();
    friend class StackFrame;
};
#endif // !__STACK_FRAME_H__