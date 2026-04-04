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
         *this = rhs;
      }

      // ---------- Brayden Code to Complete ----------
      unordered_set(unordered_set&& rhs)
      {
          // Move each bucket list over
          for (int i = 0; i < bucket_count(); ++i)
             buckets[i] = std::move(rhs.buckets[i]); // steal the contents
       
          numElements = rhs.numElements;
          rhs.numElements = 0;
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
          using std::swap;
          swap(numElements, rhs.numElements);
          for (int i = 0; i < bucket_count(); ++i)
              buckets[i].swap(rhs.buckets[i]);
      }

      // 
      // Iterator
      //
      class iterator;
      class local_iterator;

      // ---------- Brayden Code to Complete ----------
      iterator begin()
      {
         // First element in the first non-empty bucket
         for (int i = 0; i < bucket_count(); ++i)
         {
             if (!buckets[i].empty())
                 return iterator(&buckets[i], buckets + bucket_count(), buckets[i].begin());
         }

         return end();
      }

      // ---------- Brayden Code to Complete ----------
      iterator end()
      {
         // Match the “done” state that operator++ eventually produces
         // (pBucket == pBucketEnd and itList is default-constructed).
         return iterator(buckets + bucket_count(),
                         buckets + bucket_count(),
                         typename custom::list<T>::iterator());
      }

      // ---------- James Code to Complete ----------
      local_iterator begin(size_t iBucket)
      {
         // Return if bucket doesn't exist
         if (iBucket >= bucket_count())
            return end(iBucket);

         // Return if empty
         if (buckets[iBucket].empty())
            return end(iBucket);
         

         return local_iterator(buckets[iBucket].begin());
      }

      // ---------- James Code to Complete ----------
      local_iterator end(size_t iBucket)
      {
         // Return if bucket doesn't exist
         if (iBucket >= bucket_count())
            return local_iterator();
         
         return local_iterator(buckets[iBucket].end());
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
          for (int i = 0; i < bucket_count(); ++i)
              buckets[i].clear();
          numElements = 0;
      }

      // ---------- Brayden Code to Complete ----------
      iterator erase(const T& t);

      //
      // Status
      //
      // ---------- Brayden Code to Complete ----------
      size_t size() const
      {
          return (size_t)numElements;
      }

      // ---------- Brayden Code to Complete ----------
      bool empty() const
      {
         return numElements == 0;
      }

      // ---------- Brayden Code to Complete ----------
      size_t bucket_count() const
      {
         return 10;
      }

      // ---------- Brayden Code to Complete ----------
      size_t bucket_size(size_t i) const
      {
         if (i >= bucket_count())
         {
            return 0;
         }
         return buckets[i].size();
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
         if (this != &rhs)
         {
            pBucket = rhs.pBucket;
            pBucketEnd = rhs.pBucketEnd;
            itList = rhs.itList;
         }
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
          iterator temp(*this);
          ++(*this);
          return temp;
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
         ++itList;
         return *this;
      }

      // ---------- James Code to Complete ----------
      local_iterator operator ++ (int postfix)
      {
         local_iterator temp(*this);
         ++itList;
         return temp;
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
      // Find the element
      iterator itErase = find(t);
      if (itErase == end())
      {
          return end();
      }

      // Compute the next iterator before we erase itErase's list iterator
      iterator itNext = itErase;
      ++itNext;   

      // Remove from the underlying bucket
      itErase.pBucket->erase(itErase.itList);
      --numElements;

      return itNext;
   }

   /*****************************************
    * UNORDERED SET :: INSERT
    * Insert one element into the hash
    ****************************************/
    // ---------- James Code to Complete ----------
   template <typename T>
   custom::pair<typename custom::unordered_set<T>::iterator, bool> unordered_set<T>::insert(const T& t)
   {
      // 1. Find the bucket
      size_t iBucket = bucket(t);

      // 2. Check for duplicates
      for (auto it = buckets[iBucket].begin(); it != buckets[iBucket].end(); ++it)
      {
         if (*it == t)
         {
            // Return iterator to existing element and false
            return custom::pair<typename custom::unordered_set<T>::iterator, bool>(
               iterator(&buckets[iBucket], &buckets[10], it),
               false
            );
         }
      }

      // 3. Insert the new element at the FRONT of the bucket's list.
      // This makes the iterator to the new element simply .begin()
      buckets[iBucket].push_front(t);
      numElements++;

      // 4. Return results.
      // The new element is now at the front of this specific bucket.
      return custom::pair<typename custom::unordered_set<T>::iterator, bool>(
         iterator(&buckets[iBucket], &buckets[10], buckets[iBucket].begin()),
         true
      );
   }

   // ---------- James Code to Complete ----------
   template <typename T>
   void unordered_set<T>::insert(const std::initializer_list<T>& il)
   {
      for (const auto& element : il)
         insert(element);
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
   typename unordered_set<T>::iterator& unordered_set<T>::iterator::operator ++ ()
   {
      // 1. If we are already at the end of the entire table, do nothing.
      if (pBucket == pBucketEnd)
         return *this;

      // 2. Advance the list iterator.
      ++itList;

      // 3. If we haven't reached the end of the current bucket, we are done!
      if (itList != pBucket->end())
         return *this;

      // 4. If we reached the end of the bucket, find the next non-empty bucket.
      // Move to the next bucket address.
      pBucket++;

      // Keep moving until we find a bucket with something in it or we hit the end.
      while (pBucket != pBucketEnd && pBucket->empty())
      {
         pBucket++;
      }

      // 5. Update itList based on where we landed.
      if (pBucket != pBucketEnd)
         itList = pBucket->begin(); // Start of the new bucket
      else
         itList = typename custom::list<T>::iterator(); // Reset to null/default for end()

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
      lhs.swap(rhs);
   }

}