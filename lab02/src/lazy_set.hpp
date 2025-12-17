#pragma once

#include "set.hpp"
#include "std_set.hpp"

#include <mutex>
#include <atomic>
#include <limits>
#include <iostream>

/// The node used for the linked list implementation of a set in the [`LazySet`] class.
struct LazySetNode {
    // A02: You can add or remove fields as needed.
    int value;

    /// See this Stack Overflow thread for explonations of atomics in c++: <https://stackoverflow.com/questions/31978324/what-exactly-is-stdatomic>.
    /// See also the documentation for std::atomic: <https://en.cppreference.com/w/cpp/atomic/atomic>
    std::atomic<bool> mark;
    std::atomic<LazySetNode*> next;

    std::mutex lock;

    /// Default constructor which sets value and next to 0 and NULL respectively.
    LazySetNode() : value(0), mark(false), next(NULL) {}
};

/// A set implementation using a linked list with optimistic syncronization.
class LazySet: public Set {
private:
    std::atomic<LazySetNode*> head;
public:
    LazySet()
    {
        LazySetNode* tail = new LazySetNode();
        tail->value = std::numeric_limits<int>::max();
        tail->next.store(nullptr);
        tail->mark.store(false);

        LazySetNode* h = new LazySetNode();
        h->value = std::numeric_limits<int>::min();
        h->next.store(tail);
        h->mark.store(false);

        head.store(h);
    }

    ~LazySet() override {
        LazySetNode* cur = head.load();
        while (cur) {
            LazySetNode* nxt = cur->next.load();
            delete cur;
            cur = nxt;
        }
    }

private:
    LazySetNode* locate(int value) {
        LazySetNode* cur = head.load();
        LazySetNode* next = cur->next.load();
        while (next && next->value < value) {
            cur = next;
            next = cur->next.load();
        }
        return cur;
    }

public:
    bool add(int elem) override {
        bool result = false;
        while (true) {
            LazySetNode* p = locate(elem);
            LazySetNode* c = p->next.load();

            p->lock.lock();
            c->lock.lock();

            if (p->mark.load() || c->mark.load() || p->next.load() != c) {
                c->lock.unlock();
                p->lock.unlock();
                continue;
            }

            if (c->value == elem) {
                result = false;
                c->lock.unlock();
                p->lock.unlock();
                return result;
            } else {
                LazySetNode* node = new LazySetNode();
                node->value = elem;
                node->mark.store(false);
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
            LazySetNode* p = locate(elem);
            LazySetNode* c = p->next.load();

            p->lock.lock();
            c->lock.lock();

            if (p->mark.load() || c->mark.load() || p->next.load() != c) {
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
                
                c->mark.store(true);
                
                LazySetNode* cnext = c->next.load();
                p->next.store(cnext);
                c->lock.unlock();
                p->lock.unlock();
                result = true;
                return result;
            }
        }
    }

    bool ctn(int elem) override {
        bool result = false;
        LazySetNode* cur = head.load()->next.load();
        while (cur && cur->value < elem) {
            cur = cur->next.load();
        }
        result = (cur && cur->value == elem && !cur->mark.load());
        return result;
    }

    void print_state() override {
        // A02: Optionally, add code to print the state. This is useful for debugging,
        // but not part of the assignment
        std::cout << "LazySet {...}";
    }
};

