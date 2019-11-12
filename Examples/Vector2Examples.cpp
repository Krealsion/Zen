//
// Created by jdemoss on 11/7/19.
//

#include <iostream>
#include "../include/Vector2.h"

int main(){
    Vector2 a;
    std::cout << a << std::endl;
    Vector2 b = Vector2(10,5);
    std::cout << b << std::endl;
    b.Add(b).Add(b);
    std::cout << b << std::endl;
    //Use copy in order to preserve data
    std::cout << b.Copy().Normalize() << std::endl;
    std::cout << b << std::endl;
    std::cout << b.Invert() << std::endl;
    std::cout << b << std::endl;
    Vector2 c = Vector2(-1, 0);
    std::cout << b.Copy().Multiply(c).Normalize() << std::endl;
}
