#include <iostream>
#include "menu.h"
#include "Location.h"
using namespace std;

int main(){
    Location l1 = Location(51.5007, 0.1246);
    Location l2 = Location(40.6892, 74.0445);
    cout << l1.distance(l2);
    return 0;
}