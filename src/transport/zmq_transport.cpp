#include <iostream>
#include "zmq.hpp"
#include "zmq_transport.hpp"

ZmqTransport::ZmqTransport()
{
    m_running = false;
    m_context = zmq::context_t(1);
    m_subscriber = zmq::socket_t(m_context, zmq::socket_type::sub);
    m_subscriber.connect("tcp://localhost:9956");
    m_subscriber.set(zmq::sockopt::subscribe, "");
    m_subscriber.set(zmq::sockopt::rcvtimeo, 100);
    m_running = true;
    m_receiveEventsThread = std::thread([this]()
                                    { receiveEventsCallback(); });
    m_sendCommandsThread = std::thread([this]()
                                    { sendCommandsCallback(); });
    m_publisher = zmq::socket_t(m_context, zmq::socket_type::pub);
    m_publisher.bind("tcp://*:9955");
}

ZmqTransport::~ZmqTransport()
{
    std::cout << "Stopping ZMQ transport" << std::endl;

    m_running = false;
    commandsQueue.stop();
    if (m_receiveEventsThread.joinable()) {
        m_receiveEventsThread.join();
    }

    if (m_sendCommandsThread.joinable()) {
        m_sendCommandsThread.join();
    }

    m_subscriber.close();
    m_publisher.close();
    m_context.close();
}

void ZmqTransport::stop()
{
    std::cout << "stop" << std::endl;
    m_running = false;
    m_subscriber.close();
    m_context.close();
    m_receiveEventsThread.join();
}

void ZmqTransport::sendCommand(Cmd command){
    commandsQueue.push(command);
}

void ZmqTransport::getReceivedEvent(){

}

void ZmqTransport::receiveEventsCallback()
{
    std::cout << "Starting ZMQ receive events thread" << std::endl;

    try {
        while (m_running) {
            zmq::message_t msg;

            auto result = m_subscriber.recv(msg, zmq::recv_flags::none);
            if (!result) {
                continue;
            }

            std::string msg_str(static_cast<char*>(msg.data()), msg.size());
            try {
                Event event = decodeEvent(msg_str);
                eventsQueue.push(std::move(event));
            } catch (const std::exception& e) {
                std::cerr << "Event decode failed: " << e.what()
                          << "\nRaw message: " << msg_str << std::endl;
            }
        }
    } catch (const zmq::error_t& e) {

    } catch (const std::exception& e) {
        printf("ZMQ error: %s\n", e.what());
    }
}

void ZmqTransport::sendCommandsCallback(){
    std::cout << "Starting ZMQ send commands thread" << std::endl;
    try
    {
        while (m_running)
        {
            Cmd command;
            if(!commandsQueue.wait_and_pop(command)){
                continue;
            }
            std::string data = encodeCmd(command);
            zmq::message_t message(data.data(), data.size());
            auto res = m_publisher.send(message, zmq::send_flags::dontwait);
        }
    }
    catch (const std::exception &e)
    {
        printf("ZMQ error: %s\n", e.what());
    }
}
