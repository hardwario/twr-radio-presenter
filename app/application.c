#include <application.h>

// Service mode interval defines how much time
#define SERVICE_MODE_INTERVAL (15 * 60 * 1000)
#define BATTERY_UPDATE_INTERVAL (60 * 60 * 1000)
#define ACCELEROMETER_UPDATE_NORMAL_INTERVAL (200)

// Button instance
bc_button_t button;

// Accelerometer instance
bc_lis2dh12_t lis2dh12;
bc_lis2dh12_result_g_t result;

// This function dispatches button events
void button_event_handler(bc_button_t *self, bc_button_event_t event, void *event_param)
{
    if (event == BC_BUTTON_EVENT_CLICK)
    {
        bc_radio_pub_acceleration(&(result.x_axis), &(result.y_axis), &(result.z_axis));
    }
    else if (event == BC_BUTTON_EVENT_HOLD)
    {
        // Publish message on radio
        bc_radio_pub_acceleration_hold(&(result.x_axis), &(result.y_axis), &(result.z_axis));
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
}

bool bc_radio_pub_acceleration_hold(float *x_axis, float *y_axis, float *z_axis)
{
    char stringBuffer[25];
    int pointer;

    pointer = snprintf(stringBuffer, sizeof(stringBuffer), "[%.1f,%.1f,%.1f]", *x_axis, *y_axis, *z_axis);

    return bc_radio_pub_string("accelerometer/-/acceleration_hold", stringBuffer);
}

void application_init(void)
{
    // Initialize log
    bc_log_init(BC_LOG_LEVEL_INFO, BC_LOG_TIMESTAMP_ABS);
    bc_log_info("APP: Reset");

    // Initialize button
    bc_button_init(&button, BC_GPIO_BUTTON, BC_GPIO_PULL_DOWN, false);
    bc_button_set_hold_time(&button, 500);
    bc_button_set_event_handler(&button, button_event_handler, NULL);

    // Initialize battery
    bc_module_battery_init();
    bc_module_battery_set_event_handler(battery_event_handler, NULL);
    bc_module_battery_set_update_interval(BATTERY_UPDATE_INTERVAL);

    // Initialize accelerometer
    bc_lis2dh12_init(&lis2dh12, BC_I2C_I2C0, 0x19);
    bc_lis2dh12_set_resolution(&lis2dh12, BC_LIS2DH12_RESOLUTION_8BIT);
    bc_lis2dh12_set_event_handler(&lis2dh12, lis2dh12_event_handler, NULL);
    bc_lis2dh12_set_update_interval(&lis2dh12, ACCELEROMETER_UPDATE_NORMAL_INTERVAL);

    // Initialize radio
    bc_radio_init(BC_RADIO_MODE_NODE_SLEEPING);

    // Send radio pairing request
    bc_radio_pairing_request("radio-presenter", VERSION);
}
