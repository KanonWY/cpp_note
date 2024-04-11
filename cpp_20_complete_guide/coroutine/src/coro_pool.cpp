#include "coroPool.h"

#include <iostream>
#include <syncstream>

inline auto syncOut(std::ostream& strm = std::cout) {
    return std::osyncstream{ strm };
}


using namespace eon;

CoroPoolTask print(std::string id, std::string msg) {
    syncOut() << "    > " << id << " print: " << msg
              << "   on thread: " << std::this_thread::get_id() << std::endl;
    co_return; // make it a coroutine
}


CoroPoolTask runAsync(std::string id) {
    syncOut() << "===== " << id << " start     "
              << "   on thread: " << std::this_thread::get_id() << std::endl;

    co_await print(id + "a", "start");
    syncOut() << "===== " << id << " resume    "
              << "   on thread " << std::this_thread::get_id() << std::endl;

    co_await print(id + "b", "end  ");
    syncOut() << "===== " << id << " resume    "
              << "   on thread " << std::this_thread::get_id() << std::endl;

    syncOut() << "===== " << id << " done" << std::endl;
}


int main() {
    // init pool of coroutine threads:
    syncOut() << "**** main() on thread " << std::this_thread::get_id()
              << std::endl;
    CoroPool pool{ 4 };

    // start main coroutine and run it in coroutine pool:
    syncOut() << "runTask(runAsync(1))" << std::endl;
    CoroPoolTask t1 = runAsync("1");
    pool.runTask(std::move(t1));

    // wait until all coroutines are done:
    syncOut() << "\n**** waitUntilNoCoros()" << std::endl;
    pool.waitUntilNoCoros();

    syncOut() << "\n**** main() done" << std::endl;
}