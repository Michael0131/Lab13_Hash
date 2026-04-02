/***********************************************************************
 * Header:
 *    LIST
 * Summary:
 *    Our custom implementation of std::list
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *
 *    This will contain the class definition of:
 *        List         : A class that represents a List
 *        ListIterator : An iterator through List
 * Author
 *    Michael Johnson, Brayden Jones, James Gassaway
 ************************************************************************/

#pragma once
#include <cassert>     // for ASSERT
#include <iostream>    // for nullptr
#include <new>         // std::bad_alloc
#include <memory>      // for std::allocator
#include <initializer_list>
#include <utility>

class TestList;        // forward declaration for unit tests
class TestHash;        // to be used later

namespace custom
{

    /**************************************************
     * LIST
     * Just like std::list
     **************************************************/
    template <typename T>
    class list
    {
        friend class ::TestList; // give unit tests access to the privates
        friend class ::TestHash;
        friend void swap(list& lhs, list& rhs);
    public:
        // 
        // Construct
        //

        list();
        list(list <T>& rhs);
        list(list <T>&& rhs);
        list(size_t num, const T& t);
        list(size_t num);
        list(const std::initializer_list<T>& il);
        template <class Iterator>
        list(Iterator first, Iterator last);
        ~list()
        {
            clear();
        }

        // 
        // Assign
        //

        list <T>& operator = (list& rhs);
        list <T>& operator = (list&& rhs);
        list <T>& operator = (const std::initializer_list<T>& il);
        void swap(list <T>& rhs);

        //
        // Iterator
        //

        class  iterator;
        iterator begin() { return iterator(pHead); }
        iterator rbegin() { return iterator(pTail); }
        iterator end() { return iterator(nullptr); }

        //
        // Access
        //

        T& front();
        T& back();

        //
        // Insert
        //

        void push_front(const T& data);
        void push_front(T&& data);
        void push_back(const T& data);
        void push_back(T&& data);
        iterator insert(iterator it, const T& data);
        iterator insert(iterator it, T&& data);

        //
        // Remove
        //

        void pop_back();
        void pop_front();
        void clear();
        iterator erase(const iterator& it);

        // 
        // Status
        //

        bool empty()  const { return numElements == 0; }
        size_t size() const { return numElements; }

    private:
        // nested linked list class
        class Node;

        // member variables
        size_t numElements; // though we could count, it is faster to keep a variable
        Node* pHead;    // pointer to the beginning of the list
        Node* pTail;    // pointer to the ending of the list
    };

    /*************************************************
     * NODE
     * the node class.  Since we do not validate any
     * of the setters, there is no point in making them
     * private.  This is the case because only the
     * List class can make validation decisions
     *************************************************/
    template <typename T>
    class list <T> ::Node
    {
    public:
        //
        // Construct
        //
        Node()
        {
            data = T();
            pNext = pPrev = nullptr;
        }
        Node(const T& data)
        {
            this->data = data;
            pNext = pPrev = nullptr;
        }
        Node(T&& data)
        {
            this->data = std::move(data);
            pNext = pPrev = nullptr;
        }

        //
        // Data
        //

        T data;             // user data
        Node* pNext;       // pointer to next node
        Node* pPrev;       // pointer to previous node
    };

    /*************************************************
     * LIST ITERATOR
     * Iterate through a List, non-constant version
     ************************************************/
    template <typename T>
    class list <T> ::iterator
    {
        friend class ::TestList; // give unit tests access to the privates
        friend class ::TestHash;
        template <typename TT>
        friend class custom::list;
    public:
        // constructors, destructors, and assignment operator

        iterator()
        {
            p = nullptr;
        }
        iterator(Node* pIn)
        {
            p = pIn;

        }
        iterator(const iterator& rhs)
        {
            p = rhs.p;
        }
        iterator& operator = (const iterator& rhs)
        {
            if (this != &rhs)
            {
                p = rhs.p;
            }

            return *this;
        }

        // equals, not equals operator
        bool operator == (const iterator& rhs) const { return p == rhs.p; }
        bool operator != (const iterator& rhs) const { return p != rhs.p; }

        // dereference operator, fetch a node
        T& operator * ()
        {
            assert(p != nullptr);
            return p->data;
        }

        // postfix increment
        iterator operator ++ (int postfix)
        {
            iterator temp(*this);
            p = p->pNext;
            return temp;
        }

        // prefix increment
        iterator& operator ++ ()
        {
            p = p->pNext;
            return *this;
        }

        // postfix decrement
        iterator operator -- (int postfix)
        {
            iterator temp(*this);
            p = p->pPrev;
            return temp;
        }

        // prefix decrement
        iterator& operator -- ()
        {
            p = p->pPrev;
            return *this;
        }

        // two friends who need to access p directly
        friend iterator list <T> ::insert(iterator it, const T& data);
        friend iterator list <T> ::insert(iterator it, T&& data);
        friend iterator list <T> ::erase(const iterator& it);

    private:

        typename list <T> ::Node* p;
    };

    /*****************************************
     * LIST :: NON-DEFAULT constructors
     * Create a list initialized to a value
     ****************************************/
    template <typename T>
    list <T> ::list(size_t num, const T& t)
    {
        numElements = 0;
        pHead = pTail = nullptr;
        for (size_t i = 0; i < num; ++i)
            push_back(t);
    }

    /*****************************************
     * LIST :: ITERATOR constructors
     * Create a list initialized to a set of values
     ****************************************/
    template <typename T>
    template <class Iterator>
    list <T> ::list(Iterator first, Iterator last)
    {
        numElements = 0;
        pHead = pTail = nullptr;
        for (; first != last; ++first)
            push_back(*first);
    }

    /*****************************************
     * LIST :: INITIALIZER constructors
     * Create a list initialized to a set of values
     ****************************************/
    template <typename T>
    list <T> ::list(const std::initializer_list<T>& il)
    {
        numElements = 0;
        pHead = pTail = nullptr;
        for (const auto& x : il)
            push_back(x);
    }

    /*****************************************
     * LIST :: NON-DEFAULT constructors
     * Create a list initialized to a value
     ****************************************/
    template <typename T>
    list <T> ::list(size_t num)
    {
        numElements = 0;
        pHead = pTail = nullptr;
        for (size_t i = 0; i < num; ++i)
            push_back(T());
    }

    /*****************************************
     * LIST :: DEFAULT constructors
     ****************************************/
    template <typename T>
    list <T> ::list()
    {
        numElements = 0;
        pHead = pTail = nullptr;
    }

    /*****************************************
     * LIST :: COPY constructors
     ****************************************/
    template <typename T>
    list <T> ::list(list& rhs)
    {
        numElements = 0;
        pHead = pTail = nullptr;
        for (Node* cur = rhs.pHead; cur != nullptr; cur = cur->pNext)
            push_back(cur->data);
    }

    /*****************************************
     * LIST :: MOVE constructors
     * Steal the values from the RHS
     ****************************************/
    template <typename T>
    list <T> ::list(list <T>&& rhs)
    {
        numElements = rhs.numElements;
        pHead = rhs.pHead;
        pTail = rhs.pTail;

        rhs.numElements = 0;
        rhs.pHead = rhs.pTail = nullptr;
    }

    /**********************************************
     * LIST :: assignment operator - MOVE
     * Copy one list onto another
     *     INPUT  : a list to be moved
     *     OUTPUT :
     *     COST   : O(n) with respect to the size of the LHS
     *********************************************/
    template <typename T>
    list <T>& list <T> :: operator = (list <T>&& rhs)
    {
        if (this == &rhs)
            return *this;

        clear();

        numElements = rhs.numElements;
        pHead = rhs.pHead;
        pTail = rhs.pTail;

        rhs.numElements = 0;
        rhs.pHead = rhs.pTail = nullptr;

        return *this;
    }

    /**********************************************
     * LIST :: assignment operator
     * Copy one list onto another
     *     INPUT  : a list to be copied
     *     OUTPUT :
     *     COST   : O(n) with respect to the number of nodes
     *********************************************/
    template <typename T>
    list <T>& list <T> :: operator = (list <T>& rhs)
    {
        if (this == &rhs)
            return *this;

        clear();
        for (Node* cur = rhs.pHead; cur != nullptr; cur = cur->pNext)
            push_back(cur->data);

        return *this;
    }

    /**********************************************
     * LIST :: assignment operator
     * Copy one list onto another
     *     INPUT  : a list to be copied
     *     OUTPUT :
     *     COST   : O(n) with respect to the number of nodes
     *********************************************/
    template <typename T>
    list <T>& list <T> :: operator = (const std::initializer_list<T>& il)
    {
        list<T> tmp(il);  // build a temporary list from the initializer values
        this->swap(tmp);  // swap head/tail/size; tmp now holds old nodes
        return *this;     // tmp's destructor clears old nodes (when it falls out of scope)

    }

    /**********************************************
     * LIST :: CLEAR
     * Remove all the items currently in the linked list
     *     INPUT  :
     *     OUTPUT :
     *     COST   : O(n) with respect to the number of nodes
     *********************************************/
    template <typename T>
    void list <T> ::clear()
    {
        // Start at the head
        Node* cur = pHead;
        while (cur)
        {
            Node* next = cur->pNext;
            delete cur; // Delete every single node in the list
            cur = next;
        }

        pHead = nullptr;
        pTail = nullptr;
        numElements = 0;
    }

    /*********************************************
     * LIST :: PUSH BACK
     * add an item to the end of the list
     *    INPUT  : data to be added to the list
     *    OUTPUT :
     *    COST   : O(1)
     *********************************************/
    template <typename T> // copy
    void list <T> ::push_back(const T& data)
    {
        Node* n = new Node(data); // copy to make a new node

        // empty list
        if (pHead == nullptr)
        {
            pHead = pTail = n;
            numElements = 1;
            return;
        }

        // attach at tail
        n->pPrev = pTail;
        pTail->pNext = n;
        pTail = n; // update tail
        ++numElements;
    }

    template <typename T> // move
    void list <T> ::push_back(T&& data)
    {
        Node* n = new Node(std::move(data)); // move to make a new node (value is rvalue/temporary)

        // empty list
        if (pHead == nullptr)
        {
            pHead = pTail = n;
            numElements = 1;
            return;
        }

        // attach at tail
        n->pPrev = pTail;
        pTail->pNext = n;
        pTail = n; // update tail
        ++numElements;

    }

    /*********************************************
     * LIST :: PUSH FRONT
     * add an item to the head of the list
     *     INPUT  : data to be added to the list
     *     OUTPUT :
     *     COST   : O(1)
     *********************************************/
    template <typename T>
    void list <T> ::push_front(const T& data)
    {
        Node* pNew = new Node(data);

        pNew->pNext = pHead;
        if (pHead)
            pHead->pPrev = pNew;
        else
            pTail = pNew;

        pHead = pNew;
        numElements++;
    }

    template <typename T>
    void list <T> ::push_front(T&& data)
    {
        Node* pNew = new Node(data);

        pNew->pNext = pHead;
        if (pHead)
            pHead->pPrev = pNew;
        else
            pTail = pNew;

        pHead = pNew;
        numElements++;
    }


    /*********************************************
     * LIST :: POP BACK
     * remove an item from the end of the list
     *    INPUT  :
     *    OUTPUT :
     *    COST   : O(1)
     *********************************************/
    template <typename T>
    void list <T> ::pop_back()
    {
        // empty
        if (pTail == nullptr)
        {
            return;
        }

        Node* oldTail = pTail;

        // single element
        if (pHead == pTail)
        {
            pHead = pTail = nullptr;
            numElements = 0;
            delete oldTail;
            return;
        }

        // more than one element
        pTail = pTail->pPrev;
        pTail->pNext = nullptr;
        --numElements;
        delete oldTail;
    }

    /*********************************************
     * LIST :: POP FRONT
     * remove an item from the front of the list
     *    INPUT  :
     *    OUTPUT :
     *    COST   : O(1)
     *********************************************/
    template <typename T>
    void list <T> ::pop_front()
    {
        // If there is no pHead, don't even bother
        if (!pHead)
            return;

        // Note the node we need to delete
        Node* pToDelete = pHead;

        // Dislink the next node if it exists and move the head
        if (pHead != pTail)
        {
            pHead->pNext->pPrev = nullptr;
            pHead = pHead->pNext;
        }
        // Set both to nullptr if no next node exists
        else
        {
            pHead = nullptr;
            pTail = nullptr;
        }

        // Delete the node and decrement
        delete pToDelete;
        numElements--;
    }

    /*********************************************
     * LIST :: FRONT
     * retrieves the first element in the list
     *     INPUT  :
     *     OUTPUT : data to be displayed
     *     COST   : O(1)
     *********************************************/
    template <typename T>
    T& list <T> ::front()
    {
        if (empty())
            throw "ERROR: unable to access data from an empty list";
        return pHead->data;
    }

    /*********************************************
     * LIST :: BACK
     * retrieves the last element in the list
     *     INPUT  :
     *     OUTPUT : data to be displayed
     *     COST   : O(1)
     *********************************************/
    template <typename T>
    T& list <T> ::back()
    {
        if (empty())
            throw "ERROR: unable to access data from an empty list";
        return pTail->data;

    }

    /******************************************
     * LIST :: REMOVE
     * remove an item from the middle of the list
     *     INPUT  : an iterator to the item being removed
     *     OUTPUT : iterator to the new location
     *     COST   : O(1)
     ******************************************/
    template <typename T>
    typename list <T> ::iterator  list <T> ::erase(const list <T> ::iterator& it)
    {

        if (pHead == nullptr || it.p == nullptr)  // end() or empty
            return end();

        Node* cur = it.p;
        Node* next = cur->pNext;
        Node* prev = cur->pPrev;

        if (prev)
            prev->pNext = next;
        else
            pHead = next;

        if (next)
            next->pPrev = prev;
        else
            pTail = prev;

        delete cur;
        --numElements;

        return iterator(next); // if next nullptr -> end()
    }

    /******************************************
     * LIST :: INSERT
     * add an item to the middle of the list
     *     INPUT  : data to be added to the list
     *              an iterator to the location where it is to be inserted
     *     OUTPUT : iterator to the new item
     *     COST   : O(1)
     ******************************************/
    template <typename T>
    typename list <T> ::iterator list <T> ::insert(list <T> ::iterator it,
        const T& data)
    {
        Node* n = new Node(data);

        // empty list
        if (pHead == nullptr)
        {
            pHead = pTail = n;
            numElements = 1;
            return iterator(n);
        }

        // insert at end (it == end())
        if (it.p == nullptr)
        {
            n->pPrev = pTail;
            pTail->pNext = n;
            pTail = n;
            ++numElements;
            return iterator(n);
        }

        // insert before it.p
        Node* cur = it.p;
        Node* prev = cur->pPrev;

        n->pNext = cur;
        n->pPrev = prev;
        cur->pPrev = n;

        if (prev)
            prev->pNext = n;
        else
            pHead = n;

        ++numElements;
        return iterator(n);
    }

    template <typename T>
    typename list <T> ::iterator list <T> ::insert(list <T> ::iterator it,
        T&& data)
    {
        Node* n = new Node(std::move(data));

        // empty list
        if (pHead == nullptr)
        {
            pHead = pTail = n;
            numElements = 1;
            return iterator(n);
        }

        // insert at end
        if (it.p == nullptr)
        {
            n->pPrev = pTail;
            pTail->pNext = n;
            pTail = n;
            ++numElements;
            return iterator(n);
        }

        Node* cur = it.p;
        Node* prev = cur->pPrev;

        n->pNext = cur;
        n->pPrev = prev;
        cur->pPrev = n;

        if (prev)
            prev->pNext = n;
        else
            pHead = n;

        ++numElements;
        return iterator(n);
    }

    /**********************************************
     * LIST :: assignment operator - MOVE
     * Copy one list onto another
     *     INPUT  : a list to be moved
     *     OUTPUT :
     *     COST   : O(n) with respect to the size of the LHS
     *********************************************/

    template <typename T>
    void swap(list <T>& lhs, list <T>& rhs)
    {
        lhs.swap(rhs);
    }

    template <typename T>
    void list<T>::swap(list <T>& rhs)
    {
        std::swap(numElements, rhs.numElements);
        std::swap(pHead, rhs.pHead);
        std::swap(pTail, rhs.pTail);
    }

    //#endif
}; // namespace custom