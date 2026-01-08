#pragma once

#include "adt.hpp"

struct TreiberStackNode {
    // A01
    int value;
    TreiberStackNode* next = nullptr;
};

class TreiberStack: public Stack {
private:
    // A01
    std::atomic<TreiberStackNode*> top;

    EventMonitor<TreiberStack, StdStack, StackOperator>* monitor;
    std::mutex cas_lock;
public:
    TreiberStack(EventMonitor<TreiberStack, StdStack, StackOperator>* monitor)
        : monitor(monitor)
    {
        // A01
        this->top.store(nullptr);
    }

    ~TreiberStack() {
        // A01
        TreiberStackNode* cur = this->top.load();
        while (cur) {
            TreiberStackNode* nxt = cur->next;
            delete cur;
            cur = nxt;
        }
    }

    int push(int value) override {
        int result = true;
        // A01
        TreiberStackNode* node = new TreiberStackNode();
        node->value = value;

        while (true) {
            TreiberStackNode* t = this->top.load();
            node->next = t;

           
            this->cas_lock.lock();
            
            if (this->top.compare_exchange_strong(t, node)) {
                // linearization point: successful CAS
                this->monitor->add(StackEvent(StackOperator::StackPush, value, result));
                this->cas_lock.unlock();
                return result;
            }
            // CAS failed, release lock and retry
            this->cas_lock.unlock();
        }
    }

    int pop() override {
        int result = EMPTY_STACK_VALUE;
        // A01
        while (true) {
            TreiberStackNode* t = this->top.load();
            if (t == nullptr) {
                this->cas_lock.lock();
                if (this->top.load() == nullptr) {
                    this->monitor->add(StackEvent(StackOperator::StackPop, NO_ARGUMENT_VALUE, EMPTY_STACK_VALUE));
                    this->cas_lock.unlock();
                    return EMPTY_STACK_VALUE;
                }
                this->cas_lock.unlock();
                continue;
            }

            this->cas_lock.lock();
            if (this->top.compare_exchange_strong(t, t->next)) {
                result = t->value;
                this->monitor->add(StackEvent(StackOperator::StackPop, NO_ARGUMENT_VALUE, result));
                this->cas_lock.unlock();
                // delete t;
                return result;
            }
            this->cas_lock.unlock();
        }
    }

    int size() override {
        int result = 0;
        // A01
        this->cas_lock.lock();
        TreiberStackNode* cur = this->top.load();
        while (cur) {
            result++;
            cur = cur->next;
        }
        this->monitor->add(StackEvent(StackOperator::StackSize, NO_ARGUMENT_VALUE, result));
        this->cas_lock.unlock();
        return result;
    }

    void print_state() override {
        std::lock_guard<std::mutex> guard(this->cas_lock);
        std::cout << "TreiberStack {";
        TreiberStackNode* cur = this->top.load();
        bool first = true;
        while (cur) {
            if (!first) std::cout << ", ";
            std::cout << cur->value;
            first = false;
            cur = cur->next;
        }
        std::cout << "}";
    }
};