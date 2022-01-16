// Copyright Davydov Nikolay 2021

#ifndef INCLUDE_UNIQUE_GUARD_HPP_
#define INCLUDE_UNIQUE_GUARD_HPP_

#include <mutex>

namespace my_unique_guard {
    template<class Mutex>
    class unique_lock {
    private:
        Mutex *mtx_;
        bool m_owns_;

    public:
        unique_lock(const unique_lock &) = delete;

        unique_lock &operator=(const unique_lock &) = delete;

        unique_lock() noexcept: mtx_(nullptr), m_owns_(false) {}

        unique_lock(unique_lock &&other) noexcept: mtx_(other.mtx_), m_owns_(other.m_owns_) {
            other.mtx_ = nullptr;
            other.m_owns_ = false;
        }

        explicit unique_lock(Mutex &m) : mtx_(&m), m_owns_(false) {
            mtx_->lock();
            m_owns_ = true;
        }

        ~unique_lock() {
            if (m_owns_) mtx_->unlock();
        }

        unique_lock &operator=(unique_lock &&other) noexcept {
            if (this == &other) return *this;

            if (m_owns_) mtx_->unlock();

            unique_lock(std::move(other)).swap(*this);

            other.mtx_ = nullptr;
            other.m_owns_ = false;
            return *this;
        }

        unique_lock(Mutex &m, std::defer_lock_t) noexcept: mtx_(&m), m_owns_(false) {}

        unique_lock(Mutex &m, std::adopt_lock_t) noexcept: mtx_(&m), m_owns_(true) {}

        void lock() {
            if (!mtx_) throw std::runtime_error("Operation do not permitted\n");
            if (m_owns_) throw std::runtime_error("Mutex has already locked\n");
            mtx_->lock();
            m_owns_ = true;
        }

        void unlock() {
            if (!m_owns_) throw std::runtime_error("Mutex has already unlocked\n");
            if (mtx_) {
                mtx_->unlock();
                m_owns_ = false;
            }
        }

        bool try_lock(){
            if (!mtx_) throw std::runtime_error("Operation do not permitted\n");
            if (m_owns_) throw std::runtime_error("Mutex has already locked\n");
            m_owns_ = mtx_->try_lock();
            return m_owns_;
        }

        void swap(unique_lock &other) noexcept {
            std::swap(mtx_, other.mtx_);
            std::swap(m_owns_, other.m_owns_);
        }
    };
}

#endif //INCLUDE_UNIQUE_GUARD_HPP_
