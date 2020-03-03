//
// Created by light on 19-12-15.
//
#include <iostream>
#include "../RAII/shape.h"

void overloaded( int const &arg ) { std::cout << "by lvalue\n"; }
void overloaded( int && arg ) { std::cout << "by rvalue\n"; }

template< typename t >
/* "t &&" with "t" being template param is special, and  adjusts "t" to be
   (for example) "int &" or non-ref "int" so std::forward knows what to do. */
void forwarding( t && arg ) {
    std::cout << "via std::forward: ";
    overloaded( std::forward< t >( arg ) );
    std::cout << "via std::move: ";
    overloaded( std::move( arg ) ); // conceptually this would invalidate arg
    std::cout << "by simple passing: ";
    overloaded( arg );
}
void foo(const shape&)
{
    puts("foo(const shape&)");
}
void foo(shape&&)
{
    puts("foo(shape&&)");
}
template <typename T>
void bar(T&& s)
{
    foo(std::forward<T>(s));
}
int main() {
    std::cout << "initial caller passes rvalue:\n";
    forwarding( 5 );
    std::cout << "initial caller passes lvalue:\n";
    int x = 5;
    forwarding( x );

    circle temp;
    bar(temp);
    bar(circle());

}