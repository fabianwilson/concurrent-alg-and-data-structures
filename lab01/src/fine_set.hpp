#pragma once

#include "set.hpp"
#include "std_set.hpp"
#include <limits>

#include <mutex>

/// The node used for the linked list implementation of a set in the [`FineSet`]
/// class. This struct is used for task 4.
struct FineSetNode {
    // A04: You can add or remove fields as needed.
    int value;
    FineSetNode* next;
    std::mutex lock;
};

/// A set implementation using a linked list with fine grained locking.
class FineSet: public Set {
private:
    // A04: You can add or remove fields as needed. Just having the `head`
    // pointer should be sufficient for task 4
    FineSetNode* head;
    EventMonitor<FineSet, StdSet, SetOperator>* monitor;
public:
    FineSet(EventMonitor<FineSet, StdSet, SetOperator>* monitor) :
        monitor(monitor)
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

        this->monitor->add(SetEvent(SetOperator::Add, elem, result));

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
            this->monitor->add(SetEvent(SetOperator::Remove, elem, result));
            
            cur->lock.unlock();
            prev->lock.unlock();
            delete cur;
            return result;
        } else {
            result = false;
            this->monitor->add(SetEvent(SetOperator::Remove, elem, result));
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
        this->monitor->add(SetEvent(SetOperator::Contains, elem, result));

        cur->lock.unlock();
        prev->lock.unlock();
        return result;
    }

    void print_state() override {
        // A04: Optionally, add code to print the state. This is useful for debugging,
        // but not part of the assignment
        std::cout << "FineSet {";
        for (FineSetNode* cur = head->next; cur != nullptr && cur->next != nullptr; cur = cur->next) {
            if (cur->value == std::numeric_limits<int>::max()) break;
            std::cout << cur->value;
            if (cur->next && cur->next->value != std::numeric_limits<int>::max()) std::cout << ", ";
        }
        std::cout << "}";
    }
};

