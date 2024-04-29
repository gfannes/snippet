#include <iostream>

template <typename T>
class shared_ptr {
private:
    T* ptr;  // Pointer to the managed object
    int* ref_count;  // Pointer to the reference counter

public:
    // Constructor
    explicit shared_ptr(T* p = nullptr) : ptr(p), ref_count(new int(1)) {
        if (p == nullptr) {
            *ref_count = 0;
        }
    }

    // Copy Constructor
    shared_ptr(const shared_ptr& other) : ptr(other.ptr), ref_count(other.ref_count) {
        if (ptr) {
            ++(*ref_count);
        }
    }

    // Assignment Operator
    shared_ptr& operator=(const shared_ptr& other) {
        if (this != &other) {
            // Decrease old object's reference count and delete if necessary
            decrement_and_destroy();

            // Copy new data
            ptr = other.ptr;
            ref_count = other.ref_count;
            if (ptr) {
                ++(*ref_count);
            }
        }
        return *this;
    }

    // Destructor
    ~shared_ptr() {
        decrement_and_destroy();
    }

    // Dereference operator
    T& operator*() const {
        return *ptr;
    }

    // Pointer access operator
    T* operator->() const {
        return ptr;
    }

    // Utility function to view reference count
    int use_count() const {
        return *ref_count;
    }

private:
    void decrement_and_destroy() {
        if (ptr && --(*ref_count) == 0) {
            delete ptr;
            delete ref_count;
            ptr = nullptr;
            ref_count = nullptr;
        }
    }
};

// Example usage
int main() {
    shared_ptr<int> sp1(new int(10));
    std::cout << "sp1 use count: " << sp1.use_count() << std::endl;  // Should be 1

    {
        shared_ptr<int> sp2 = sp1;
        std::cout << "sp1 use count after sp2 copy: " << sp1.use_count() << std::endl;  // Should be 2
    }

    std::cout << "sp1 use count after sp2 destruction: " << sp1.use_count() << std::endl;  // Should return to 1
    return 0;
}

