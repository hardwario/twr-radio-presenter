#include <application.h>

// Service mode interval defines how much time
#define SERVICE_MODE_INTERVAL (15 * 60 * 1000)
#define BATTERY_UPDATE_INTERVAL (60 * 60 * 1000)
#define TEMPERATURE_PUB_INTERVAL (15 * 60 * 1000)
#define TEMPERATURE_PUB_DIFFERENCE 0.2f
#define TEMPERATURE_UPDATE_SERVICE_INTERVAL (1 * 1000)
#define TEMPERATURE_UPDATE_NORMAL_INTERVAL (10 * 1000)
#define ACCELEROMETER_UPDATE_SERVICE_INTERVAL (1 * 1000)
#define ACCELEROMETER_UPDATE_NORMAL_INTERVAL (10 * 1000)

// LED instance
bc_led_t led;

// Button instance
bc_button_t button;

// Thermometer instance
bc_tmp112_t tmp112;

// Accelerometer instance
bc_lis2dh12_t lis2dh12;
bc_lis2dh12_result_g_t result;


// Dice instance
bc_dice_t dice;

// Counters for button events
uint16_t button_click_count = 0;
uint16_t button_hold_count = 0;

// Time of button has press
bc_tick_t tick_start_button_press;
// Flag for button hold event
bool button_hold_event;

// Time of next temperature report
bc_tick_t tick_temperature_report = 0;

// This function dispatches button events
void button_event_handler(bc_button_t *self, bc_button_event_t event, void *event_param)
{
    if (event == BC_BUTTON_EVENT_CLICK)
    {
        // Pulse LED for 100 milliseconds
        bc_led_pulse(&led, 100);

        // Increment press count
        button_click_count++;

        bc_log_info("APP: Publish button press count = %u", button_click_count);

        bc_radio_pub_acceleration(&(result.x_axis), &(result.y_axis), &(result.z_axis));
    }
    else if (event == BC_BUTTON_EVENT_HOLD)
    {
        // Pulse LED for 250 milliseconds
        bc_led_pulse(&led, 250);

        // Increment hold count
        button_hold_count++;

        bc_log_info("APP: Publish button hold count = %u", button_hold_count);

        // Publish message on radio
        bc_radio_pub_event_count(BC_RADIO_PUB_EVENT_HOLD_BUTTON, &button_hold_count);

        // Set button hold event flag
        button_hold_event = true;
    }
    else if (event == BC_BUTTON_EVENT_PRESS)
    {
        // Reset button hold event flag
        button_hold_event = false;

        tick_start_button_press = bc_tick_get();
    }
    else if (event == BC_BUTTON_EVENT_RELEASE)
    {
        if (button_hold_event)
        {
            int hold_duration = bc_tick_get() - tick_start_button_press;

            bc_radio_pub_value_int(BC_RADIO_PUB_VALUE_HOLD_DURATION_BUTTON, &hold_duration);
        }
    }
}

// This function dispatches battery events
void battery_event_handler(bc_module_battery_event_t event, void *event_param)
{
    // Update event?
    if (event == BC_MODULE_BATTERY_EVENT_UPDATE)
    {
        float voltage;

        // Read battery voltage
        if (bc_module_battery_get_voltage(&voltage))
        {
            bc_log_info("APP: Battery voltage = %.2f", voltage);

            // Publish battery voltage
            bc_radio_pub_battery(&voltage);
        }
    }
}

// This function dispatches accelerometer events
void lis2dh12_event_handler(bc_lis2dh12_t *self, bc_lis2dh12_event_t event, void *event_param)
{
    // Update event?
    if (event == BC_LIS2DH12_EVENT_UPDATE)
    {
        // Successfully read accelerometer vectors?
        if (bc_lis2dh12_get_result_g(self, &result))
        {
            bc_log_info("APP: Acceleration = [%.2f,%.2f,%.2f]", result.x_axis, result.y_axis, result.z_axis);
        }
    }
    // Error event?
    else if (event == BC_LIS2DH12_EVENT_ERROR)
    {
        bc_log_error("APP: Accelerometer error");
    }
}

// This function is run as task and exits service mode
void exit_service_mode_task(void *param)
{
    // Set thermometer update interval to normal
    bc_tmp112_set_update_interval(&tmp112, TEMPERATURE_UPDATE_NORMAL_INTERVAL);

    // Set accelerometer update interval to normal
    bc_lis2dh12_set_update_interval(&lis2dh12, ACCELEROMETER_UPDATE_NORMAL_INTERVAL);

    // Unregister current task (it has only one-shot purpose)
    bc_scheduler_unregister(bc_scheduler_get_current_task_id());
}

void application_init(void)
{
    // Initialize log
    bc_log_init(BC_LOG_LEVEL_INFO, BC_LOG_TIMESTAMP_ABS);
    bc_log_info("APP: Reset");

    // Initialize LED
    bc_led_init(&led, BC_GPIO_LED, false, false);
    bc_led_set_mode(&led, BC_LED_MODE_OFF);

    // Initialize button
    bc_button_init(&button, BC_GPIO_BUTTON, BC_GPIO_PULL_DOWN, false);
    bc_button_set_event_handler(&button, button_event_handler, NULL);

    // Initialize battery
    bc_module_battery_init();
    bc_module_battery_set_event_handler(battery_event_handler, NULL);
    bc_module_battery_set_update_interval(BATTERY_UPDATE_INTERVAL);

    // Initialize accelerometer
    bc_lis2dh12_init(&lis2dh12, BC_I2C_I2C0, 0x19);
    bc_lis2dh12_set_event_handler(&lis2dh12, lis2dh12_event_handler, NULL);
    bc_lis2dh12_set_update_interval(&lis2dh12, ACCELEROMETER_UPDATE_SERVICE_INTERVAL);

    // Initialize dice
    bc_dice_init(&dice, BC_DICE_FACE_UNKNOWN);

    // Initialize radio
    bc_radio_init(BC_RADIO_MODE_NODE_SLEEPING);

    // Send radio pairing request
    bc_radio_pairing_request("push-button", VERSION);

    bc_scheduler_register(exit_service_mode_task, NULL, SERVICE_MODE_INTERVAL);

    // Pulse LED
    bc_led_pulse(&led, 2000);
}
