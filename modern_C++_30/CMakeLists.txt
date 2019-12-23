cmake_minimum_required(VERSION 3.14)
project(Morden_C++)

set(CMAKE_CXX_STANDARD 11)

add_executable(heap RAII/heap.cpp)
add_executable(stack RAII/stack.cpp)
add_executable(RAII RAII/RAII.cpp)
add_executable(RAII_fstram RAII/RAII_fstram.cpp)

add_executable(auto_scope smart_ptr/auto_scope.cpp)
add_executable(unique_ptr smart_ptr/unique_ptr.cpp)
add_executable(unique_ptr_U smart_ptr/unique_ptr_U.cpp)
add_executable(shared_ptr smart_ptr/shared_ptr.cpp)

add_executable(reference reference/reference.cpp)
add_executable(forward reference/forward.cpp)
add_executable(collapses reference/collapses.cpp)
add_executable(lifetime reference/lifetime.cpp)
add_executable(dontreturnReference reference/don'treturnReference.cpp)

add_executable(container container1/container.cpp)
add_executable(cont container2/hash.cpp)
add_executable(vector_l container1/vector_l.cpp)
add_executable(priority_queue container2/priority_queue.cpp)
add_executable(relacontainer container2/relacontainer.cpp)
add_executable(unorder container2/unorder.cpp)
add_executable(array container2/array.cpp)
add_executable(exception exception/exception.cpp)


add_executable(returnObj returnObj/returnObj1.cpp)
add_executable(returnObj_add returnObj/returnObj2.cpp)
add_executable(returnObj3 returnObj/returnObj3.cpp)
add_executable(returnObj4 returnObj/returnObj4.cpp)
add_executable(returnObj5 returnObj/returnObj5.cpp)
add_executable(rvonrvo returnObj/all.cpp)
