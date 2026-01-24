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

static const struct gpio_dt_spec gps_pps = GPIO_DT_SPEC_GET(DT_ALIAS(pps0), gpios);

int main() {

    int ret;

    if (!gpio_is_ready_dt(&button)){
        printk("Button device not ready\n");
        return 0;
    }

    if (!gpio_is_ready_dt(&gps_pps)){
        printk("Button device not ready\n");
        return 0;
    }
    

    //Configure button pin as input
    ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
    if (ret < 0){
        printk("Error %d: failed to configure button pin\n", ret);
        return 0;
    }

    //Configure GPS PPS pin as input
    ret = gpio_pin_configure_dt(&gps_pps, GPIO_INPUT);
    if (ret < 0){
        printk("Error %d: failed to configure GPS PPS pin\n", ret);
        return 0;
    }

    while(1){

        int val = gpio_pin_get_dt(&button);
        if (val < 0){
            printk("Error %d: failed to read button pin\n", val);
            return 0;
        }

        int ppsState = gpio_pin_get_dt(&gps_pps);
        if (ppsState < 0){
            printk("Error %d: failed to read GPS PPS pin\n", ppsState);
            return 0;
        }

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