#pragma once

#include <asio.hpp>

class Worker {
private:
    asio::io_context m_IOContext;
    std::thread m_WorkerThread;

    Worker() {
        m_WorkerThread = std::thread([this]() {
            m_IOContext.run();
        });
    }

public:
    ~Worker() {
        if(m_WorkerThread.joinable()) {
            m_WorkerThread.join();
        }
    }

    static Worker& getInstance() {
        static Worker instance;
        return instance;
    }

    asio::io_context &getContext() {
        return m_IOContext;
    }
};
