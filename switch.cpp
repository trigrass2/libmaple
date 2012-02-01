// Sample main.cpp file. Blinks the built-in LED, sends a message out
// USART1.

#include "wirish.h"
#include "switch.h"
#include "power.h"

// for power control support
#include "pwr.h"
#include "scb.h"
#include "exti.h"
#include "gpio.h"

#define MANUAL_WAKEUP_GPIO 18 // PC3


static void
switch_change(void)
{
    //    init();
    //    Serial1.begin(115200);
    
    delay(10); /* Debounce */
    if (switch_state(&back_switch)) {
        Serial1.println("Powering board on!");
        delay(20); /* Let I2C come up */
        power_set_state(PWRSTATE_USER);
    }
    else {
        Serial1.println("Powering board off!");
        power_set_state(PWRSTATE_LOG);
    }
}

static int
switch_init(void)
{
    pinMode(MANUAL_WAKEUP_GPIO, INPUT);
    return 0;
}


static int
switch_suspend(struct device *dev) {
    return 0;
}


static int
switch_deinit(struct device *dev)
{
    return 0;
}

static int
switch_resume(struct device *dev)
{
    attachInterrupt(MANUAL_WAKEUP_GPIO, switch_change, CHANGE);
    return 0;
}

int
switch_state(struct device *dev)
{
    return digitalRead(MANUAL_WAKEUP_GPIO) == HIGH;
}

struct device back_switch = {
    switch_init,
    switch_deinit,
    switch_suspend,
    switch_resume,

    "Back Switch",
};
