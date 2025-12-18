#pragma once

#include "set.hpp"
#include "std_set.hpp"

#include <mutex>

/// The node used for the linked list implementation of a multiset in the
/// [`FineMultiset`] class.
struct FineMultisetNode
{
    // A08: You can add or remove fields as needed.
    int value;
    FineMultisetNode *next;
    std::mutex lock;
};

/// A multiset implementation using a linked list with fine grained locking.
class FineMultiset : public Multiset
{
private:
    // A08: You can add or remove fields as needed.
    FineMultisetNode *head;
    EventMonitor<FineMultiset, StdMultiset, MultisetOperator> *monitor;

public:
    FineMultiset(EventMonitor<FineMultiset, StdMultiset, MultisetOperator> *monitor) : monitor(monitor)
    {
        // create sentinel nodes: head = INT_MIN, tail = INT_MAX
        FineMultisetNode *tail = new FineMultisetNode();
        tail->value = std::numeric_limits<int>::max();
        tail->next = nullptr;

        FineMultisetNode *h = new FineMultisetNode();
        h->value = std::numeric_limits<int>::min();
        h->next = tail;
        head = h;
    }

    ~FineMultiset() override
    {
        // cleanup
        FineMultisetNode *cur = head;
        while (cur)
        {
            FineMultisetNode *nxt = cur->next;
            delete cur;
            cur = nxt;
        }
    }

    int add(int elem) override
    {
        int result = true;
        // A08: Add code to insert the element into the set.
        //      Make sure, to insert the event inside the locked region of
        //      the linearization point.
        FineMultisetNode *prev = head;
        prev->lock.lock();
        FineMultisetNode *cur = prev->next;
        cur->lock.lock();

        while (cur->value < elem)
        {
            prev->lock.unlock();
            prev = cur;
            cur = cur->next;
            cur->lock.lock();
        } // iter until cur value >= elem

        FineMultisetNode *node = new FineMultisetNode();
        node->value = elem;
        node->next = cur;
        prev->next = node;
        result = true;

        this->monitor->add(MultisetEvent(MultisetOperator::MSetAdd, elem, result));
        
        cur->lock.unlock();
        prev->lock.unlock();
        return result;
    }

    int rmv(int elem) override
    {
        int result = false;
        // A08: Add code to remove the element from the set and update `result`.
        //      Also make sure, to insert the event inside the locked region of
        //      the linearization point.
        FineMultisetNode *prev = head;
        prev->lock.lock();
        FineMultisetNode *cur = prev->next;
        cur->lock.lock();

        while (cur->value < elem)
        {
            prev->lock.unlock();
            prev = cur;
            cur = cur->next;
            cur->lock.lock();
        }

        if (cur->value != elem)
        {
            result = false; // not found
        }
        else
        {
            prev->next = cur->next; // remove first occurence
            result = true;
        }

        this->monitor->add(MultisetEvent(MultisetOperator::MSetRemove, elem, result));

        cur->lock.unlock();
        prev->lock.unlock();
        return result;
    }

    int ctn(int elem) override
    {
        int result = 0;
        // A08: Add code to count how often elem is inside the set and update `result`.
        //      Also make sure, to insert the event inside the locked region of
        //      the linearization point.
        //
        //      There are different ways to implement a multiset ADT. The
        //      skeleton code provides `monitor->add()`, `monitor->reserve()`,
        //      and `event->complete()` functions for this purpose. One can
        //      use only `monitor->add() or a combination of `monitor->reserve()`
        //      and `event->complete()` depending on their multiset
        //      implementation. Go to `monitoring.hpp` and see the descriptions
        //      of these function.
        FineMultisetNode *prev = head;
        prev->lock.lock();
        FineMultisetNode *cur = prev->next;
        cur->lock.lock();

        while (cur->value < elem)
        {
            prev->lock.unlock();
            prev = cur;
            cur = cur->next;
            cur->lock.lock();
        }
        
        Event<MultisetOperator> *event = this->monitor->reserve(MultisetEvent(MultisetOperator::MSetCount, elem));

        prev->lock.unlock();


        while (cur->value == elem)
        {
            result++;
            FineMultisetNode *tmp = cur;
            cur = cur->next;
            cur->lock.lock();  
            tmp->lock.unlock();
        }

        //this->monitor->add(MultisetEvent(MultisetOperator::MSetCount, elem, result));
        
        event->complete(result);
        cur->lock.unlock();
        return result;
    }

    void print_state() override
    {
        // A08: Optionally, add code to print the state. This is useful for debugging,
        // but not part of the assignment
        std::cout << "FineMultiset {";
        for (FineMultisetNode *cur = head->next; cur != nullptr && cur->next != nullptr; cur = cur->next)
        {
            if (cur->value == std::numeric_limits<int>::max())
                break;
            std::cout << cur->value;
            if (cur->next && cur->next->value != std::numeric_limits<int>::max())
                std::cout << ", ";
        }
        std::cout << "}";
    }
};
