/***********************************************************************
 * Header:
 *    HASH
 * Summary:
 *    Our custom implementation of std::unordered_Set
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *    This will contain the class definition of:
 *        unordered_set           : A class that represents a hash
 *        unordered_set::iterator : An interator through hash
 * Author
 *    Michael, James, Brayden
 ************************************************************************/

#pragma once

#include "list.h"    // because this->buckets[0] is a list
#include <memory>    // for std::allocator
#include <functional> // for std::hash
#include <cmath>     // for std::ceil
#include <initializer_list>

class TestHash;          // forward declaration for Hash unit tests

namespace custom
{
   /************************************************
    * UNORDERED SET
    * A set implemented as a hash
    ************************************************/
   template <typename T>
   class unordered_set
   {
      friend class ::TestHash;   // give unit tests access to the privates
   public:
      //
      // Construct
      //
      // ---------- Michael Code to Complete ----------
      unordered_set()
      {
         numElements = 0;
      }

      // ---------- James Code to Complete ----------
      unordered_set(unordered_set& rhs)
      {
      }

      // ---------- Brayden Code to Complete ----------
      unordered_set(unordered_set&& rhs)
      {
      }

      // ---------- Michael Code to Complete ----------
      template <class Iterator>
      unordered_set(Iterator first, Iterator last)
      {
         numElements = 0;
         for (; first != last; ++first)
            insert(*first);
      }

      //
      // Assign
      //
      // ---------- Michael Code to Complete ----------
      unordered_set& operator=(unordered_set& rhs)
      {
         if (this == &rhs)
            return *this;
         clear();
         for (int i = 0; i < 10; ++i)
            buckets[i] = rhs.buckets[i];
         numElements = rhs.numElements;
         return *this;
      }

      // ---------- Michael Code to Complete ----------
      unordered_set& operator=(unordered_set&& rhs)
      {
         if (this == &rhs)
            return *this;
         clear();
         for (int i = 0; i < 10; ++i)
            buckets[i] = std::move(rhs.buckets[i]);
         numElements = rhs.numElements;
         rhs.numElements = 0;
         return *this;
      }

      // ---------- Michael Code to Complete ----------
      unordered_set& operator=(const std::initializer_list<T>& il)
      {
         clear();
         insert(il);
         return *this;
      }

      // ---------- Brayden Code to Complete ----------
      void swap(unordered_set& rhs)
      {
      }

      // 
      // Iterator
      //
      class iterator;
      class local_iterator;

      // ---------- Brayden Code to Complete ----------
      iterator begin()
      {
         return iterator();
      }

      // ---------- Brayden Code to Complete ----------
      iterator end()
      {
         return iterator();
      }

      // ---------- James Code to Complete ----------
      local_iterator begin(size_t iBucket)
      {
         return local_iterator();
      }

      // ---------- James Code to Complete ----------
      local_iterator end(size_t iBucket)
      {
         return local_iterator();
      }

      //
      // Access
      //
      // ---------- Michael Code to Complete ----------
      size_t bucket(const T& t)
      {
         return std::hash<T>{}(t) % 10;
      }

      // ---------- Michael Code to Complete ----------
      iterator find(const T& t);

      //   
      // Insert
      //
      // ---------- James Code to Complete ----------
      custom::pair<iterator, bool> insert(const T& t);

      // ---------- James Code to Complete ----------
      void insert(const std::initializer_list<T>& il);

      // 
      // Remove
      //
      // ---------- Brayden Code to Complete ----------
      void clear() noexcept
      {
      }

      // ---------- Brayden Code to Complete ----------
      iterator erase(const T& t);

      //
      // Status
      //
      // ---------- Brayden Code to Complete ----------
      size_t size() const
      {
         return 99;
      }

      // ---------- Brayden Code to Complete ----------
      bool empty() const
      {
         return false;
      }

      // ---------- Brayden Code to Complete ----------
      size_t bucket_count() const
      {
         return 100;
      }

      // ---------- Brayden Code to Complete ----------
      size_t bucket_size(size_t i) const
      {
         return 99;
      }

   private:

      custom::list<T> buckets[10];   // exactly 10 buckets
      int numElements;            // number of elements in the Hash
   };


   /************************************************
    * UNORDERED SET ITERATOR
    * Iterator for an unordered set
    ************************************************/
   template <typename T>
   class unordered_set <T> ::iterator
   {
      friend class ::TestHash;   // give unit tests access to the privates
      template <class TT>
      friend class custom::unordered_set;
   public:
      // 
      // Construct
      //
      // ---------- Michael Code to Complete ----------
      iterator()
      {
         pBucket = nullptr;
         pBucketEnd = nullptr;
         itList = typename custom::list<T>::iterator();
      }

      // ---------- Michael Code to Complete ----------
      iterator(typename custom::list<T>* pBucket,
         typename custom::list<T>* pBucketEnd,
         typename custom::list<T>::iterator itList)
      {
         this->pBucket = pBucket;
         this->pBucketEnd = pBucketEnd;
         this->itList = itList;
      }

      // ---------- Michael Code to Complete ----------
      iterator(const iterator& rhs)
      {
         pBucket = rhs.pBucket;
         pBucketEnd = rhs.pBucketEnd;
         itList = rhs.itList;
      }

      //
      // Assign
      //
      // ---------- Brayden Code to Complete ----------
      iterator& operator = (const iterator& rhs)
      {
         return *this;
      }

      //
      // Compare
      //
      // ---------- Michael Code to Complete ----------
      bool operator != (const iterator& rhs) const
      {
         return !(*this == rhs);
      }

      // ---------- Michael Code to Complete ----------
      bool operator == (const iterator& rhs) const
      {
         return pBucket == rhs.pBucket &&
            pBucketEnd == rhs.pBucketEnd &&
            itList == rhs.itList;
      }

      // 
      // Access
      //
      // ---------- Michael Code to Complete ----------
      T& operator * ()
      {
         return *itList;
      }

      //
      // Arithmetic
      //
      // ---------- James Code to Complete ----------
      iterator& operator ++ ();

      // ---------- Brayden Code to Complete ----------
      iterator operator ++ (int postfix)
      {
         return *this;
      }

   private:
      custom::list<T>* pBucket;
      custom::list<T>* pBucketEnd;
      typename list<T>::iterator itList;
   };


   /************************************************
    * UNORDERED SET LOCAL ITERATOR
    * Iterator for a single bucket in an unordered set
    ************************************************/
   template <typename T>
   class unordered_set <T> ::local_iterator
   {
      friend class ::TestHash;   // give unit tests access to the privates

      template <class TT>
      friend class custom::unordered_set;
   public:
      // 
      // Construct
      //
      // ---------- Michael Code to Complete ----------
      local_iterator()
      {
         itList = typename custom::list<T>::iterator();
      }

      // ---------- Michael Code to Complete ----------
      local_iterator(const typename custom::list<T>::iterator& itList)
      {
         this->itList = itList;
      }

      // ---------- Michael Code to Complete ----------
      local_iterator(const local_iterator& rhs)
      {
         itList = rhs.itList;
      }

      //
      // Assign
      //
      // ---------- Michael Code to Complete ----------
      local_iterator& operator = (const local_iterator& rhs)
      {
         if (this != &rhs)
            itList = rhs.itList;
         return *this;
      }

      // 
      // Compare
      //
      // ---------- Michael Code to Complete ----------
      bool operator != (const local_iterator& rhs) const
      {
         return !(*this == rhs);
      }

      // ---------- Michael Code to Complete ----------
      bool operator == (const local_iterator& rhs) const
      {
         return itList == rhs.itList;
      }

      // 
      // Access
      //
      // ---------- Michael Code to Complete ----------
      T& operator * ()
      {
         return *itList;
      }

      // 
      // Arithmetic
      //
      // ---------- James Code to Complete ----------
      local_iterator& operator ++ ()
      {
         return *this;
      }

      // ---------- James Code to Complete ----------
      local_iterator operator ++ (int postfix)
      {
         return *this;
      }

   private:
      typename list<T>::iterator itList;
   };


   /*****************************************
    * UNORDERED SET :: ERASE
    * Remove one element from the unordered set
    ****************************************/
    // ---------- Brayden Code to Complete ----------
   template <typename T>
   typename unordered_set <T> ::iterator unordered_set<T>::erase(const T& t)
   {
      return iterator();
   }

   /*****************************************
    * UNORDERED SET :: INSERT
    * Insert one element into the hash
    ****************************************/
    // ---------- James Code to Complete ----------
   template <typename T>
   custom::pair<typename custom::unordered_set<T>::iterator, bool> unordered_set<T>::insert(const T& t)
   {
      return custom::pair<custom::unordered_set<T>::iterator, bool>(iterator(), true);
   }

   // ---------- James Code to Complete ----------
   template <typename T>
   void unordered_set<T>::insert(const std::initializer_list<T>& il)
   {
   }

   /*****************************************
    * UNORDERED SET :: FIND
    * Find an element in an unordered set
    ****************************************/
    // ---------- Michael Code to Complete ----------
   template <typename T>
   typename unordered_set <T> ::iterator unordered_set<T>::find(const T& t)
   {
      size_t iBucket = bucket(t);
      auto it = buckets[iBucket].begin();

      while (it != buckets[iBucket].end())
      {
         if (*it == t)
            return iterator(buckets + iBucket, buckets + 10, it);
         ++it;
      }

      return end();
   }

   /*****************************************
    * UNORDERED SET :: ITERATOR :: INCREMENT
    * Advance by one element in an unordered set
    ****************************************/
    // ---------- James Code to Complete ----------
   template <typename T>
   typename unordered_set <T> ::iterator& unordered_set<T>::iterator::operator ++ ()
   {
      return *this;
   }

   /*****************************************
    * SWAP
    * Stand-alone unordered set swap
    ****************************************/
    // ---------- James Code to Complete ----------
   template <typename T>
   void swap(unordered_set<T>& lhs, unordered_set<T>& rhs)
   {
   }

}