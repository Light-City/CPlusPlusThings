//
// Created by light on 19-12-9.
//

#include <iostream>

using namespace std;
// 在创建枚举时，将它们放在名称空间中，以便可以使用有意义的名称访问它们:
namespace EntityType {
    enum Enum {
        Ground = 0,
        Human,
        Aerial,
        Total
    };
}

void foo(EntityType::Enum entityType)
{
    if (entityType == EntityType::Ground) {
        /*code*/
    }
}

// enum class
enum class EntityType1 {
    Ground = 0,
    Human,
    Aerial,
    Total
};

void foo(EntityType1 entityType)
{
    if (entityType == EntityType1::Ground) {
        /*code*/
    }
}

int main() {

    return 0;
}
