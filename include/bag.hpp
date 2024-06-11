
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

      int initialSize = nodes.size();

      // We push all the elements of the other bag into this bag
      for (int i = 0; i < other.size(); i++) {
         nodes.push_back(other.get(i));
      }
      // We clear the previous bag (need this because otherwise destructor will delete the nodes we just inserted)
      other.nodes.clear();

      // We still have (if present) the nodes of the previous bag in this. We delete them and remove them
      for (int i = 0; i < initialSize; i++) {
         delete this->get(0);
         this->nodes.erase(this->nodes.begin()); //Deletes the first element (due to push_back, old nodes are at the beginning of the vector)
      }

      return *this;
   }

   // Methods
   void add(trie<T>* node) {
      if (nodes.size() == 0) {
         nodes.push_back(node);
         return;
      }

      for (size_t i = 0; i < nodes.size(); i++) {
         if (*node->get_label() > *nodes[i]->get_label()) {
            if (i == nodes.size() - 1) {
               nodes.push_back(node);
               return;
            }

            if (*node->get_label() < *nodes[i + 1]->get_label()) {
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
      if (i < 0 || i >= (int)nodes.size()) {
         return nullptr;
      }

      return nodes[i];
   }

   void setParent(trie<T>* parent) {
      for (auto node : nodes) {
         node->set_parent(parent);
      }
   }

   void reorderChilds(){
      vector<trie<T>*> newArr = nodes;
      
      this->clear();

      for(auto tr : newArr){
         this->add(tr);
      }

   }

   int size() const { return nodes.size(); }

   void clear() {
      nodes.clear();
   }

   struct child_iterator {
      using iterator_category = std::forward_iterator_tag;
      using value_type = trie<T>;
      using pointer = trie<T>*;
      using reference = trie<T>&;

      child_iterator(bag<trie<T>>* ptr) {
         index = 0;
         m_ptr = ptr;
         if(!ptr){
            node = nullptr;
            return;
         }
         node = ptr->get(0);
         
      }
      reference operator*() const { return *node; }
      pointer operator->() const { return node; }
      child_iterator& operator++() {
         index++;
         if(index < m_ptr->size()){
            node = m_ptr->get(index);
         }else{
            node = nullptr;
         }
         return *this;
      }
      child_iterator operator++(int) {
         child_iterator temp = *this;
         ++(*this);
         return temp;
      }
      bool operator==(child_iterator const& other) const { return this->node == other.node; }
      bool operator!=(child_iterator const& other) const { return this->node != other.node; }

     private:
      bag<trie<T>>* m_ptr;
      trie<T> * node;
      int index;
   };
   struct const_child_iterator {
      using iterator_category = std::forward_iterator_tag;
      using value_type = const trie<T>;
      using pointer = trie<T> const*;
      using reference = trie<T> const&;

      const_child_iterator(const bag<trie<T>>* ptr) {
         index = 0;
         m_ptr = ptr;
         if(!ptr){
            node = nullptr;
            return;
         }
         node = ptr->get(0);
         
      }
      reference operator*() const { return *node; }
      pointer operator->() const { return node; }
      const_child_iterator& operator++() {
         index++;
         if(index < m_ptr->size()){
            node = m_ptr->get(index);
         }else{
            node = nullptr;
         }
         return *this;
      }
      const_child_iterator operator++(int) {
         const_child_iterator temp = *this;
         ++(*this);
         return temp;
      }
      bool operator==(const_child_iterator const& other) const { return this->node == other.node; }
      bool operator!=(const_child_iterator const& other) const { return this->node != other.node; }

     private:
      const bag<trie<T>>* m_ptr;
      const trie<T> * node;
      int index;
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