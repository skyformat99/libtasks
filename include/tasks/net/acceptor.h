/*
 * Copyright (c) 2013-2014 ADTECH GmbH
 * Licensed under MIT (https://github.com/adtechlabs/libtasks/blob/master/COPYING)
 *
 * Author: Andreas Pohl
 */

#ifndef _TASKS_ACCEPTOR_H_
#define _TASKS_ACCEPTOR_H_

#include <cassert>
#include <vector>

#include <tasks/net_io_task.h>
#include <tasks/dispatcher.h>
#include <tasks/logging.h>

namespace tasks {
namespace net {

// Takes a handler class as argument that needs to take the client socket in its constructor.
// See echo_server example.
template <class T>
class acceptor : public net_io_task {
  public:
    acceptor(int port) : net_io_task(EV_READ) {
        // Create a non-blocking master socket.
        tdbg("acceptor(" << this << "): listening on port " << port << std::endl);
        try {
            socket().listen(port);
        } catch (socket_exception& e) {
            terr("acceptor(" << this << "): " << e.what() << std::endl);
            assert(false);
        }
    }

    acceptor(std::string path) : net_io_task(EV_READ) {
        // Create a non-blocking master socket.
        tdbg("acceptor(" << this << "): listening on unix:" << path << std::endl);
        try {
            socket().listen(path);
        } catch (socket_exception& e) {
            terr("acceptor(" << this << "): " << e.what() << std::endl);
            assert(false);
        }
    }

    ~acceptor() { socket().shutdown(); }

    bool handle_event(worker* worker, int /* revents */) {
        try {
            net::socket client = socket().accept();
            tdbg("acceptor(" << this << "): new client fd " << client.fd() << std::endl);
            T* task = new T(client);
            // Note: Calling net_io_tasks::add_task will add the client fd to the event loop
            //       from the context of the current worker thread. If you are using multi
            //       loop mode this meams a client fd will always be added to the same event
            //       loop as the server socket and won't be handled by other threads. If you
            //       run only one acceptor you will effectively running single threaded. But
            //       this can be very usefull if you are running one acceptor per worker.
            //       If you need to distribute client fd's accross your workers you should
            //       implement you own acceptor and use
            //
            //         dispatcher::instance()->add_task(task);
            //
            //       instead.
            add_task(worker, task);
        } catch (socket_exception& e) {
            terr("acceptor(" << this << "): " << e.what() << std::endl);
            assert(false);
        }
        return true;
    }
};

}  // net
}  // tasks

#endif  // _TASKS_ACCEPTOR_H_
