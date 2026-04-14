#pragma once

#include <atomic>
#include <memory>
#include <unordered_map>

#include "evt/router/event_queue.h"
#include "evt/session/session_event.h"
#include "net/session/session.h"
#include "net/session_id.h"

namespace net::session {

class manager {
private:
    std::unordered_map<session_id, std::unique_ptr<session>, session_id_hash> sessions;
    std::unordered_map<int, session_id> fd_index;

    evt::event_queue<evt::session::event>* sink{nullptr};
    std::atomic<uint64_t> id_counter{1};
    mutable std::mutex m;

    session_id next_id() { return session_id(id_counter.fetch_add(1, std::memory_order_relaxed)); }

public:
    bool create_session(int fd) noexcept;
    bool delete_session_by_id(session_id) noexcept;
    bool delete_session_by_fd(int fd) noexcept;

    void scan_sessions() noexcept;
    void shutdown() noexcept;

    void bind_sink(evt::event_queue<evt::session::event>& q) noexcept { sink = &q; }
    // use assert(sink) everywhere!

    manager() noexcept = default;
    ~manager() noexcept = default;
};

} // namespace net::session