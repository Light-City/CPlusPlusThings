# Story about static

The static keyword has different meanings when used with different types. We can use the static keyword：

**Static variable：** Variables in functions, variables in classes

**Members of a static class：** Class objects and functions in classes

Now let's take a closer look at these static usages：

**Static variable**

- Static variables in functions

When the variable is declared static，space **Will be allocated over the life cycle of the program**。Even if the function is called multiple times, the space for the static variable is allocated**only once**，The value of the variable in the previous call is passed through the next function call. This is useful in C / C + + or any other application that needs to store the state of previous functions. 

```c++
#include <iostream> 
#include <string> 
using namespace std; 

void demo() 
{ 
	// static variable 
	static int count = 0; 
	cout << count << " "; 
	
	// value is updated and 
	// will be carried to next 
	// function calls 
	count++; 
} 

int main() 
{ 
	for (int i=0; i<5; i++)	 
		demo(); 
	return 0; 
} 
```

Output：

```
0 1 2 3 4 
```

You can see in the above program that the variable count is declared static.So, Its value is passed through a function call. The variable count is not initialized each time the function is called.

- Static variables in class

Because variables declared static are initialized only once，Because they allocate space in separate static storage。Therefore, static variables in a class are **shared by objects。**You cannot have multiple copies of the same static variable for different objects. For this reason, static variables cannot be initialized with constructors.

```c++

#include<iostream> 
using namespace std; 

class Apple 
{ 
public: 
	static int i; 
	
	Apple() 
	{ 
		// Do nothing 
	}; 
}; 

int main() 
{ 
Apple obj1; 
Apple obj2; 
obj1.i =2; 
obj2.i = 3; 
	
// prints value of i 
cout << obj1.i<<" "<<obj2.i; 
} 
```

You can see in the above program that we have tried to create multiple copies of static variable i for multiple objects. But it didn't happen.
Therefore, static variables in a class should be explicitly initialized by the user using class names and range resolution operators outside the class, as shown below

```c++

#include<iostream> 
using namespace std; 

class Apple 
{ 
public: 
	static int i; 
	
	Apple() 
	{ 
		// Do nothing 
	}; 
}; 

int Apple::i = 1; 

int main() 
{ 
	Apple obj; 
	// prints value of i 
	cout << obj.i; 
} 
```

Output：

```
1
```

**Static member**

- Class objects are static

Just like variables, objects have scope when declared static until the life cycle of the program

Consider the following program, where the object is non static

```c++
#include<iostream> 
using namespace std; 

class Apple 
{ 
	int i; 
	public: 
		Apple() 
		{ 
			i = 0; 
			cout << "Inside Constructor\n"; 
		} 
		~Apple() 
		{ 
			cout << "Inside Destructor\n"; 
		} 
}; 

int main() 
{ 
	int x = 0; 
	if (x==0) 
	{ 
		Apple obj; 
	} 
	cout << "End of main\n"; 
} 

```


Output：

```c++
Inside Constructor
Inside Destructor
End of main
```

In the above program, the object is declared non static within the if block. Therefore, the scope of the variable is only within the if block. Therefore, when an object is created, the constructor is called, and is called when the control of the if block passes over the destructor, because the scope of the object is only within the if block in which it is declared.

If we declare the object static, now let's look at the changes in the output.

```c++
#include<iostream> 
using namespace std; 

class Apple 
{ 
	int i; 
	public: 
		Apple() 
		{ 
			i = 0; 
			cout << "Inside Constructor\n"; 
		} 
		~Apple() 
		{ 
			cout << "Inside Destructor\n"; 
		} 
}; 

int main() 
{ 
	int x = 0; 
	if (x==0) 
	{ 
		static Apple obj; 
	} 
	cout << "End of main\n"; 
} 

```


Output：

```
Inside Constructor
End of main
Inside Destructor
```

You can clearly see the output change. Now, call the destructor function after the end of main.This is because the scope of static objects is throughout the life cycle of the program.

- Static functions in class

Just like static data members or static variables in a class, Static member functions are also independent of class objects.We are allowed to use objects and "." to call static member functions. However, it is recommended to call static members using class names and range resolution operators.

Allows static member functions to access only static data members or other static member functions that cannot access non static data members or member functions of a class.

```c++
#include<iostream> 
using namespace std; 

class Apple 
{ 
    public: 
        // static member function 
        static void printMsg() 
        {
            cout<<"Welcome to Apple!"; 
        }
}; 

// main function 
int main() 
{ 
    // invoking a static member function 
    Apple::printMsg(); 
} 
```

Output：

```
Welcome to Apple!
```

