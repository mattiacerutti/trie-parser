
#include <vector>

/*
 * Your implementation of the container bag<Val>.
 * Write both the declaration and definition of the
 * container in this file.
 */

using namespace std;

template <typename T>
struct trie;

template <typename T>
struct bag;

template <typename T>
struct bag<trie<T>> {
   // Contructors and deconstructor

   bag() { nodes = vector<trie<T>*>(); }

   bag(const bag& other) { *this = other; }

   bag(const bag&& other) { *this = std::move(other); }

   ~bag() {
      for (trie<T>* node : nodes) {
         delete node;
      }
      nodes.clear();
   }

   bag& operator=(const bag& other) {
      if (this == &other) return *this;

      for (auto node : nodes) {
         delete node;
      }
      nodes.clear();

      for (int i = 0; i < other.size(); i++) {
         const trie<T>* oldNode = other.get(i);
         trie<T>* newNode = new trie<T>(*oldNode);

         nodes.push_back(newNode);
      }

      return *this;
   }

   bag& operator=(bag&& other) {
      if (this == &other) return *this;

      for (auto node : nodes) {
         delete node;
      }
      nodes.clear();

      for (int i = 0; i < other.size(); i++) {
         nodes.push_back(other.get(i));
      }
      other.nodes.clear();

      return *this;
   }

   // Methods
   void add(trie<T>* node) {
      if (nodes.size() == 0) {
         nodes.push_back(node);
         return;
      }

      for (int i = 0; i < nodes.size(); i++) {
         if (*(*node).get_label() > *(*nodes[i]).get_label()) {
            if (i == nodes.size() - 1) {
               nodes.push_back(node);
               return;
            }

            if ((*node).get_label() < (*nodes[i + 1]).get_label()) {
               nodes.insert(nodes.begin() + i + 1, node);
               return;
            }
         } else {
            nodes.insert(nodes.begin() + i, node);
            return;
         }
      }
   }

   trie<T>* getWithLabel(const T label) const {
      for (trie<T>* node : nodes) {
         if (*node->get_label() == label) {
            return node;
         }
      }

      return nullptr;
   }

   trie<T>* get(int i) const {
      if (i < 0 || i >= nodes.size()) {
         return nullptr;
      }

      return nodes[i];
   }

   void setParent(trie<T>* parent) {
      for (auto node : nodes) {
         node->set_parent(parent);
      }
   }

   int size() const { return nodes.size(); }

   struct child_iterator {
      using iterator_category = std::forward_iterator_tag;
      using value_type = trie<T>;
      using pointer = trie<T>*;
      using reference = trie<T>&;

      child_iterator(bag<trie<T>>* ptr) : m_ptr(ptr) {}
      reference operator*() const { return *m_ptr->get(index); }
      pointer operator->() const { return m_ptr->get(index); }
      child_iterator& operator++() {
         index++;
         return *this;
      }
      child_iterator operator++(int) {
         child_iterator temp = *this;
         index++;
         return temp;
      }
      bool operator==(child_iterator const&) const { return m_ptr == m_ptr; }
      bool operator!=(child_iterator const&) const { return m_ptr != m_ptr; }

     private:
      bag<trie<T>>* m_ptr;
      int index = 0;
   };
   struct const_child_iterator {
      using iterator_category = std::forward_iterator_tag;
      using value_type = const trie<T>;
      using pointer = trie<T> const*;
      using reference = trie<T> const&;

      const_child_iterator(bag<trie<T>> const* ptr) : m_ptr(ptr) {}
      reference operator*() const { return *m_ptr->get(index); }
      pointer operator->() const { return m_ptr->get(index); }
      const_child_iterator& operator++() {
         index++;
         return *this;
      }
      const_child_iterator operator++(int) {
         const_child_iterator temp = *this;
         index++;
         return temp;
      }
      bool operator==(const_child_iterator const&) const {
         return m_ptr == m_ptr;
      }
      bool operator!=(const_child_iterator const&) const {
         return m_ptr != m_ptr;
      }

     private:
      bag<trie<T>> const* m_ptr;
      int index = 0;
   };

   /* methods to return iterators */
   child_iterator begin() { return child_iterator(this); }
   child_iterator end() { return child_iterator(nullptr); }

   const_child_iterator begin() const { return const_child_iterator(this); }
   const_child_iterator end() const { return const_child_iterator(nullptr); }

  private:
   // Data
   vector<trie<T>*> nodes;
};