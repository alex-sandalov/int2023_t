#include <lib/number.h>
#include <iostream>

int main() {

    int2023_t a = from_string("405272312330606683982498447530407677486444946329741974138101544027695953739965");
    int2023_t b = from_string("3626777458843887524118528");
    int2023_t c = from_string("111744466521471062588629470729710044638866394325887658");
    std::cout << ((a / b) == c) << '\n';
   // std::cout << ((a / b) == c) << '\n';
   // std::cout << a / b << '\n' << c << '\n';

    return 0;
}
