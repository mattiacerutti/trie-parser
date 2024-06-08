#include "../include/trie.hpp"  // It is forbidden to include other libraries!
// #include "trie.hpp"

/* Here below, your implementation of trie. */
template <typename T>
void trie<T>::set_weight(double w) {
   if (this->m_c.size() > 0) {
      throw parser_exception("Cannot set weight for a non-leaf node");
   }

   this->m_w = w;
}

template <typename T>
double trie<T>::get_weight() const {
   return this->m_w;
}

template <typename T>
void trie<T>::set_label(T* l) {
   // Set the label only if node IS NOT the root (meaning it has a parent)

   if (this->m_p == nullptr) {
      delete l;
      throw parser_exception(
          "Root node should not have a label. If you're trying to set a label "
          "for a child node, please call set_parent first");
   }

   if (this->m_l != nullptr) {
      delete this->m_l;
   }

   this->m_l = l;
}

template <typename T>
T const* trie<T>::get_label() const {
   // Return the label only if node IS NOT the root (meaning it has a parent)
   if (this->m_p != nullptr) {
      return this->m_l;
   }

   return nullptr;
}

template <typename T>
T* trie<T>::get_label() {
   // Return the label only if node IS NOT the root (meaning it has a parent)
   if (this->m_p != nullptr) {
      return this->m_l;
   }

   return nullptr;
}

template <typename T>
void trie<T>::set_parent(trie<T>* p) {
   this->m_p = p;
}

template <typename T>
const trie<T>* trie<T>::get_parent() const {
   return this->m_p;
}

template <typename T>
trie<T>* trie<T>::get_parent() {
   return this->m_p;
}

template <typename T>
void trie<T>::add_child(trie<T> const& c) {
   // Should have a valid label before being added
   if (c.get_label() == nullptr) {
      throw parser_exception(
          "Child node should have a valid label before being added");
   }

   // Should have a valid parent before being added
   if (c.get_parent() == nullptr) {
      throw parser_exception(
          "Child node should have a valid parent before being added");
   }

   // Check if there are any siblings with the same label
   if (this->m_c.getWithLabel(*c.get_label()) != nullptr) {
      throw parser_exception(
          "Invalid input: a node with the same label already exists");
   }

   // If current node has a weight, we need to remove it (because non-leaf
   // nodes can't have a weight)
   if (this->m_w != 0.0) {
      this->m_w = 0.0;
   }

   trie<T>* p_child = new trie<T>(c);
   p_child->set_parent(this);

   this->m_c.add(p_child);
}

template <typename T>
bag<trie<T>> const& trie<T>::get_children() const {
   return this->m_c;
}

template <typename T>
bag<trie<T>>& trie<T>::get_children() {
   return this->m_c;
}

template <typename T>
trie<T>::trie() {
   this->m_p = nullptr;
   this->m_l = nullptr;
   this->m_w = 0.0;
}

template <typename T>
trie<T>::trie(double w) {
   this->m_p = nullptr;
   this->m_l = nullptr;
   this->m_w = w;
}

/* Copy Constructor */
template <typename T>
trie<T>::trie(trie<T> const& other) {
   if (other.get_label() == nullptr) {
      this->m_l = nullptr;
   } else {
      this->m_l = new T(*other.get_label());
   }

   this->m_p = other.m_p;

   this->m_w = other.get_weight();

   // This copy-construct also the childrens
   this->m_c = other.get_children();

   this->m_c.setParent(this);
}

template <typename T>
trie<T>::trie(trie<T>&& other) {
   this->m_l = other.m_l;
   this->m_w = other.m_w;
   this->m_p = other.m_p;

   this->m_c = std::move(other.m_c);

   other.m_l = nullptr;
   other.m_p = nullptr;
}

template <typename T>
trie<T>::~trie() {
   delete this->m_l;

   // Set pointers to null. We can use this to check via debugger if something
   // is actually been de-allocated
   this->m_p = nullptr;
   this->m_l = nullptr;
   this->m_w = 0.0;
}

/* Copy Assignment Operator */
template <typename T>
trie<T>& trie<T>::operator=(trie<T> const& other) {
   if (this == &other) {
      return *this;
   }

   // Clear the previous trie
   delete this->m_l;
   this->m_l = nullptr;

   this->m_w = other.get_weight();
   this->m_c = other.get_children();
   this->m_c.setParent(this);

   return *this;
}

/* Move Assignment Operator */
template <typename T>
trie<T>& trie<T>::operator=(trie<T>&& other) {
   if (this == &other) {
      return *this;
   }

   delete this->m_l;
   this->m_l = nullptr;

   this->m_w = other.get_weight();

   this->m_c = std::move(other.m_c);
   this->m_c.setParent(this);

   return *this;
}

template <typename T>
bool areChildrenEqual(bag<trie<T>> const& c1, bag<trie<T>> const& c2) {
   if (c1.size() != c2.size()) {
      return false;
   }

   for (int i = 0; i < c1.size(); i++) {
      trie<T>* child1 = c1.get(i);
      trie<T>* child2 = c2.get(i);

      if (*child1->get_label() != *child2->get_label() ||
          child1->get_weight() != child2->get_weight()) {
         return false;
      }

      if (!areChildrenEqual(child1->get_children(), child2->get_children())) {
         return false;
      }
   }

   return true;
}

template <typename T>
bool trie<T>::operator==(trie<T> const& other) const {
   // If one is root and the other is not, they are not equal
   if (this->m_l == nullptr && other.get_label() != nullptr ||
       this->m_l != nullptr && other.get_label() == nullptr) {
      return false;
   }

   // If both are root, we need to check if they have the same children and the
   // same weight
   if (this->m_l == nullptr && other.get_label() == nullptr) {
      return this->m_w == other.get_weight() &&
             areChildrenEqual(this->m_c, other.get_children());
   }

   // If both are non-root, we need to check if they have the same label and
   // weight
   if (*this->m_l != *other.get_label() || this->m_w != other.get_weight()) {
      return false;
   }

   // Finally, we need to check if they have the same children
   return areChildrenEqual(this->m_c, other.get_children());
}

template <typename T>
bool trie<T>::operator!=(trie<T> const& other) const {
   return !(*this == other);
}

template <typename T>
trie<T>& trie<T>::operator[](std::vector<T> const& path) {
   trie<T>* current = this;

   for (T label : path) {
      trie<T>* tmp = current->get_children().getWithLabel(label);

      if (tmp != nullptr) {
         current = tmp;
      } else {
         break;
      }
   }

   return *current;
}

template <typename T>
trie<T> const& trie<T>::operator[](std::vector<T> const& path) const {
   const trie<T>* current = this;

   for (T label : path) {
      const trie<T>* tmp = current->get_children().getWithLabel(label);

      if (tmp != nullptr) {
         current = tmp;
      } else {
         break;
      }
   }

   return *current;
}

void cleanString(string& str) {
   int firstChar = str.find_first_not_of(" \t\n");

   int lastChar = str.find_last_not_of(" \t\n");

   if (firstChar == string::npos || lastChar == string::npos) {
      // If the string has only spaces, tabs or newlines, it means that is
      // invalid
      return;
   }

   string newStr = str.substr(firstChar, lastChar - firstChar + 1);

   // If the new string still has spaces, tabs or newlines, it means that the
   // keyword is not well formatted
   if (newStr.find(' ') != string::npos || newStr.find('\t') != string::npos ||
       newStr.find('\n') != string::npos) {
      throw parser_exception(
          "Invalid input: keywords can't have spaces, tabs or newlines in "
          "the middle of the string");
   }

   str = newStr;
}

string getNextChunk(istream& is) {
   if (is.eof()) {
      throw parser_exception("Unexpected end of file");
   }

   string chunk;
   getline(is, chunk, ' ');

   cleanString(chunk);

   while (chunk == "" || chunk.find(' ') != string::npos ||
          chunk.find('\t') != string::npos ||
          chunk.find('\n') != string::npos) {
      getline(is, chunk, ' ');
      cleanString(chunk);
   }

   return chunk;
}

char getNextChar(istream& is) {
   char nextChar;
   is >> nextChar;

   while (nextChar == ' ' || nextChar == '\n' || nextChar == '\t') {
      is >> nextChar;
   }

   return nextChar;
}

void C(istream& is) {
   string firstChunk = getNextChunk(is);

   if (firstChunk == "children") {
      // We have a label and a weight
   } else {
      throw parser_exception(
          "Invalid input: expected children but got something else (this "
          "usually happens when a node has another property other than "
          "weight and label)");
   }
}

template <typename T>
void B(istream& is, bool& shouldEspectLeaf, trie<T>& currentTrie) {
   string firstChunk = getNextChunk(is);

   if (firstChunk == "children") {
      // We have only a label

   } else {
      // We have a label and a weight

      // Check if weight is double
      try {
         currentTrie.set_weight(stod(firstChunk));
      } catch (invalid_argument e) {
         throw parser_exception("Invalid input: weight should be a double");
      }

      shouldEspectLeaf = true;

      C(is);
   }
}

template <typename T>
void S(istream& is, trie<T>& currentTrie, trie<T>& parentTrie);

template <typename T>
void S(istream& is, trie<T>& currentTrie);

template <typename T>
void R(istream& is, bool shouldEspectLeaf, trie<T>& currentTrie) {
   char nextChar = getNextChar(is);
   if (nextChar == '}') {
      // We have a leaf

      if (!shouldEspectLeaf) {
         throw parser_exception(
             "Invalid input: a leaf node must have a weight");
      }

      is.unget();
   } else {
      // We have a node

      if (shouldEspectLeaf) {
         throw parser_exception(
             "Invalid input: a non-leaf node must not have a weight");
      }

      // Go back one character
      is.unget();

      // Build the child

      auto it = currentTrie.root();
      using ValueType = typename decltype(it)::value_type;

      trie<ValueType> child;
      child.set_parent(&currentTrie);
      S(is, child, currentTrie);

      // Add the new child to the parent trie we passed as an argument
      currentTrie.add_child(child);
   }
}

template <typename T>
void S(istream& is, trie<T>& currentTrie, trie<T>& parentTrie) {
   string firstChunk = getNextChunk(is);

   bool shouldEspectLeaf = false;

   if (firstChunk == "children") {
      // We are in the root

      throw parser_exception(
          "Invalid input: non-root nodes should have a label");

   } else {
      // We are in a child and firstChunk is a label

      if (firstChunk.find('}') != string::npos ||
          firstChunk.find('{') != string::npos ||
          firstChunk.find('=') != string::npos ||
          firstChunk.find(',') != string::npos) {
         throw parser_exception(
             "Invalid input: got unexpected character in the beginning of "
             "the string");
      }

      using trieType = decay_t<T>;

      try {
         if constexpr (is_same_v<trieType, string>) {
            string* newStr = new string(firstChunk);
            currentTrie.set_label(newStr);
         } else if constexpr (is_same_v<trieType, char>) {
            if (firstChunk.size() != 1) {
               throw parser_exception(
                   "Invalid input: char label can only have one "
                   "character");
            }

            currentTrie.set_label(new trieType(firstChunk[0]));
         } else if constexpr (is_integral_v<trieType>) {
            if (firstChunk.find('.') != string::npos) {
               throw parser_exception(
                   "Invalid input: label type is an integer, but got a "
                   "floating point number");
            }

            trieType value = stoi(firstChunk);

            currentTrie.set_label(new trieType(value));
         } else if constexpr (is_floating_point_v<trieType>) {
            trieType value = stod(firstChunk);

            currentTrie.set_label(new trieType(value));
         }

         else {
            throw parser_exception("Invalid input: label type not supported");
         }
      } catch (invalid_argument e) {
         throw parser_exception("Error while parsing label");
      }

      B(is, shouldEspectLeaf, currentTrie);
   }

   /* Here we correctly set label and weight */

   // Check if there is "="
   char equalChar = getNextChar(is);
   if (equalChar != '=') {
      throw parser_exception(
          "Invalid input: expected = but got something else");
   }

   // Check if there is "{"
   char firstChar = getNextChar(is);
   if (firstChar != '{') {
      throw parser_exception(
          "Invalid input: expected { but got something else");
   }

   R(is, shouldEspectLeaf, currentTrie);

   // Check if there is "}"
   char secondChar = getNextChar(is);
   if (secondChar != '}') {
      throw parser_exception(
          "Invalid input: expected } but got something else");
   }

   /* Here we correctly inserted all children */

   // Check if there is ","
   char thirdChar = getNextChar(is);
   if (thirdChar == ',') {
      // We have a sibling

      auto it = currentTrie.root();
      using ValueType = typename decltype(it)::value_type;

      trie<ValueType> sibling;

      sibling.set_parent(&parentTrie);

      S(is, sibling, parentTrie);

      // Add the new sibling to the parent trie we passed as an argument
      parentTrie.add_child(sibling);

   } else {
      // We need to go back one character, because we already read the next
      // pharentesis
      is.unget();
   }
}

template <typename T>
void S(istream& is, trie<T>& currentTrie) {
   string firstChunk = getNextChunk(is);

   bool shouldEspectLeaf = false;

   if (firstChunk == "children") {
      // We are in the root

   } else {
      throw parser_exception(
          "Invalid input: root node should start with children keyword");
   }

   /* Here we correctly set label and weight */

   // Check if there is "="
   char equalChar = getNextChar(is);
   if (equalChar != '=') {
      throw parser_exception(
          "Invalid input: expected = but got something else");
   }

   // Check if there is "{"
   char firstChar = getNextChar(is);
   if (firstChar != '{') {
      throw parser_exception(
          "Invalid input: expected { but got something else");
   }

   R(is, shouldEspectLeaf, currentTrie);

   // Check if there is "}"
   char secondChar = getNextChar(is);
   if (secondChar != '}') {
      throw parser_exception(
          "Invalid input: expected } but got something else");
   }

   /* Here we correctly inserted all children */

   // Check if there is ","
   char thirdChar = getNextChar(is);
   if (thirdChar == ',') {
      // We have a sibling

      throw parser_exception(
          "Invalid input: root node should not have siblings");

   } else {
      // We need to go back one character, because we already read the next
      // pharentesis
      is.unget();
   }
}

template <typename T>
istream& operator>>(std::istream& stream, trie<T>& trie) {
   // TODO: Add label type checking = T

   // If file is empty
   if (stream.peek() == EOF) {
      trie.set_label(nullptr);
      trie.set_weight(0.0);
      trie.set_parent(nullptr);
      return stream;
   }

   S(stream, trie);

   return stream;
}

template <typename T>
trie<T>::node_iterator::node_iterator(trie<T>* ptr) : m_ptr(ptr) {}

template <typename T>
typename trie<T>::node_iterator& trie<T>::node_iterator::operator++() {
   // TODO: Don't know if i should do this
   if (m_ptr->m_p == nullptr) {
      throw parser_exception("Node has no parent");
   }

   m_ptr = m_ptr->m_p;

   return *this;
}

template <typename T>
typename trie<T>::node_iterator trie<T>::node_iterator::operator++(int) {
   node_iterator ob = *this;

   ++(*this);

   return ob;
}

template <typename T>
typename trie<T>::node_iterator::reference trie<T>::node_iterator::operator*()
    const {
   // TODO: Don't know if i should do this
   if (m_ptr->m_l == nullptr) {
      throw parser_exception("Invalid input: node has no label");
   }
   return *m_ptr->m_l;
}

template <typename T>
typename trie<T>::node_iterator::pointer trie<T>::node_iterator::operator->()
    const {
   if (m_ptr == nullptr) {
      throw parser_exception("Invalid input: node has no label");
   }
   return m_ptr->m_l;
}

template <typename T>
bool trie<T>::node_iterator::operator==(node_iterator const& node) const {
   return this->m_ptr == node.m_ptr;
}

template <typename T>
bool trie<T>::node_iterator::operator!=(node_iterator const& node) const {
   return this->m_ptr != node.m_ptr;
}

template <typename T>
trie<T>::const_node_iterator::const_node_iterator(trie<T> const* ptr)
    : m_ptr(ptr) {}

template <typename T>
typename trie<T>::const_node_iterator&
trie<T>::const_node_iterator::operator++() {
   // TODO: Don't know if i should do this
   if (m_ptr->m_p == nullptr) {
      throw parser_exception("Node has no parent");
   }

   m_ptr = m_ptr->m_p;

   return *this;
}

template <typename T>
typename trie<T>::const_node_iterator trie<T>::const_node_iterator::operator++(
    int) {
   const_node_iterator ob = *this;

   ++(*this);

   return ob;
}

template <typename T>
typename trie<T>::const_node_iterator::reference
trie<T>::const_node_iterator::operator*() const {
   // TODO: Don't know if i should do this
   if (m_ptr->m_l == nullptr) {
      throw parser_exception("Invalid input: node has no label");
   }
   return *m_ptr->m_l;
}

template <typename T>
typename trie<T>::const_node_iterator::pointer
trie<T>::const_node_iterator::operator->() const {
   if (m_ptr == nullptr) {
      throw parser_exception("Invalid input: node has no label");
   }
   return m_ptr->m_l;
}

template <typename T>
bool trie<T>::const_node_iterator::operator==(
    const_node_iterator const& node) const {
   return this->m_ptr == node.m_ptr;
}

template <typename T>
bool trie<T>::const_node_iterator::operator!=(
    const_node_iterator const& node) const {
   return this->m_ptr != node.m_ptr;
}

template <typename T>
trie<T>::leaf_iterator::leaf_iterator(trie<T>* ptr) : m_ptr(ptr) {}

template <typename T>
typename trie<T>::leaf_iterator::reference trie<T>::leaf_iterator::operator*()
    const {
   // TODO: Don't know if i should do this
   if (m_ptr->m_l == nullptr) {
      throw parser_exception("Invalid input: node has no label");
   }
   return *m_ptr->m_l;
}

template <typename T>
typename trie<T>::leaf_iterator::pointer trie<T>::leaf_iterator::operator->()
    const {
   if (m_ptr == nullptr) {
      throw parser_exception("Invalid input: node has no label");
   }
   return m_ptr->m_l;
}

template <typename T>
bool trie<T>::leaf_iterator::operator==(leaf_iterator const& node) const {
   return this->m_ptr == node.m_ptr;
}

template <typename T>
bool trie<T>::leaf_iterator::operator!=(leaf_iterator const& node) const {
   return this->m_ptr != node.m_ptr;
}

template <typename T>
trie<T>* findNextLeaf(trie<T>* current, trie<T>* root, bool& foundCurrent) {
   if (root == nullptr) {
      return nullptr;
   }

   if (root->get_children().size() == 0) {
      if (foundCurrent) {
         return root;
      }
      if (root == current) {
         foundCurrent = true;
      }
      return nullptr;
   }

   for (int i = 0; i < root->get_children().size(); i++) {
      trie<T>* node =
          findNextLeaf(current, root->get_children().get(i), foundCurrent);
      if (node != nullptr) {
         return node;
      }
   }

   return nullptr;
}

template <typename T>
typename trie<T>::leaf_iterator& trie<T>::leaf_iterator::operator++() {
   trie<T>* root = m_ptr;
   while (root->get_parent() != nullptr) {
      root = root->m_p;
   }

   bool foundCurrent = false;
   trie<T>* nextLeaf = findNextLeaf(m_ptr, root, foundCurrent);

   m_ptr = nextLeaf;

   return *this;
}

template <typename T>
typename trie<T>::leaf_iterator trie<T>::leaf_iterator::operator++(int) {
   leaf_iterator ob = *this;

   ++(*this);

   return ob;
}

template <typename T>
trie<T>::leaf_iterator::operator trie<T>::node_iterator() const {
   trie<T>* tr = m_ptr;

   return node_iterator(tr);
}

template <typename T>
trie<T>& trie<T>::leaf_iterator::get_leaf() const {
   return *m_ptr;
}

template <typename T>
trie<T>::const_leaf_iterator::const_leaf_iterator(const trie<T>* ptr)
    : m_ptr(ptr) {}

template <typename T>
typename trie<T>::const_leaf_iterator::reference
trie<T>::const_leaf_iterator::operator*() const {
   // TODO: Don't know if i should do this
   if (m_ptr->m_l == nullptr) {
      throw parser_exception("Invalid input: node has no label");
   }
   return *m_ptr->m_l;
}

template <typename T>
typename trie<T>::const_leaf_iterator::pointer
trie<T>::const_leaf_iterator::operator->() const {
   if (m_ptr == nullptr) {
      throw parser_exception("Invalid input: node has no label");
   }
   return m_ptr->m_l;
}

template <typename T>
bool trie<T>::const_leaf_iterator::operator==(
    const_leaf_iterator const& node) const {
   return this->m_ptr == node.m_ptr;
}

template <typename T>
bool trie<T>::const_leaf_iterator::operator!=(
    const_leaf_iterator const& node) const {
   return this->m_ptr != node.m_ptr;
}

template <typename T>
const trie<T>* findNextLeaf(const trie<T>* current, const trie<T>* root,
                            bool& foundCurrent) {
   if (root == nullptr) {
      return nullptr;
   }

   if (root->get_children().size() == 0) {
      if (foundCurrent) {
         return root;
      }
      if (root == current) {
         foundCurrent = true;
      }
      return nullptr;
   }

   for (int i = 0; i < root->get_children().size(); i++) {
      const trie<T>* node =
          findNextLeaf(current, root->get_children().get(i), foundCurrent);
      if (node != nullptr) {
         return node;
      }
   }

   return nullptr;
}

template <typename T>
typename trie<T>::const_leaf_iterator&
trie<T>::const_leaf_iterator::operator++() {
   const trie<T>* root = m_ptr;
   while (root->get_parent() != nullptr) {
      root = root->m_p;
   }

   bool foundCurrent = false;
   const trie<T>* nextLeaf = findNextLeaf(m_ptr, root, foundCurrent);

   m_ptr = nextLeaf;

   return *this;
}

template <typename T>
typename trie<T>::const_leaf_iterator trie<T>::const_leaf_iterator::operator++(
    int) {
   const_leaf_iterator ob = *this;

   ++(*this);

   return ob;
}

template <typename T>
trie<T>::const_leaf_iterator::operator trie<T>::const_node_iterator() const {
   const trie<T>* tr = m_ptr;

   return const_node_iterator(tr);
}

template <typename T>
trie<T> const& trie<T>::const_leaf_iterator::get_leaf() const {
   return *m_ptr;
}

template <typename T>
typename trie<T>::node_iterator trie<T>::root() {
   trie<T>* tr = this;
   while (tr->get_parent() != nullptr) {
      tr = tr->m_p;
   }

   return node_iterator(tr);
}

template <typename T>
typename trie<T>::leaf_iterator trie<T>::begin() {
   trie<T>* tr = this;

   while (tr->get_children().size() > 0) {
      tr = tr->get_children().get(0);
   }

   return leaf_iterator(tr);
}

template <typename T>
typename trie<T>::leaf_iterator trie<T>::end() {
   if (this->get_parent() == nullptr) return leaf_iterator(nullptr);

   trie<T>* tr = this;
   if (tr->get_children().size() == 0) {
      return leaf_iterator(nullptr);
   }

   while (tr->get_children().size() > 0) {
      tr = tr->get_children().get(tr->get_children().size() - 1);
   }

   // Now tr is the last leaf that is still a child to the trie. We need to find
   // the one next to tr.

   trie<T>* root = this;
   while (root->get_parent() != nullptr) {
      root = root->m_p;
   }

   bool foundCurrent = false;
   trie<T>* nextLeaf = findNextLeaf(tr, root, foundCurrent);

   return leaf_iterator(nextLeaf);
}

template <typename T>
typename trie<T>::const_leaf_iterator trie<T>::begin() const {
   trie<T>* tr = this;

   while (tr->get_children().size() > 0) {
      tr = tr->get_children().get(0);
   }

   return const_leaf_iterator(tr);
}

template <typename T>
typename trie<T>::const_leaf_iterator trie<T>::end() const {
   if (this->get_parent() == nullptr) return const_leaf_iterator(nullptr);

   trie<T>* tr = this;
   if (tr->get_children().size() == 0) {
      return const_leaf_iterator(nullptr);
   }

   while (tr->get_children().size() > 0) {
      tr = tr->get_children().get(tr->get_children().size() - 1);
   }

   // Now tr is the last leaf that is still a child to the trie. We need to find
   // the one next to tr.

   trie<T>* root = this;
   while (root->get_parent() != nullptr) {
      root = root->m_p;
   }

   bool foundCurrent = false;
   trie<T>* nextLeaf = findNextLeaf(tr, root, foundCurrent);

   return const_leaf_iterator(nextLeaf);
}

template <typename T>
typename trie<T>::const_node_iterator trie<T>::root() const {
   const trie<T>* tr = this;
   while (tr->get_parent() != nullptr) {
      tr = tr->m_p;
   }

   return const_node_iterator(tr);
}

template <typename T>
trie<T>& trie<T>::max() {
   double maxWeight = 0.0;
   leaf_iterator maxLeafIt = this->begin();

   for (auto leaf_it = this->begin(); leaf_it != this->end(); ++leaf_it) {
      const trie<T>& leaf = leaf_it.get_leaf();
      if (leaf.get_weight() > maxWeight) {
         maxWeight = leaf.get_weight();
         maxLeafIt = leaf_it;
      }
   }

   return maxLeafIt.get_leaf();
}

template <typename T>
const trie<T>& trie<T>::max() const {
   double maxWeight = 0.0;
   const_leaf_iterator maxLeafIt = this->begin();

   for (auto leaf_it = this->begin(); leaf_it != this->end(); ++leaf_it) {
      const trie<T>& leaf = leaf_it.get_leaf();

      if (leaf.get_weight() > maxWeight) {
         maxWeight = leaf.get_weight();
         maxLeafIt = leaf_it;
      }
   }

   return maxLeafIt.get_leaf();
}

template <typename T>
void addWithRecurion(trie<T>* tr, const trie<T>* other) {
   if (tr->get_children().size() == 0) {
      // We are in a leaf
      if (other) {
         if (other->get_children().size() > 0) {
            double trWeight = tr->get_weight();
            // Add all other's children to tr
            for (typename bag<trie<T>>::const_child_iterator it =
                     other->get_children().begin();
                 it != other->get_children().end(); ++it) {
               tr->add_child(*it);
            }

            for (typename trie<T>::leaf_iterator leaf_it = tr->begin();
                 leaf_it != tr->end(); ++leaf_it) {
               leaf_it.get_leaf().set_weight(leaf_it.get_leaf().get_weight() +
                                             trWeight);
            }
         } else {
            // Sum the weights
            tr->set_weight(tr->get_weight() + other->get_weight());
         }
      }
   } else {
      if (other->get_children().size() > 0) {
         for (int i = 0; i < other->get_children().size(); i++) {
            trie<T>* otherChild = other->get_children().get(i);
            trie<T>* trChild =
                tr->get_children().getWithLabel(*otherChild->get_label());

            if (!trChild) {
               tr->add_child(*otherChild);
            } else {
               addWithRecurion(trChild, otherChild);
            }
         }
      } else {
         for (typename trie<T>::leaf_iterator leaf_it = tr->begin();
              leaf_it != tr->end(); ++leaf_it) {
            leaf_it.get_leaf().set_weight(leaf_it.get_leaf().get_weight() +
                                          other->get_weight());
         }
      }
   }
}

template <typename T>
trie<T> trie<T>::operator+(trie<T> const& other) const {
   // Make return trie copy contructed from this
   trie<T> newTrie = *this;

   // Now, we need to add other
   addWithRecurion(&newTrie, &other);

   return newTrie;
}

template <typename T>
trie<T>& trie<T>::operator+=(trie<T> const& other) {
   *this = *this + other;

   return *this;
}

template <typename T>
void compressWithRecursion(trie<T>* tr) {
   if (tr->get_children().size() == 0) {
      return;
   }

   for (typename bag<trie<T>>::child_iterator it = tr->get_children().begin();
        it != tr->get_children().end(); ++it) {
      compressWithRecursion(&(*it));
   }

   if (tr->get_children().size() == 1) {
      // We need to compress this node
      T compressedLabel =
          *tr->get_label() + *tr->get_children().get(0)->get_label();
      *tr = std::move(*tr->get_children().get(0));
      tr->set_label(new T(compressedLabel));
   }
}

template <typename T>
void trie<T>::path_compress() {
   compressWithRecursion(this);
}

template <typename T>
void traverseRecursive(ostream& os, const trie<T>& tr) {
   if (tr.get_label() != nullptr) {
      os << *tr.get_label() << " ";
   }

   if (tr.get_weight() != 0.0) {
      os << tr.get_weight() << " ";
   }

   os << "children = {";

   for (auto it = tr.get_children().begin(); it != tr.get_children().end();
        ++it) {
      if (it != tr.get_children().begin()) {
         os << ", ";
      }
      traverseRecursive(os, *it);
   }

   os << "}";
}

template <typename T>
std::ostream& operator<<(std::ostream& os, trie<T> const& tr) {
   traverseRecursive(os, tr);

   return os;
}

// int main() {
//     try{

//         // //FIXME: ONLY FOR TESTING REASONS
//         // trie<char> t;
//         // ifstream file("../test.txt");
//         // file>>t;
//         // file.close();

//         // trie<char> t1;
//         // ifstream asd("../test2.txt");
//         // asd>>t1;
//         // asd.close();

//         // const trie<char>& t3 = t1;

//         // // *t.get_children().get(1)->get_children().get(0) = move(t1);

//         // vector<char> s{'c', 'i', 'z'};

//         // const trie<char>& t2 = t3[s];

//         // // trie<char> mv = std::move(t);

//         return 0;
//     }  catch (parser_exception e){
//         throw runtime_error(e.what());
//         return 1;
//     } catch (trie_exception e){
//         throw runtime_error(e.what());
//         return 1;
//     }

// }