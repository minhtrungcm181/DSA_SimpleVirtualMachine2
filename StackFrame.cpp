#include "StackFrame.h"
#include <iostream>
#include <fstream>
#include "errors.h"
#include "constants.h"



using namespace std;


StackFrame::StackFrame() : opStackMaxSize(OPERAND_STACK_MAX_SIZE), localVarSpaceSize(LOCAL_VARIABLE_SPACE_SIZE) {
    b = new operand(opStackMaxSize);
    c = new AVL(localVarSpaceSize);
}
StackFrame::~StackFrame()
{
    delete b;
    delete c;
}
string instructions(string s) {
    string delimiter = " ";
    string token = s.substr(0, s.find(delimiter));
    return token;
}

string value(string s) {
    string delimiter = " ";
    string token = s.substr(0, s.find(delimiter));
    s.erase(0, s.find(delimiter) + delimiter.length());
    string token2 = s.substr(0, s.find(delimiter));
    return token2;
}

int AVL::stringcmp(string a, string b) {
    int n = 0;
    if (a == b) return 0;
    else {
        if (a.length() < b.length()) n = b.length();
        else n = a.length();
        for (int i = 0; i < n; i++) {
            if (a[i] == '\n' && b[i] != '\n') { return 1; }
            else if (a[i] != '\n' && b[i] == '\n') { return 2; }
            else if (a[i] != b[i]) {
                if (a[i] < b[i]) return 1;
                else return 2;
            }

        }
    }
    // neu string a be hon string b return 1, con lai return 2
    return 5;
}
int AVL::getTreeSize() {
    return this->countTreeNode;
}
int AVL::getHeight(treeNode *tmp) {
    if (tmp == NULL)
        return 0;
    return tmp->height;
}
int AVL::getBal(treeNode *node) {
    if (node == NULL)
        return 0;
    return getHeight(node->left) - getHeight(node->right);
}

treeNode* AVL::RotateRight(treeNode* tmp) {
    treeNode* x = tmp->left;
    

    // Perform rotation
    x->right = tmp;
    tmp->left = x;

    // Update heights
    int n = 0; 
    if (getHeight(tmp->left) > getHeight(tmp->right)) { n = getHeight(tmp->left); }
    else n = getHeight(tmp->right);
    tmp->height = n + 1;

    if (getHeight(x->left) > getHeight(x->right)) { n = getHeight(x->left); }
    else n = getHeight(x->right);
    x->height = n + 1;

    // Return new root
    return x;
}
treeNode* AVL::RotateLeft(treeNode* tmp) {
    treeNode* y = tmp->right;
    

    // Perform rotation
    y->left = tmp;
    tmp->right = y;

    // Update heights
    int n = 0;
    if (getHeight(tmp->left) > getHeight(tmp->right)) { n = getHeight(tmp->left); }
    else n = getHeight(tmp->right);
    tmp->height = n + 1;

    if (getHeight(y->left) > getHeight(y->right)) { n = getHeight(y->left); }
    else n = getHeight(y->right);
    y->height = n + 1;
    
    // Return new root
    return y;
}
treeNode* AVL::Insert(treeNode* node, int idata, int type, float fdata, string key) {
    if (node == nullptr && this->getTreeSize() == 0)
    {
        treeNode* tmp = new treeNode(idata, 0, type, fdata, key);
        this->root = tmp; 
        this->countTreeNode++;
        return this->root;
    }
    else if (node == nullptr) {
        treeNode* tmp = new treeNode(idata, 1, type, fdata, key);
        node = tmp; 
        this->countTreeNode++;
        return node;
    }
    else if (this->stringcmp(key, node->key) == 0) {
        node->idata = idata;
        node->fdata = fdata;
        node->type = type;
    }
    else if (this->stringcmp(key, node->key) == 2) {
        node->right = Insert(node->right, idata, type, fdata, key);
    }
    else if (this->stringcmp(key, node->key) == 1) {
        node->left = Insert(node->left, idata, type, fdata, key);
    }
    int n = 0;
    if (this->getHeight(node->left) > this->getHeight(node->right)) { n = this->getHeight(node->left); }
    else n = this->getHeight(node->right);
    node->height = 1 + n;
    int balance = getBal(node);

    if (balance > 1 && stringcmp(key, node->left->key) == 1)
        this->root = RotateRight(node);
    if (balance < -1 && stringcmp(key, node->right->key) == 2)
        this->root = RotateLeft(node); 
    if (balance > 1 && stringcmp(key, node->left->key) == 2)
    {
        node->left = RotateLeft(node->left);
        this ->root = RotateRight(node);
    }
    if (balance < -1 && stringcmp(key, node->right->key) == 1)
    {
        node->right = RotateRight(node->right);
        this->root = RotateLeft(node);
    }
    
    return node;
}
treeNode* AVL::search(treeNode *tmp, string key) {
    treeNode* tmp1 = new treeNode();
    if (tmp->key == key) {
        return tmp;
    }
    else if (this->stringcmp(key, tmp->key) == 1 && tmp->left != nullptr) { return search(tmp->left, key); }
    else if (this->stringcmp(key, tmp->key) == 2 && tmp->right != nullptr) { return search(tmp->right, key); }
    else return tmp1;
}
void AVL::search3(treeNode* tmp, string key)
{
    if (tmp->key == key) {
        if (tmp->type == 1) { 
            this->iprint = 999;
            this->fprint = tmp->fdata; }
        else if (tmp->type == 0)
        {
            this->fprint = 999;
            this->iprint = tmp->idata;
        }
        return;
    }
    else if (this->stringcmp(key, tmp->key) == 1 && tmp->left != nullptr) { search3(tmp->left, key); }
    else if (this->stringcmp(key, tmp->key) == 2 && tmp->right != nullptr) { search3(tmp->right, key); }
}
void AVL::search2(treeNode* tmp, string key) {
    if (tmp->key == key) {
        return;
    }
    else if (this->stringcmp(key, tmp->key) == 1 && tmp->left != nullptr) { parents.push(tmp->key); search2(tmp->left, key); }
    else if (this->stringcmp(key, tmp->key) == 2 && tmp->right != nullptr) { parents.push(tmp->key); search2(tmp->right, key); }
    else parents.push("nothing"); 
    return;
}
void AVL::clear_stack() {
    while (parents.empty() == false) {parents.pop();}
}
void operand::push(Node* tmp) {
    if (full() == true) { return; }
    else if (empty() == true) {
        head = tail = new Node(tmp);
        countNode++;
    }
    else {
        tail->next = new Node(tmp);
        tail = tail->next;
        countNode++;
    }
}
float operand::top() {
    if (tail->type == 1) { return tail->fvalue; }
    else if (tail->type == 0)
        return tail->ivalue;
    else return 0;
}
void operand::pop() {
    if (empty() == true) { return; }
    else {
        Node* tmp = new Node();
        tmp = head;
        if (countNode == 1) { head = nullptr; delete head; countNode--; }
        else if (countNode == 2) { tail = head; countNode--; }
        else {
            while (tmp->next != tail) {
                tmp = tmp->next;
            }
            tmp->next = nullptr;
            tail = nullptr;
            delete tail;
            tail = tmp;
            countNode--;
        }

    }
}
bool operand::full() {
    if (countNode >= OPERAND_STACK_MAX_SIZE/2) {
        return true;
    }
    else {
        return false;
    }
}
bool operand::empty() {
    if (countNode == 0) {
        return true;
    }
    else {
        return false;
    }
}
int operand::getSize() {
    return countNode;
}
int operand::checktype() {
    return tail->type;
}
int operand::iadd() {
    int a, b, c;
    if (getSize() < 2) return 2;
    a = top(); c = checktype();
    if (c == 1) { return 0; }
    pop();
    b = top(); c = checktype();
    if (c == 1) { return 0; }
    pop();
    c = a + b;
    Node* tmp = new Node();
    tmp->ivalue = c;
    tmp->type = 0;
    tmp->fvalue = 0;
    push(tmp);
    return 1;
}
int operand::fadd() {
    float a, b, c;
    if (getSize() < 2) return 2;
    a = top();
    pop();
    b = top();
    pop();
    c = a + b;
    Node* tmp = new Node();
    tmp->fvalue = c;
    tmp->ivalue = 0;
    tmp->type = 1;
    push(tmp);
    return 1;
}
int operand::isub() {
    int a, b, c;
    if (getSize() < 2) return 2;
    a = top(); c = checktype();
    if (c == 1) { return 0; }
    pop();
    b = top(); c = checktype();
    if (c == 1) { return 0; }
    pop();
    c = b - a;
    Node* tmp = new Node();
    tmp->fvalue = 0;
    tmp->ivalue = c;
    tmp->type = 0;
    push(tmp);
    return 1;
}
int operand::fsub() {
    float a, b, c;
    if (getSize() < 2) return 2;
    a = top();
    pop();
    b = top();
    pop();
    c = b - a;
    Node* tmp = new Node();
    tmp->fvalue = c;
    tmp->type = 1;
    tmp->ivalue = 0;
    push(tmp);
    return 1;
}
int operand::imul() {
    int a, b, c;
    if (getSize() < 2) return 2;
    a = top(); c = checktype();
    if (c == 1) { return 0; }
    pop();
    b = top();
    c = checktype();
    if (c == 1) { return 0; }
    pop();
    c = a * b;
    Node* tmp = new Node();
    tmp->ivalue = c;
    tmp->type = 0;
    push(tmp);
    return 1;
}
int operand::fmul() {
    float a, b, c;
    if (getSize() < 2) return 2;
    a = top();
    pop();
    b = top();
    pop();
    c = a * b;
    Node* tmp = new Node();
    tmp->fvalue = c;
    tmp->ivalue = 0;
    tmp->type = 1;
    push(tmp);
    return 1;
}
int operand::idiv() {
    int a, b, c;
    if (getSize() < 2) return 3;
    a = top(); c = checktype();
    if (c == 1) { return 0; }
    pop();
    b = top(); c = checktype();
    if (c == 1) { return 0; }
    if (a == 0) { return 2; }
    pop();
    c = b / a;
    Node* tmp = new Node();
    tmp->fvalue = 0;
    tmp->ivalue = c;
    tmp->type = 0;
    push(tmp);
    return 1;
}
int operand::fdiv() {
    float a, b, c;
    if (getSize() < 2) return 3;
    a = top();  if (a == 0) { return 2; }
    pop();
    b = top();
    pop();
    c = b / a;
    Node* tmp = new Node();
    tmp->fvalue = c;
    tmp->ivalue = 0;
    tmp->type = 1;
    push(tmp);
    return 1;
}
int operand::irem() {
    int a, b, c;
    if (getSize() < 2) return 3;
    a = top();
    if (checktype() == 1) return 0;
    pop();
    b = top(); if (checktype() == 1) return 0;
    pop(); if (a == 0) { return 2; }
    c = a - (a / b) * b;
    Node* tmp = new Node();
    tmp->fvalue = c;
    tmp->ivalue = 0;
    tmp->type = 0;
    push(tmp);
    return 1;
}
int operand::ineg() {
    int a;
    if (getSize() < 1) return 2;
    a = top(); if (checktype() == 1) { return 0; }
    pop();
    Node* tmp = new Node();
    tmp->fvalue = 0;
    tmp->ivalue = -a;
    tmp->type = 0;
    push(tmp);
    return 1;
}

int operand::fneg() {
    float a;
    if (getSize() < 1) return 2;
    a = top();
    pop();
    Node* tmp = new Node();
    tmp->fvalue = -a;
    tmp->ivalue = 0;
    tmp->type = 1;
    push(tmp);
    return 1;
}
int operand::iand() {
    int a, b;
    if (getSize() < 2) return 2;
    a = top(); if (checktype() == 1) { return 0; }
    pop();
    b = top(); if (checktype() == 1) { return 0; }
    pop();
    int c = a & b;
    Node* tmp = new Node();
    tmp->fvalue = 0;
    tmp->ivalue = c;
    tmp->type = 0;
    push(tmp);
    return 1;
}
int operand::ior() {
    int a, b;
    if (getSize() < 2) return 2;
    a = top(); if (checktype() == 1) return 0;
    pop();
    b = top(); if (checktype() == 1) return 0;
    pop();
    int c = a | b;
    Node* tmp = new Node();
    tmp->fvalue = 0;
    tmp->ivalue = c;
    tmp->type = 0;
    push(tmp);
    return 1;
}
int operand::ieq() {
    int a, b;
    if (getSize() < 2) return 2;
    a = top(); if (checktype() == 1) return 0;
    pop();
    b = top(); if (checktype() == 1) return 0;
    pop();
    int c = 0;
    if (a == b) { c = 1; }
    Node* tmp = new Node();
    tmp->ivalue = c;
    tmp->fvalue = 0;
    tmp->type = 0;
    push(tmp);
    return 1;
}
int operand::feq() {
    float a, b;
    if (getSize() < 2) return 2;
    a = top();
    pop();
    b = top();
    pop();
    int c = 0;
    if (a == b) { c = 1; }
    Node* tmp = new Node();
    tmp->ivalue = c;
    tmp->fvalue = 0;
    tmp->type = 0;
    push(tmp);
    return 1;
}
int operand::ineq() {
    int a, b;
    if (getSize() < 2) return 2;
    a = top(); if (checktype() == 1) return 0;
    pop();
    b = top(); if (checktype() == 1) return 0;
    pop();
    int c = 0;
    if (a != b) { c = 1; }
    Node* tmp = new Node();
    tmp->ivalue = c;
    tmp->fvalue = 0;
    tmp->type = 0;
    push(tmp);
    return 1;
}
int operand::fneq() {
    float a, b;
    if (getSize() < 2) return 2;
    a = top();
    pop();
    b = top();
    pop();
    int c = 0;
    if (a != b) { c = 1; }
    Node* tmp = new Node();
    tmp->ivalue = c;
    tmp->fvalue = 0;
    tmp->type = 0;
    push(tmp);
    return 1;
}
int operand::ilt() {
    int a, b;
    if (getSize() < 2) return 2;
    a = top(); if (checktype() == 1) return 0;
    pop();
    b = top(); if (checktype() == 1) return 0;
    pop();
    int c = 0;
    if (b < a) { c = 1; }
    Node* tmp = new Node();
    tmp->ivalue = c;
    tmp->fvalue = 0;
    tmp->type = 0;
    push(tmp);
    return 1;
}
int operand::flt() {
    float a, b;
    if (getSize() < 2) return 2;
    a = top();
    pop();
    b = top();
    pop();
    int c = 0;
    if (b < a) { c = 1; }
    Node* tmp = new Node();
    tmp->ivalue = c;
    tmp->fvalue = 0;
    tmp->type = 0;
    push(tmp);
    return 1;
}
int operand::igt() {
    int a, b;
    if (getSize() < 2) return 2;
    a = top(); if (checktype() == 1) return 0;
    pop();
    b = top(); if (checktype() == 1) return 0;
    pop();
    int c = 0;
    if (b > a) { c = 1; }
    Node* tmp = new Node();
    tmp->ivalue = c;
    tmp->fvalue = 0;
    tmp->type = 0;
    push(tmp);
    return 1;
}
int operand::fgt()
{
    float a, b;
    if (getSize() < 2) return 2;
    a = top();
    pop();
    b = top();
    pop();
    int c = 0;
    if (b > a) { c = 1; }
    Node* tmp = new Node();
    tmp->ivalue = c;
    tmp->fvalue = 0;
    tmp->type = 0;
    push(tmp);
    return 1;
}

int operand::ibnot() {
    int a;
    if (getSize() < 1) return 2;
    a = top(); if (checktype() == 1)return 0;
    pop();
    int c = 0;
    if (a != 0) c = 0;
    else c = 1;
    Node* tmp = new Node();
    tmp->ivalue = c;
    tmp->fvalue = 0;
    tmp->type = 0;
    push(tmp);
    return 1;
}

int operand::iconst(string val) {
    int s = stoi(val);
    if (full() == true) return 0;
    Node* tmp = new Node;
    tmp->ivalue = s;
    tmp->fvalue = 0;
    tmp->type = 0;
    push(tmp);
    return 1;
}
int operand::fconst(string s) {
    float number = stof(value(s));
    if (full() == true) return 0;
    Node* tmp = new Node();
    tmp->ivalue = 0;
    tmp->fvalue = number;
    tmp->type = 1;
    push(tmp);
    return 1;
}
int operand::i2f() {
    int a;
    if (getSize() < 1) return 2;
    a = top(); if (checktype() == 1) return 0;
    pop();
    float c = float(a);
    Node* tmp = new Node();
    tmp->ivalue = 0;
    tmp->fvalue = c;
    tmp->type = 1;
    push(tmp);
    return 1;
}
int operand::f2i() {
    float a;
    if (getSize() < 1) return 2;
    a = top(); if (checktype() == 0) return 0;
    pop();
    int c = int(a);
    Node* tmp = new Node();
    tmp->ivalue = c;
    tmp->fvalue = 0;
    tmp->type = 0;
    push(tmp);
    return 1;
}
void StackFrame::doing(string* member, int n) {
    for (int i = 0; i < n; i++) {
        string s = member[i];
        string ins = instructions(s);
        string val = value(s);


        if (ins == "iadd") { int a = b->iadd(); if (a == 0) { throw TypeMisMatch(i + 1); } if (a == 2) { throw StackEmpty(i + 1); } }
        else if (ins == "fadd") { int a = b->fadd(); if (a == 2) { throw StackEmpty(i + 1); } }
        else if (ins == "isub") { int a = b->isub(); if (a == 0) { throw TypeMisMatch(i + 1); } if (a == 2) { throw StackEmpty(i + 1); } }
        else if (ins == "fsub") { int a = b->fsub(); if (a == 2) { throw StackEmpty(i + 1); } }
        else if (ins == "imul") { int a = b->imul(); if (a == 0) { throw TypeMisMatch(i + 1); }if (a == 2) { throw StackEmpty(i + 1); } }
        else if (ins == "fmul") { int a = b->fmul(); if (a == 2) { throw StackEmpty(i + 1); } }
        else if (ins == "idiv") {
            int a = b->idiv();
            if (a == 0) throw TypeMisMatch(i + 1);
            else if (a == 3) { throw StackEmpty(i + 1); }
            else if (a == 2) throw DivideByZero(i + 1);
        }
        else if (ins == "fdiv") {
            int a = b->fdiv();
            if (a == 2) throw DivideByZero(i + 1);
            else if (a == 3) { throw StackEmpty(i + 1); }
        }
        else if (ins == "irem") {
            int a = b->irem();
            if (a == 2) throw DivideByZero(i + 1);
            else if (a == 3) { throw StackEmpty(i + 1); }
            else if (a == 0) throw TypeMisMatch(i + 1);
        }
        else if (ins == "ineg") {
            int a = b->ineg();
            if (a == 2) { throw StackEmpty(i + 1); }
            if (a == 0) throw TypeMisMatch(i + 1);
        }
        else if (ins == "fneg") { int a = b->fneg(); if (a == 2) { throw StackEmpty(i + 1); } }
        else if (ins == "iand") { int a = b->iand(); if (a == 0) throw TypeMisMatch(i + 1); if (a == 2) { throw StackEmpty(i + 1); } }
        else if (ins == "ior") { int a = b->ior(); if (a == 0)throw TypeMisMatch(i + 1); if (a == 2) { throw StackEmpty(i + 1); } }
        else if (ins == "ieq") { int a = b->ieq(); if (a == 0)throw TypeMisMatch(i + 1); if (a == 2) { throw StackEmpty(i + 1); } }
        else if (ins == "feq") { int a = b->feq(); if (a == 2) { throw StackEmpty(i + 1); } }
        else if (ins == "ineq") { int a = b->ineq(); if (a == 0)throw TypeMisMatch(i + 1); if (a == 2) { throw StackEmpty(i + 1); } }
        else if (ins == "fneq") {
            int a = b->fneq(); if (a == 2) { throw StackEmpty(i + 1); }
        }
        else if (ins == "ilt") { int a = b->ilt(); if (a == 0)throw TypeMisMatch(i + 1); if (a == 2) { throw StackEmpty(i + 1); } }
        else if (ins == "flt") { int a = b->flt(); if (a == 2) { throw StackEmpty(i + 1); } }
        else if (ins == "igt") { int a = b->igt(); if (a == 0)throw TypeMisMatch(i + 1); if (a == 2) { throw StackEmpty(i + 1); } }
        else if (ins == "fgt") { int a = b->fgt(); if (a == 2) { throw StackEmpty(i + 1); } }
        else if (ins == "ibnot") { int a = b->ibnot(); if (a == 0) throw TypeMisMatch(i + 1); if (a == 2) { throw StackEmpty(i + 1); } }
        else if (ins == "iconst") { int a = b->iconst(val); if (a == 0) throw StackFull(i + 1); }
        else if (ins == "fconst") { int a = b->fconst(val); if (a == 0) throw StackFull(i + 1); }
        else if (ins == "top") {
            if (b->empty()) throw StackEmpty(i + 1);
            else cout << b->top() << endl;
        }
        else if (ins == "istore") {
            if (b->empty() == true) throw StackEmpty(i + 1);
            else {
                int d = b->checktype();
                if (d == 1) { throw TypeMisMatch(i + 1); }
                else {
                    d = b->top();
                    if (c->getTreeSize() == c->maxsize) throw LocalSpaceFull(i + 1);
                    c->Insert(c->root, d, 0, 0.0, val);
                    b->pop();
                }
            }
        }
        else if (ins == "fstore") {
            if (b->empty() == true) throw StackEmpty(i + 1);
            else {
                int d = b->checktype();
                if (d == 0) { throw TypeMisMatch(i + 1); }
                else {
                    float e = 0.0;
                    e = b->top();
                    if (c->getTreeSize() == c->maxsize) throw LocalSpaceFull(i + 1);
                    c->Insert(c->root, 0, 1, e, val);
                    b->pop();
                }
            }
        }
        else if (ins == "iload") {
            if (c->getTreeSize() == 0) throw UndefinedVariable(i + 1); 
            else {
                treeNode* f = c->search(c->root, val);
                if (f->type == 5) { throw UndefinedVariable(i + 1); }
                else if (f->type == 1) { throw TypeMisMatch(i + 1); }
                else if (f->type == 0) { int g = f->idata; string s = to_string(g); b->iconst(s); }
            }
        }
        else if (ins == "fload") {
            if (c->getTreeSize() == 0) throw UndefinedVariable(i + 1);
            else {
                treeNode* f = c->search(c->root, val);
                if (f->type == 5) { throw UndefinedVariable(i + 1); }
                else if (f->type == 0) { throw TypeMisMatch(i + 1); }
                else if (f->type == 1) { float g = f->fdata; string s = to_string(g); b->fconst(s); }
            }
        }
        else if (ins == "i2f") { int a = b->i2f(); if (a == 0) throw TypeMisMatch(i + 1); if (a == 2) throw StackEmpty(i + 1); }
        else if (ins == "f2i") { int a = b->f2i(); if (a == 0) throw TypeMisMatch(i + 1); if (a == 2) throw StackEmpty(i + 1); }
        else if (ins == "top") {
            if (b->empty()) throw StackEmpty(i + 1);
            else cout << b->top() << endl;
        }
        else if (ins == "val") {
            if (c->getTreeSize() == 0) throw UndefinedVariable(i + 1);
            else {
                c->search3(c->root, val);
                if (c->iprint == 999) { cout << c->fprint << endl; }
                else if (c->fprint == 999) { cout << c->iprint << endl; }
                else if (c->fprint == 0 && c->iprint == 0) throw UndefinedVariable(i + 1);
                c->iprint = 0;
                c->fprint = 0;
            }
        }
        else if (ins == "par") {
        if (c->getTreeSize() == 0) throw UndefinedVariable(i + 1);
        else {
            c->search2(c->root, val);
            if (c->parents.size() == 0) { cout << "null" << endl; }
            else {
                string s = c->parents.top();
                if (s == "nothing") throw UndefinedVariable(i + 1);
                else if (s != "nothing" ) { cout << s << endl; }
            }
        }
        c->clear_stack();
        
}

    }
}
void StackFrame::run(string filename) {
    string* member;
    int n = 0;
    ifstream ifs;
    string tmp = "";
    ifs.open(filename);
    while (!ifs.eof()) {
        getline(ifs, tmp);
        n++;
    }
    ifs.close();
    member = new string[n];
    n = 0;
    ifs.open(filename);
    while (!ifs.eof()) {
        getline(ifs, tmp);
        member[n] = tmp;
        n++;
    }
    ifs.close();
    doing(member, n);
    delete[] member;
 }