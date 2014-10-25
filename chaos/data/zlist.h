#ifndef ZLIST_H
#define ZLIST_H

#include "ztypes.h"
#include "yindexedaccess.h"

#include "zlog.h"

namespace LibChaos {

// Implemented as a circular doubly-linked list
template <typename T> class ZList : public YIndexedAccess<T> {
private:
    struct Node {
        Node *prev;
        Node *next;
        T *data;
    };

public:
    ZList() : _size(0), head(nullptr){}

    ~ZList(){
        if(_size){
            head->prev->next = nullptr; // Break circular link
            Node *current = head;
            Node *next;
            do { // Delete all nodes
                delete current->data;
                next = current->next;
                delete current;
                current = next;
            } while(current != nullptr);
        }
    }

    void pushFront(const T &data){
        Node *node = newNode(data);
        if(head == nullptr){
            head = node;
            node->prev = head;
            node->next = head;
        } else {
            node->prev = head->prev;
            node->next = head;
            head = node;
            head->next->prev = head;
            head->prev->next = head;
        }
        ++_size;
    }
    void pushBack(const T &data){
        Node *node = newNode(data);
        if(head == nullptr){
            head = node;
            node->prev = head;
            node->next = head;
        } else {
            node->prev = head->prev;
            node->next = head;
            head->prev = node;
            head->prev->prev->next = node;
        }
        ++_size;
    }
    inline void push(const T &data){ pushBack(data); }

    void debug() const {
        ZString str;
        Node *current = head;
        for(int i = 0; i < _size; ++i){
            str += ZString::ItoS((zu64)current->prev, 16) + " ";
            str += ZString::ItoS((zu64)current, 16);
            str += " " + ZString::ItoS((zu64)current->next, 16);
            str += ", ";
            current = current->next;
        }
        LOG(str);
    }

    T &operator[](zu64 index){
        return *getNode(index)->data;
    }
    const T &operator[](zu64 index) const {
        return *getNode(index)->data;
    }

    zu64 size() const {
        return _size;
    }

private:
   static Node *newNode(const T &data){
       Node *node = new Node;
       node->data = new T(data);
       return node;
    }

    Node *getNode(zu64 index) const {
        Node *current = head;
        for(zu64 i = 0; i < index; ++i){
            current = current->next;
        }
        return current;
    }

private:
    zu64 _size;
    Node *head;
};

}

#endif // ZLIST_H
