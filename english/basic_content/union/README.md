# Story About Union




![](../img/wechat.jpg)

（Union）is a space saving special class.A union can have multiple data members.However, only one data member can have a value at any time. When a member is assigned a value, other members become undefined.

- The default access control character is public
- It can contain constructors and destructors
- Cannot contain a member of a reference type
- Cannot inherit from other classes, cannot be a base class
- Cannot contain virtual functions
- Anonymous union can directly access union members in the scope of definition
- Anonymous union cannot contain protected or private members
- Global anonymous union must be static


