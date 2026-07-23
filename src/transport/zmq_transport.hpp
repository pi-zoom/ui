#ifndef ZMQ_TRANSPORT_HPP
#define ZMQ_TRANSPORT_HPP

#include <queue>
#include <mutex>
#include <atomic>
#include <variant>
#include <thread>
#include <condition_variable>
#include <zmq.hpp>
#include <nlohmann/json.hpp>
#include "protocol/protocol.hpp"

// -----------------------------------------------------------------------------
// Thread-safe queue
// -----------------------------------------------------------------------------
template<typename T>
class ThreadSafeQueue {
public:
    void push(T value)
    {
        std::lock_guard lock(mutex_);
        queue_.push(std::move(value));
        cv_.notify_one();
    }

    bool wait_and_pop(T& value)
    {
        std::unique_lock<std::mutex> lock(mutex_);

        cv_.wait(lock, [&] {
            return !queue_.empty() || stop_;
        });

        if (stop_ || queue_.empty())
            return false;

        value = std::move(queue_.front());
        queue_.pop();
        return true;
    }

    bool try_pop(T& value)
    {
        std::lock_guard lock(mutex_);

        if(queue_.empty())
            return false;

        value = std::move(queue_.front());
        queue_.pop();
        return true;
    }

    void stop()
    {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            stop_ = true;
        }
        cv_.notify_all();
    }

private:
    std::queue<T> queue_;
    std::mutex mutex_;
    std::condition_variable cv_;
    bool stop_ = false;
};

class ZmqTransport{
    public:
// bool ZmqTransport::addLoop()
// {
//     commandsQueue.push(CmdLooperAddLoop{});
// }

// bool ZmqTransport::removeLoop()
// {
//     commandsQueue.push(CmdLooperRemoveLoop{});
// }

// void ZmqTransport::selectLoop(int loop)
// {
//     commandsQueue.push(CmdLooperSelectLoop{.id = loop});
// }

// void ZmqTransport::selectPedalboard(std::string name)
// {
//     commandsQueue.push(CmdModSelectPedalboard{.name = name});
//     // std::string msg = "SelectPedalboard " + name;
//     // ZmqTransport::message_t message(msg.data(), msg.size());
//     // std::cout << msg << std::endl;
//     // auto res = m_publisher.send(message, ZmqTransport::send_flags::dontwait);
// }

// void ZmqTransport::setEffectParam(std::string pluginName, std::string paramName, float value)
// {
//     commandsQueue.push(CmdModEffectParamSet{.pluginName = pluginName, .parameterName = paramName, .value = value});
//     // std::string msg = "SetParam " + pluginName + " " + paramName + " " + std::to_string(value);
//     // ZmqTransport::message_t message(msg.data(), msg.size());
//     // std::cout << msg << std::endl;
//     // auto res = m_publisher.send(message, ZmqTransport::send_flags::dontwait);
// }
        ZmqTransport(void);
        ~ZmqTransport();
        ThreadSafeQueue<Event> eventsQueue;
        ThreadSafeQueue<Cmd> commandsQueue;
        void sendCommand(Cmd command);
        void getReceivedEvent();
        // bool addLoop();
        // bool removeLoop();
        // void selectLoop(int loop);
        // void selectPedalboard(std::string name);
        // void setEffectParam(std::string pluginName, std::string paramName, float value);
        void stop();

    private:
        std::atomic_bool m_running;
        std::thread m_receiveEventsThread;
        std::thread m_sendCommandsThread;
        zmq::context_t m_context;
        zmq::socket_t m_subscriber;
        zmq::socket_t m_publisher;

        void receiveEventsCallback();
        void sendCommandsCallback();
        // Event decodeEvent(std::string &data);
        // std::string encodeCmd(Cmd &command);
};

extern ZmqTransport transport;

#endif