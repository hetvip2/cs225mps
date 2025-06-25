
class ListIterator : public std::iterator<std::bidirectional_iterator_tag, T> {
  private:
    // @TODO: graded in mp_lists part 1
    ListNode* position_;
    ListNode* tail_;

  public:
    ListIterator() : position_(NULL), tail_(NULL) { }
    ListIterator(ListNode* x, ListNode* tail) : position_(x), tail_(tail) { }


    // Pre-Increment, ++iter
    ListIterator& operator++() {
        // @TODO: graded in mp_lists part 1
        if (position_ != NULL) {
            position_ = position_->next;
        }
        return *this;
    }
    
    // Post-Increment, iter++
    ListIterator operator++(int) {
        // @TODO: graded in mp_lists part 1
        ListIterator temp = *this;
        if (position_ != NULL) {
            position_ = position_->next;
        }
        //return temp;
        return ListIterator(temp);
    }

    // Pre-Decrement, --iter
    ListIterator& operator--() {
    // @TODO: graded in mp_lists part 1
    if (position_) {
        if (position_->prev) {
            position_ = position_->prev;
        } else {
            position_ = tail_;
        }
    } else {
        position_ = tail_;
    }
    return *this;
}

    // Post-Decrement, iter--
    ListIterator operator--(int) {
    // @TODO: graded in mp_lists part 1
    ListIterator temp = *this;
    if (position_) {
        if (position_->prev) {
            position_ = position_->prev;
        } else {
            position_ = tail_;
        }
    } else {
        position_ = tail_;
    }
    return temp;
}

    bool operator!=(const ListIterator& rhs) {
        // @TODO: graded in mp_lists part 1
        return position_ != rhs.position_;
    }

    bool operator==(const ListIterator& rhs) {
        return !(*this != rhs);
    }

    const T& operator*() {
        return position_->data;
    }

    const T* operator->() {
        return &(position_->data);
    }
};
