#pragma once

#include "set.hpp"
#include "std_set.hpp"

#include <mutex>

// A03: Copy your `FineSet` implementation from Lab 01 into this file and
// remove all references to the monitor. We want to benchmark the data
// structure and monitoring the performed operation would influence the
// results.

struct FineSetNode {
    // A04: You can add or remove fields as needed.
    int value;
    FineSetNode* next;
    std::mutex lock;
};

class FineSet: public Set {
private:
    FineSetNode* head;
public:
    FineSet()
    {
        // A04: Initiate the internal state
        FineSetNode* tail = new FineSetNode();
        tail->value = std::numeric_limits<int>::max();
        tail->next = nullptr;
        FineSetNode* h = new FineSetNode();
        h->value = std::numeric_limits<int>::min();
        h->next = tail;
        head = h;
    }
    ~FineSet() override {
        // A04: Cleanup any memory that was allocated
        FineSetNode* cur = head;
        while (cur) {
            FineSetNode* nxt = cur->next;
            delete cur;
            cur = nxt;
        }
    }

    bool add(int elem) override {
        bool result = false;
        // A04: Add code to insert the element into the set and update `result`.
        //      Also make sure, to insert the event inside the locked region of
        //      the linearization point.
        FineSetNode* prev = head;
        prev->lock.lock();
        FineSetNode* cur = prev->next;
        cur->lock.lock();

        while (cur->value < elem) {
            prev->lock.unlock();
            prev = cur;
            cur = cur->next;
            cur->lock.lock();
        } // iter until cur value >= elem

        if (cur->value == elem) {
            result = false;
        } else {
            FineSetNode* node = new FineSetNode();
            node->value = elem;
            node->next = cur;
            prev->next = node;
            result = true;
        }

        cur->lock.unlock();
        prev->lock.unlock();
        return result;
    }

    bool rmv(int elem) override {
        bool result = false;
        // A04: Add code to remove the element from the set and update `result`.
        //      Also make sure, to insert the event inside the locked region of
        //      the linearization point.
        FineSetNode* prev = head;
        prev->lock.lock();
        FineSetNode* cur = prev->next;
        cur->lock.lock();

        while (cur->value < elem) {
            prev->lock.unlock();
            prev = cur;
            cur = cur->next;
            cur->lock.lock();
        } // iter until cur value >= elem

        if (cur->value == elem) {
            prev->next = cur->next;
            result = true;
            
            cur->lock.unlock();
            prev->lock.unlock();
            delete cur;
            return result;
        } else {
            result = false;
            cur->lock.unlock();
            prev->lock.unlock();
            return result;
        }
    }

    bool ctn(int elem) override {
        bool result = false;
        // A04: Add code to check if the element is inside the set and update `result`.
        //      Also make sure, to insert the event inside the locked region of
        //      the linearization point.
        FineSetNode* prev = head;
        prev->lock.lock();
        FineSetNode* cur = prev->next;
        cur->lock.lock();

        while (cur->value < elem) {
            prev->lock.unlock();
            prev = cur;
            cur = cur->next;
            cur->lock.lock();
        }

        result = (cur->value == elem);

        cur->lock.unlock();
        prev->lock.unlock();
        return result;
    }

    void print_state() override {
        std::cout << "FineSet {...}";
    }
};

