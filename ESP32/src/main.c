#include <stdio.h>

// Zephyr includes
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include "zephyr/random/random.h"

// Project specific includes
#include "my_lib.h"

// Project module includes
#ifdef CONFIG_TIMEBASE
#include "timeBase.h"
#endif

static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET(DT_ALIAS(button1), gpios);

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led1), gpios);

int main() {

    int ret;

    if (!gpio_is_ready_dt(&button)){
        printk("Button device not ready\n");
        return 0;
    }

    if (!gpio_is_ready_dt(&led)){
        printk("LED device not ready\n");
        return 0;
    }

    //Configure button pin as input
    ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
    if (ret < 0){
        printk("Error %d: failed to configure button pin\n", ret);
        return 0;
    }

    //Configure LED pin as output
    ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
    if (ret < 0){
        printk("Error %d: failed to configure LED pin\n", ret);
        return 0;
    }

    while(1){

        int val = gpio_pin_get_dt(&button);
        if (val < 0){
            printk("Error %d: failed to read button pin\n", val);
            return 0;
        }

        //Set LED state to match button state
        gpio_pin_set_dt(&led, val);

        printk("Button state: %s\n", val ? "PRESSED" : "RELEASED");

        k_msleep(100);
    }

    //Do forever
    while(1){

        //Generate and print psuedo-random number
        uint32_t randNum = sys_rand32_get();

        printk("Random Number: %u\n", randNum);

        //Call hello function from my_lib
        say_hello();

        // Call the timeBase module function
        #ifdef CONFIG_TIMEBASE
        printTimeBaseMessage();
        #endif

        //Sleep for 1 second
        k_msleep(1000);

    }

    return 0;
}