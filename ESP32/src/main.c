//--------------------------------INCLUDES BEGIN----------------------------------//

// C standard includes
#include <stdio.h>
#include <string.h>

// Zephyr includes
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/lora.h>
#include <zephyr/drivers/display.h>
#include <zephyr/display/cfb.h>
#include <zephyr/logging/log.h>
#include <zephyr/random/random.h>

// LVGL includes
#include <lvgl.h>

// Project specific includes
#include "my_lib.h"

// Project module includes
#ifdef CONFIG_TIMEBASE
#include "timeBase.h"
#endif

//Register logging module
#define LOG_LEVEL CONFIG_LOG_DEBUG
LOG_MODULE_REGISTER(aether_main);

//--------------------------------DT ALIASES BEGIN--------------------------------//
#define BUTTON_NODE DT_ALIAS(button1)
#define GPS_PPS_NODE DT_ALIAS(pps0)
#define LORA_NODE DT_ALIAS(lora0)
#define OLED_NODE DT_ALIAS(oled0)


//--------------------------------GPIO SPEC BEGIN----------------------------------//
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET(BUTTON_NODE, gpios);

static const struct gpio_dt_spec gps_pps = GPIO_DT_SPEC_GET(GPS_PPS_NODE, gpios);


//--------------------------------LORA SPEC BEGIN----------------------------------//
static const struct device *lora_dev = DEVICE_DT_GET(LORA_NODE);


//--------------------------------DISPLAY SPEC BEGIN--------------------------------//
static const struct device *oled_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));


//--------------------------------CONFIG STRUCTS BEGIN------------------------------//
// LoRa device configuration
struct lora_modem_config lora_cfg = {
    .frequency = 915000000,
    .bandwidth = BW_125_KHZ,
    .datarate = SF_12,
    .coding_rate = CR_4_8,
    .preamble_len = 8,
    .tx_power = 4,
    .tx = true,
    .iq_inverted = false,
    .public_network = false,
    .packet_crc_disable = false
};

struct display_buffer_descriptor buf_desc = {
    .buf_size = 128 * 64 / 8,
    .width = 128,
    .height = 64,
    .pitch = 128,
    .frame_incomplete = false
};

uint8_t display_buffer[64 * 128 / 8];


//--------------------------------USER CODE BEGIN----------------------------------//
int main() {

    // Variable to hold return values
    int ret;

    //--------------------------------DEVICE READY CHECKS BEGIN----------------------------//
    // Check if button device is ready
    if (!gpio_is_ready_dt(&button)){
        printk("Button device not ready\n");
        return 0;
    }

    // Check if GPS PPS device is ready
    if (!gpio_is_ready_dt(&gps_pps)){
        printk("Button device not ready\n");
        return 0;
    }

    // Check if LoRa device is ready
    if (!device_is_ready(lora_dev)){
        printk("LoRa device not ready\n");
        return 0;
    }

    // Check if OLED device is ready
    if (!device_is_ready(oled_dev)){
        printk("OLED device not ready\n");
        return 0;
    }
    // Disable OLED display blanking
    display_blanking_off(oled_dev);


    //--------------------------------PIN CONFIGURATION BEGIN------------------------------//
    // Configure button pin as input
    ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
    if (ret < 0){
        printk("Error %d: failed to configure button pin\n", ret);
        return 0;
    }

    // Configure GPS PPS pin as input
    ret = gpio_pin_configure_dt(&gps_pps, GPIO_INPUT);
    if (ret < 0){
        printk("Error %d: failed to configure GPS PPS pin\n", ret);
        return 0;
    }


    //--------------------------------LORA CONFIGURATION BEGIN------------------------------//
    // Configure LoRa device
    ret = lora_config(lora_dev, &lora_cfg);
    if (ret < 0){
        printk("Error %d: failed to configure LoRa device\n", ret);
        return 0;
    }
	
    //--------------------------------LVGL GUI SETUP----------------------------------------//
    lv_obj_t *hello_label;

    // Create a static label widget
    hello_label = lv_label_create(lv_scr_act());
    lv_label_set_text(hello_label, "Preoject_Aether\nLVGL");
    lv_obj_align(hello_label, LV_ALIGN_TOP_MID, 0, 5);

	display_blanking_off(oled_dev);

    while(1){

        // Handle LVGL tasks
        lv_task_handler();

        // Read button state
        int val = gpio_pin_get_dt(&button);
        if (val < 0){
            printk("Error %d: failed to read button pin\n", val);
            return 0;
        }

        // Read GPS PPS state
        int ppsState = gpio_pin_get_dt(&gps_pps);
        if (ppsState < 0){
            printk("Error %d: failed to read GPS PPS pin\n", ppsState);
            return 0;
        }

        // Print button state
        printk("Button state: %s\n", val ? "PRESSED" : "RELEASED");

        // (Disabled) transmit LoRa message upon PPS signal
        if (0){
            char message[] = "PPS Signal Detected";
            ret = lora_send(lora_dev, (uint8_t*)message, sizeof(message));
            if(ret < 0){
                printk("Error %d: failed to send LoRa message\n", ret);
            } else {
                printk("LoRa message sent: %s\n", message);
            }
        }
        
        // cfb_framebuffer_init(oled_dev);
        // cfb_set_kerning(oled_dev, 1);
        // for(int fontIndex = 0; fontIndex < cfb_get_numof_fonts(oled_dev); fontIndex++){
        //     cfb_framebuffer_clear(oled_dev, 0);
        //     cfb_framebuffer_set_font(oled_dev, fontIndex);
        //     cfb_print(oled_dev, "Hello", 0, 0);
        //     cfb_framebuffer_finalize(oled_dev);
        // k_msleep(1000);
        // }

        // Display to OLED when PPS signal active
        //display_write(oled_dev, 0, 0, &buf_desc, display_buffer);

        k_msleep(10);

    }

    // should never reach here
    return 0;
}

