#ifndef UNIQUE_PTR_H
#define UNIQUE_PTR_H

#include <memory>
#include <type_traits>

namespace my_uniq {
template<typename T>
struct unique_ptr {
private:
    struct concept_ {
        virtual void del(T *p) = 0;
        virtual ~concept_() = default;
    };

    template<typename D>
    struct model : concept_ {
    private:
        D d;
    public:
        explicit model(D &&d): d(std::move(d)) {}
        explicit model(D const &d) : d(d) {}

        void del(T *p) override {
            d(p);
        }

        ~model() override {
        }
    };

private:
    T* data;
    concept_* deleter;
public:
    constexpr unique_ptr() noexcept :  data(nullptr), deleter(nullptr){}

    constexpr unique_ptr(std::nullptr_t) noexcept : data(nullptr), deleter(nullptr){}

    explicit unique_ptr(T* p) noexcept : data(p), deleter(nullptr){}

    template<typename D>
    explicit unique_ptr(T* p, D d) : data(p), deleter(new model<D>(d)){}

    unique_ptr( unique_ptr&& u ) noexcept {
        data = std::move(u.data);
        deleter = std::move(u.deleter);
        u.data = nullptr;
        u.deleter = nullptr;
    }

    ~unique_ptr() {
        if(deleter){
            deleter->del(data);
        } else {
            delete data;
        }
        delete deleter;
    }

    unique_ptr& operator=( unique_ptr&& r ) noexcept {
        if(this == &r) {return *this;}
        if(deleter){
            deleter->del(data);
        } else {
            delete data;
        }
        delete deleter;
        data = std::move(r.data);
        deleter = std::move(r.deleter);
        r.data = nullptr;
        r.deleter = nullptr;
        return *this;
    }

    unique_ptr& operator=(std::nullptr_t) noexcept {
        if(this == nullptr) {return *this;}
        if(deleter){
            deleter->del(data);
        } else {
            delete data;
        }
        delete deleter;
        data = nullptr;
        deleter = nullptr;
        return *this;
    }

    T* release() noexcept {
        T* t = data;
        data = nullptr;
        deleter = nullptr;
        return t;
    }

    void reset(T* t) noexcept {
        if(this == nullptr) {return *this;}
        if(deleter){
            deleter->del(data);
        } else {
            delete data;
        }
        delete deleter;
        data = t;
        deleter = nullptr;
    }

    template<typename D>
    void reset(T* t, D d) {
        if(this == nullptr) {return *this;}
        if(deleter){
            deleter->del(data);
        } else {
            delete data;
        }
        delete deleter;
        data = t;
        deleter = new model<D>(d);
    }

    void swap(unique_ptr& other) noexcept {
        unique_ptr t(std::move(other));
        other = std::move(*this);
        *this = std::move(t);
    }

    T* get() const noexcept {
        return data;
    }

    explicit operator bool() const noexcept {
        return data != nullptr;
    }

    T& operator*() const noexcept {
        return *data;
    }

    T* operator->() const noexcept {
        return data;
    }

};

template<class T, class... Args>
unique_ptr<T> make_unique( Args&&... args ){
    return unique_ptr<T>(new T(std::forward<Args>(args)...));
}

template< class T >
void swap( unique_ptr<T>& lhs, unique_ptr<T>& rhs ) noexcept {
    lhs.swap(rhs);
}

template<class T1, class T2>
bool operator==(const unique_ptr<T1>& x, const unique_ptr<T2>& y) {
    x.get() == y.get();
}

template<class T1, class T2>
bool operator!=(const unique_ptr<T1>& x, const unique_ptr<T2>& y) {
    x.get() != y.get();
}

template<class T1, class T2>
bool operator<(const unique_ptr<T1>& x, const unique_ptr<T2>& y) {
    x.get() < y.get();
}

template<class T1, class T2>
bool operator<=(const unique_ptr<T1>& x, const unique_ptr<T2>& y) {
    x.get() <= y.get();
}

template<class T1, class T2>
bool operator>(const unique_ptr<T1>& x, const unique_ptr<T2>& y) {
    x.get() > y.get();
}

template<class T1, class T2>
bool operator>=(const unique_ptr<T1>& x, const unique_ptr<T2>& y) {
    x.get() >= y.get();
}

}

#endif // UNIQUE_PTR_H
