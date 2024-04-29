template<typename T>
class unique_ptr
{
private:
    T *ptr;

public:
    // Default constructor
    explicit unique_ptr(T *p = nullptr)
        : ptr(p) {}

    // Destructor
    ~unique_ptr()
    {
        delete ptr;
    }

    // Disable copy constructor and copy assignment operator
    unique_ptr(const unique_ptr &) = delete;
    unique_ptr &operator=(const unique_ptr &) = delete;

    // Move constructor
    unique_ptr(unique_ptr &&other)
        : ptr(other.ptr)
    {
        other.ptr = nullptr;
    }

    // Move assignment operator
    unique_ptr &operator=(unique_ptr &&other)
    {
        if (this != &other)
        {
            delete ptr;
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }

    // Operator* to dereference the pointer to access the managed object
    T &operator*() const
    {
        return *ptr;
    }

    // Operator-> to support accessing the managed object's members
    T *operator->() const
    {
        return ptr;
    }

    // Function to release ownership of the managed object and return the pointer
    T *release()
    {
        T *temp = ptr;
        ptr = nullptr;
        return temp;
    }

    // Function to reset the managed object with a new object
    void reset(T *p = nullptr)
    {
        T *old = ptr;
        ptr = p;
        delete old;
    }

    // Function to swap the managed object with another unique_ptr
    void swap(unique_ptr &other)
    {
        T *temp = ptr;
        ptr = other.ptr;
        other.ptr = temp;
    }

    // Function to get the raw pointer without transferring ownership
    T *get() const
    {
        return ptr;
    }
};

int main()
{
    unique_ptr<int> ptr(new int);
    return 0;
}
