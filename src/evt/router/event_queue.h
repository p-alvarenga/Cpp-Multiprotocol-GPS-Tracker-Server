#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>
#include <utility>

namespace evt {

template <typename T>

struct event_queue { // temporary solution, better with SPSC queue with ring buffer;
private:
    std::queue<T> q;
    mutable std::mutex m;
    std::condition_variable cv;

public:
    void push(T e) noexcept {
        std::lock_guard<std::mutex> lk(m);
        q.push(std::move(e));
        cv.notify_one();
    }

    T wait_and_pop() noexcept {
        std::unique_lock<std::mutex> lk(m);
        cv.wait(lk, [this] { return !q.empty(); });

        T out = std::move(q.front());
        q.pop();

        return std::move(out);
    }

    size_t size() const noexcept {
        std::lock_guard<std::mutex> lk(m);
        return q.size();
    }

    bool empty() const noexcept {
        std::lock_guard<std::mutex> lk(m);
        return q.empty();
    }

    bool clear() noexcept {
        std::lock_guard<std::mutex> lk(m);
        while (!q.empty()) {
            q.pop();
        }
    }
};
} // namespace evt