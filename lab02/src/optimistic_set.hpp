#pragma once

#include "set.hpp"
#include "std_set.hpp"

#include <mutex>
#include <atomic>

#include <limits>
#include <iostream>

/// The node used for the linked list implementation of a set in the [`OptimisticSet`]
/// class.
struct OptimisticSetNode {
    // A01: You can add or remove fields as needed.
    int value;

    /// See this Stack Overflow thread for explonations of atomics in c++: <https://stackoverflow.com/questions/31978324/what-exactly-is-stdatomic>.
    /// See also the documentation for std::atomic: <https://en.cppreference.com/w/cpp/atomic/atomic>
    std::atomic<OptimisticSetNode*> next;

    std::mutex lock;

    /// Default constructor which sets value and next to 0 and NULL respectively.
    OptimisticSetNode() : value(0), next(NULL) {}
};

/// A set implementation using a linked list with optimistic syncronization.
class OptimisticSet: public Set {
private:
    // A01: You can add or remove fields as needed. Just having the `head`
    // pointer should be sufficient for this task
    std::atomic<OptimisticSetNode*> head;
public:
    OptimisticSet()
    {
        // create sentinel nodes: head = INT_MIN, tail = INT_MAX
        OptimisticSetNode* tail = new OptimisticSetNode();
        tail->value = std::numeric_limits<int>::max();
        tail->next.store(nullptr);

        OptimisticSetNode* h = new OptimisticSetNode();
        h->value = std::numeric_limits<int>::min();
        h->next.store(tail);

        head.store(h);
    }

    ~OptimisticSet() override {
        OptimisticSetNode* cur = head.load();
        while (cur) {
            OptimisticSetNode* nxt = cur->next.load();
            delete cur;
            cur = nxt;
        }
    }

private:
    bool validate(OptimisticSetNode* p, OptimisticSetNode* c) {
        OptimisticSetNode* node = head.load();
        while (node && node->value <= p->value) {
            if (node == p) {
                return (p->next.load() == c);
            }
            node = node->next.load();
        }
        return false;
    }

public:
    bool add(int elem) override {
        bool result = false;
        while (true) {
            OptimisticSetNode* p = head.load();
            OptimisticSetNode* c = p->next.load();

            while (c->value < elem) {
                p = c;
                c = c->next.load();
            }

            p->lock.lock();
            c->lock.lock();

            if (!validate(p, c)) {
                c->lock.unlock();
                p->lock.unlock();
                continue; 
            }

            if (c->value == elem) {
                // alredy in set
                result = false;
                c->lock.unlock();
                p->lock.unlock();
                return result;
            } else {
                OptimisticSetNode* node = new OptimisticSetNode();
                node->value = elem;
                node->next.store(c);
                p->next.store(node);
                result = true;
                c->lock.unlock();
                p->lock.unlock();
                return result;
            }
        }
    }

    bool rmv(int elem) override {
        bool result = false;
        while (true) {
            OptimisticSetNode* p = head.load();
            OptimisticSetNode* c = p->next.load();
            // find window
            while (c->value < elem) {
                p = c;
                c = c->next.load();
            }

            p->lock.lock();
            c->lock.lock();

            if (!validate(p, c)) {
                c->lock.unlock();
                p->lock.unlock();
                continue;
            }

            if (c->value != elem) {
                result = false;
                c->lock.unlock();
                p->lock.unlock();
                return result;
            } else {
                OptimisticSetNode* cnext = c->next.load();
                p->next.store(cnext);
                c->lock.unlock();
                p->lock.unlock();
                // delete c;
                result = true;
                return result;
            }
        }
    }

    bool ctn(int elem) override {
        bool result = false;
        OptimisticSetNode* cur = head.load()->next.load();
        while (cur && cur->value < elem) {
            cur = cur->next.load();
        }
        result = (cur && cur->value == elem);
        return result;
    }

    void print_state() override {
        // A01: Optionally, add code to print the state. This is useful for debugging,
        // but not part of the assignment
        std::cout << "OptimisticSet {...}";
    }
};

