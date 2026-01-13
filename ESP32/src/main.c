// Zephyr includes
#include "zephyr/random/random.h"

// Project includes
#include "my_lib.h"

int main() {

    //Do forever
    while(1){

        //Generate and print psuedo-random number
        uint32_t randNum = sys_rand32_get();

        printk("Random Number: %u\n", randNum);

        //Call hello function from my_lib
        say_hello();

        //Sleep for 1 second
        k_msleep(1000);

    }

    return 0;
}