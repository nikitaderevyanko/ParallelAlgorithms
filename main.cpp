#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include "SyncContainer.h"
#include <vector>
#include <list>
#include <deque>
#include "gtest/gtest.h"

using namespace std;

int main() {
    CSyncContainer< std::vector<int> > cont;
    cont.push( 10 );
    cont.push( 20 );
    cont.popNoSleep();
    cont.popOrSleep();
    return 0;
}