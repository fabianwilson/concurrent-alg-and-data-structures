#pragma once

#include "adt.hpp"

struct TreiberStackNode {
    // A01: You can add or remove fields as needed.
    int value;
    TreiberStackNode* next = nullptr;
};

class TreiberStack: public Stack {
private:
    // A01: You can add or remove fields as needed.
    std::atomic<TreiberStackNode*> top;

    EventMonitor<TreiberStack, StdStack, StackOperator>* monitor;
    std::mutex cas_lock;
public:
    TreiberStack(EventMonitor<TreiberStack, StdStack, StackOperator>* monitor)
        : monitor(monitor)
    {
        // A01: Initiate the internal state
        this->top.store(nullptr);
    }

    ~TreiberStack() {
        // A01: Cleanup any memory that was allocated
        TreiberStackNode* cur = this->top.load();
        while (cur) {
            TreiberStackNode* nxt = cur->next;
            delete cur;
            cur = nxt;
        }
    }

    int push(int value) override {
        int result = true;
        // A01: Add code to insert the element at the top of the stack.
        TreiberStackNode* node = new TreiberStackNode();
        node->value = value;

        {
            std::lock_guard<std::mutex> guard(this->cas_lock);
            TreiberStackNode* old = this->top.load();
            node->next = old;
            this->top.store(node);

            this->monitor->add(StackEvent(StackOperator::StackPush, value, result));
        }

        return result;
    }

    int pop() override {
        int result = EMPTY_STACK_VALUE;
        // A01: Add code to pop the element at the top of the stack.
        TreiberStackNode* popped = nullptr;
        {
            std::lock_guard<std::mutex> guard(this->cas_lock);
            TreiberStackNode* cur = this->top.load();
            if (cur == nullptr) {
                result = EMPTY_STACK_VALUE;
            } else {
                popped = cur;
                result = cur->value;
                this->top.store(cur->next);
            }

            this->monitor->add(StackEvent(StackOperator::StackPop, NO_ARGUMENT_VALUE, result));
        }

        if (popped) delete popped; // no memory leak, not really needed cuz memleaks are allowed in this assignment 
        return result;
    }

    int size() override {
        int result = 0;
        // A01: Add code to get the size of the stack.
        {
            std::lock_guard<std::mutex> guard(this->cas_lock);
            TreiberStackNode* cur = this->top.load();
            while (cur) {
                result++;
                cur = cur->next;
            }

            this->monitor->add(StackEvent(StackOperator::StackSize, NO_ARGUMENT_VALUE, result));
        }
        return result;
    }

    void print_state() override {
        std::cout << "TreiberStack { ... }";
    }
};