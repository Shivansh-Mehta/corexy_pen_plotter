#include "shi_motor.h"
#include "shi_definitions.h"
#include <esp_log.h>
#include <esp_timer.h>

const char *motor_tag = "motor";
const char *virtual_motor_tag = "virtual_motor";
const char *kinematic_tag = "kinematic";

motor::motor(
    uint8_t i_id,
    gpio_num_t i_pin_a,
    gpio_num_t i_pin_b,
    gpio_num_t i_pin_en,
    ledc_timer_t i_timer,
    ledc_channel_t i_channel_a,
    ledc_channel_t i_channel_b,
    bool i_dir,
    bool i_en)
    : m_id(i_id),
      m_pin_a(i_pin_a),
      m_pin_b(i_pin_b),
      m_pin_en(i_pin_en),
      m_timer(i_timer),
      m_channel_a(i_channel_a),
      m_channel_b(i_channel_b),
      m_dir(i_dir),
      m_enable(i_en)
{
    ESP_LOGI(motor_tag, "%d constructing  ", m_id);

    esp_rom_gpio_pad_select_gpio(m_pin_a);
    gpio_set_direction(m_pin_a, GPIO_MODE_OUTPUT);
    esp_rom_gpio_pad_select_gpio(m_pin_b);
    gpio_set_direction(m_pin_b, GPIO_MODE_OUTPUT);
    esp_rom_gpio_pad_select_gpio(m_pin_en);
    gpio_set_direction(m_pin_en, GPIO_MODE_OUTPUT);
    gpio_set_level(m_pin_en, m_enable);

    m_pwm = 0;

    const ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_8_BIT,
        .timer_num = i_timer,
        .freq_hz = 1000,
        .clk_cfg = LEDC_AUTO_CLK};
    ledc_timer_config(&ledc_timer);

    const ledc_channel_config_t ledc_channel_a = {
        .gpio_num = i_pin_a,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = i_channel_a,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = i_timer,
        .duty = m_pwm,
        .hpoint = 0};
    ledc_channel_config(&ledc_channel_a);
    ledc_set_duty(LEDC_LOW_SPEED_MODE, i_channel_a, 0);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, i_channel_a);

    const ledc_channel_config_t ledc_channel_b = {
        .gpio_num = i_pin_b,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = i_channel_b,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = i_timer,
        .duty = m_pwm,
        .hpoint = 0};
    ledc_channel_config(&ledc_channel_b);
    ledc_set_duty(LEDC_LOW_SPEED_MODE, i_channel_b, 0);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, i_channel_b);
};

void motor::set_pwm(uint8_t i_pwm)
{
    if (m_pwm != i_pwm)
    {
        run(this->get_direction(), i_pwm);
        m_pwm = i_pwm;
        ESP_LOGI(motor_tag, "%d | PWM Value Set: %d", m_id, m_pwm);
    }
};

uint8_t motor::get_pwm()
{
    return m_pwm;
};

void motor::set_direction(bool i_direction)
{
    if (m_dir != i_direction)
    {
        uint8_t cache_pwm = m_pwm;
        set_pwm(0);
        m_dir = i_direction;
        ESP_LOGI(motor_tag, "%d | Direction Set: %d", m_id, m_dir);
        set_pwm(cache_pwm);
    }
};

bool motor::get_direction()
{
    return m_dir;
};

void motor::enable(bool i_enable)
{
    m_enable = i_enable;
    esp_rom_gpio_pad_select_gpio(m_pin_en);
    gpio_set_level(m_pin_en, i_enable);
    ESP_LOGI(motor_tag, "%d | Enable: %d", m_id, m_enable);
};

bool motor::is_enabled()
{
    return m_enable;
};

void motor::run(bool b_pin_a)
{
    uint8_t ramp = 0;
    if (b_pin_a)
    {
        ledc_set_duty(LEDC_LOW_SPEED_MODE, m_channel_b, 0);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, m_channel_b);
        while (ramp < m_pwm)
        {
            ledc_set_duty(LEDC_LOW_SPEED_MODE, m_channel_a, ramp++);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, m_channel_a);
            vTaskDelay(pdMS_TO_TICKS(10));
        }
    }
    else
    {
        ledc_set_duty(LEDC_LOW_SPEED_MODE, m_channel_a, 0);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, m_channel_a);
        while (ramp < m_pwm)
        {
            ledc_set_duty(LEDC_LOW_SPEED_MODE, m_channel_b, ramp++);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, m_channel_b);
            vTaskDelay(pdMS_TO_TICKS(10));
        }
    }
};

void motor::run(bool b_pin_a, uint8_t i_pwm)
{
    if (b_pin_a)
    {
        ledc_set_duty(LEDC_LOW_SPEED_MODE, m_channel_b, 0);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, m_channel_b);
        if (i_pwm > m_pwm)
        {
            while (i_pwm != m_pwm)
            {
                ledc_set_duty(LEDC_LOW_SPEED_MODE, m_channel_a, ++m_pwm);
                ledc_update_duty(LEDC_LOW_SPEED_MODE, m_channel_a);
                vTaskDelay(pdMS_TO_TICKS(10));
            }
        }
        else if (i_pwm < m_pwm)
        {
            while (i_pwm != m_pwm)
            {
                ledc_set_duty(LEDC_LOW_SPEED_MODE, m_channel_a, --m_pwm);
                ledc_update_duty(LEDC_LOW_SPEED_MODE, m_channel_a);
                vTaskDelay(pdMS_TO_TICKS(10));
            }
        }
    }
    else
    {
        ledc_set_duty(LEDC_LOW_SPEED_MODE, m_channel_a, 0);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, m_channel_a);
        if (i_pwm > m_pwm)
        {
            while (i_pwm != m_pwm)
            {
                ledc_set_duty(LEDC_LOW_SPEED_MODE, m_channel_b, ++m_pwm);
                ledc_update_duty(LEDC_LOW_SPEED_MODE, m_channel_b);
                vTaskDelay(pdMS_TO_TICKS(10));
            }
        }
        else if (i_pwm > m_pwm)
        {
            while (i_pwm != m_pwm)
            {
                ledc_set_duty(LEDC_LOW_SPEED_MODE, m_channel_b, --m_pwm);
                ledc_update_duty(LEDC_LOW_SPEED_MODE, m_channel_b);
                vTaskDelay(pdMS_TO_TICKS(10));
            }
        }
    }
};

virtual_motor::virtual_motor(
    uint8_t i_id,
    gpio_num_t i_pin_a_a,
    gpio_num_t i_pin_a_b,
    gpio_num_t i_pin_b_a,
    gpio_num_t i_pin_b_b,
    gpio_num_t i_pin_en_a,
    gpio_num_t i_pin_en_b,
    ledc_timer_t i_timer,
    ledc_channel_t i_channel_a,
    ledc_channel_t i_channel_b,
    bool i_dir,
    bool i_en)
    : m_id(i_id),
      m_pin_a_a(i_pin_a_a),
      m_pin_a_b(i_pin_a_b),
      m_pin_b_a(i_pin_b_a),
      m_pin_b_b(i_pin_b_b),
      m_pin_en_a(i_pin_en_a),
      m_pin_en_b(i_pin_en_b),
      m_timer(i_timer),
      m_channel_a(i_channel_a),
      m_channel_b(i_channel_b),
      m_dir(i_dir),
      m_enable(i_en)
{
    ESP_LOGI(virtual_motor_tag, "%d constructing  ", m_id);

    esp_rom_gpio_pad_select_gpio(m_pin_a_a);
    gpio_set_direction(m_pin_a_a, GPIO_MODE_OUTPUT);
    esp_rom_gpio_pad_select_gpio(m_pin_a_b);
    gpio_set_direction(m_pin_a_b, GPIO_MODE_OUTPUT);

    esp_rom_gpio_pad_select_gpio(m_pin_b_a);
    gpio_set_direction(m_pin_b_a, GPIO_MODE_OUTPUT);
    esp_rom_gpio_pad_select_gpio(m_pin_b_b);
    gpio_set_direction(m_pin_b_b, GPIO_MODE_OUTPUT);

    esp_rom_gpio_pad_select_gpio(m_pin_en_a);
    gpio_set_direction(m_pin_en_a, GPIO_MODE_OUTPUT);
    esp_rom_gpio_pad_select_gpio(m_pin_en_b);
    gpio_set_direction(m_pin_en_b, GPIO_MODE_OUTPUT);

    gpio_set_level(m_pin_en_a, m_enable);
    gpio_set_level(m_pin_en_b, m_enable);

    m_pwm = 0;

    const ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_8_BIT,
        .timer_num = i_timer,
        .freq_hz = 1000,
        .clk_cfg = LEDC_AUTO_CLK};
    ledc_timer_config(&ledc_timer);

    const ledc_channel_config_t ledc_channel_a_a = {
        .gpio_num = i_pin_a_a,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = i_channel_a,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = i_timer,
        .duty = m_pwm,
        .hpoint = 0};
    ledc_channel_config(&ledc_channel_a_a);

    const ledc_channel_config_t ledc_channel_b_a = {
        .gpio_num = i_pin_b_a,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = i_channel_a,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = i_timer,
        .duty = m_pwm,
        .hpoint = 0};
    ledc_channel_config(&ledc_channel_b_a);
    ledc_set_duty(LEDC_LOW_SPEED_MODE, i_channel_a, 0);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, i_channel_a);

    const ledc_channel_config_t ledc_channel_a_b = {
        .gpio_num = i_pin_a_b,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = i_channel_b,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = i_timer,
        .duty = m_pwm,
        .hpoint = 0};
    ledc_channel_config(&ledc_channel_a_b);

    const ledc_channel_config_t ledc_channel_b_b = {
        .gpio_num = i_pin_b_b,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = i_channel_b,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = i_timer,
        .duty = m_pwm,
        .hpoint = 0};
    ledc_channel_config(&ledc_channel_b_b);
    ledc_set_duty(LEDC_LOW_SPEED_MODE, i_channel_b, 0);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, i_channel_b);
};

void virtual_motor::set_pwm(uint8_t i_pwm)
{
    if (m_pwm != i_pwm)
    {
        m_pwm = i_pwm;
        ESP_LOGI(virtual_motor_tag, "%d | PWM Value Set: %d", m_id, m_pwm);
        run();
    }
};

uint8_t virtual_motor::get_pwm()
{
    return m_pwm;
};

void virtual_motor::set_direction(bool i_direction)
{
    if (m_dir != i_direction)
    {
        uint8_t cache_pwm = m_pwm;
        set_pwm(0);
        m_dir = i_direction;
        ESP_LOGI(virtual_motor_tag, "%d | Direction Set: %d", m_id, m_dir);
        set_pwm(cache_pwm);
    }
};

bool virtual_motor::get_direction()
{
    return m_dir;
};

void virtual_motor::enable(bool i_enable)
{
    m_enable = i_enable;
    esp_rom_gpio_pad_select_gpio(m_pin_en_a);
    gpio_set_level(m_pin_en_a, i_enable);
    esp_rom_gpio_pad_select_gpio(m_pin_en_b);
    gpio_set_level(m_pin_en_b, i_enable);
    ESP_LOGI(virtual_motor_tag, "%d | Enable: %d", m_id, m_enable);
};

bool virtual_motor::is_enabled()
{
    return m_enable;
};

void virtual_motor::run()
{
    uint8_t ramp = 0;
    if (m_dir)
    {
        ledc_set_duty(LEDC_LOW_SPEED_MODE, m_channel_b, 0);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, m_channel_b);
        while (ramp < m_pwm)
        {
            ledc_set_duty(LEDC_LOW_SPEED_MODE, m_channel_a, ramp++);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, m_channel_a);
            vTaskDelay(pdMS_TO_TICKS(10));
        }
    }
    else
    {
        ledc_set_duty(LEDC_LOW_SPEED_MODE, m_channel_a, 0);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, m_channel_a);
        while (ramp < m_pwm)
        {
            ledc_set_duty(LEDC_LOW_SPEED_MODE, m_channel_b, ramp++);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, m_channel_b);
            vTaskDelay(pdMS_TO_TICKS(10));
        }
    }
};

kinematic::kinematic(
    uint8_t i_id,
    kinematic_config i_config,
    gpio_num_t i_pin_a_a,
    gpio_num_t i_pin_a_b,
    gpio_num_t i_pin_b_a,
    gpio_num_t i_pin_b_b,
    gpio_num_t i_pin_en_a,
    gpio_num_t i_pin_en_b,
    ledc_timer_t i_timer,
    ledc_channel_t i_channel_a_a,
    ledc_channel_t i_channel_a_b,
    ledc_channel_t i_channel_b_a,
    ledc_channel_t i_channel_b_b,
    bool i_dir,
    bool i_en)
    : m_id(i_id),
      m_config(i_config),
      m_pin_a_a(i_pin_a_a),
      m_pin_a_b(i_pin_a_b),
      m_pin_b_a(i_pin_b_a),
      m_pin_b_b(i_pin_b_b),
      m_pin_en_a(i_pin_en_a),
      m_pin_en_b(i_pin_en_b),
      m_timer(i_timer),
      m_channel_a_a(i_channel_a_a),
      m_channel_a_b(i_channel_a_b),
      m_channel_b_a(i_channel_b_a),
      m_channel_b_b(i_channel_b_b),
      m_dir(i_dir),
      m_enable(i_en)
{
    ESP_LOGI(kinematic_tag, "%d | running kinematic() / constructor", m_id);
    m_pwm = 0;

    const ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_8_BIT,
        .timer_num = m_timer,
        .freq_hz = 1000,
        .clk_cfg = LEDC_AUTO_CLK};
    ledc_timer_config(&ledc_timer);

    const ledc_channel_config_t ledc_channel_a = {
        .gpio_num = m_pin_a_a,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = m_channel_a_a,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = m_timer,
        .duty = m_pwm,
        .hpoint = 0};
    ledc_channel_config(&ledc_channel_a);
    ledc_set_duty(LEDC_LOW_SPEED_MODE, m_channel_a_a, m_pwm);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, m_channel_a_a);

    const ledc_channel_config_t ledc_channel_b = {
        .gpio_num = m_pin_a_b,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = m_channel_a_b,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = m_timer,
        .duty = m_pwm,
        .hpoint = 0};
    ledc_channel_config(&ledc_channel_b);
    ledc_set_duty(LEDC_LOW_SPEED_MODE, m_channel_a_b, m_pwm);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, m_channel_a_b);

    const ledc_channel_config_t ledc_channel_c = {
        .gpio_num = m_pin_b_a,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = m_channel_b_a,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = m_timer,
        .duty = m_pwm,
        .hpoint = 0};
    ledc_channel_config(&ledc_channel_c);
    ledc_set_duty(LEDC_LOW_SPEED_MODE, m_channel_b_a, m_pwm);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, m_channel_b_a);

    const ledc_channel_config_t ledc_channel_d = {
        .gpio_num = m_pin_b_b,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = m_channel_b_b,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = m_timer,
        .duty = m_pwm,
        .hpoint = 0};
    ledc_channel_config(&ledc_channel_d);
    ledc_set_duty(LEDC_LOW_SPEED_MODE, m_channel_b_b, m_pwm);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, m_channel_b_b);

    enable(m_enable);
};

void kinematic::set_pwm(uint8_t i_pwm)
{
    ESP_LOGI(kinematic_tag, "%d | running set_pwm()", m_id);
    if (m_pwm != i_pwm)
    {
        m_pwm = i_pwm;
        ESP_LOGI(kinematic_tag, "%d | PWM Value Set: %d", m_id, m_pwm);
        run();
    }
};

uint8_t kinematic::get_pwm()
{
    ESP_LOGI(kinematic_tag, "%d | running get_pwm()", m_id);
    return m_pwm;
};

void kinematic::set_direction(bool i_direction)
{
    ESP_LOGI(kinematic_tag, "%d | running set_direction()", m_id);
    if (m_dir != i_direction)
    {
        uint8_t cache_pwm = m_pwm;
        set_pwm(0);
        m_dir = i_direction;
        ESP_LOGI(kinematic_tag, "%d | Direction Set: %d", m_id, m_dir);
        set_pwm(cache_pwm);
    }
};

bool kinematic::get_direction()
{
    ESP_LOGI(kinematic_tag, "%d | running get_direction()", m_id);
    return m_dir;
};

void kinematic::enable(bool i_enable)
{
    ESP_LOGI(kinematic_tag, "%d | running enable()", m_id);
    m_enable = i_enable;
    ESP_LOGI(kinematic_tag, "%d | Enable Status: %d", m_id, (int)m_enable);
    switch (m_config)
    {
    case X_AXIS:
    {
        ESP_LOGI(kinematic_tag, "%d : X_AXIS enabled", m_id);
        gpio_set_level(m_pin_en_a, m_enable);
        gpio_set_level(m_pin_en_b, m_enable);
        break;
    }
    case Y_AXIS:
    {
        ESP_LOGI(kinematic_tag, "%d : Y_AXIS enabled", m_id);
        gpio_set_level(m_pin_en_a, m_enable);
        gpio_set_level(m_pin_en_b, m_enable);
        break;
    }
    case QUAD13:
    {
        ESP_LOGI(kinematic_tag, "%d : QUAD13 enabled", m_id);
        gpio_set_level(m_pin_en_a, m_enable);
        gpio_set_level(m_pin_en_b, 0);
        break;
    }
    case QUAD24:
    {
        ESP_LOGI(kinematic_tag, "%d : QUAD24 enabled", m_id);
        gpio_set_level(m_pin_en_a, 0);
        gpio_set_level(m_pin_en_b, m_enable);
        break;
    }
    default:
    {
        ESP_LOGI(kinematic_tag, "%d : default enabled", m_id);
        gpio_set_level(m_pin_en_a, 0);
        gpio_set_level(m_pin_en_b, 0);
        break;
    }
    }
};

bool kinematic::is_enabled()
{
    ESP_LOGI(kinematic_tag, "%d | running is_enabled()", m_id);
    return m_enable;
};

void kinematic::run()
{
    ESP_LOGI(kinematic_tag, "%d | running run()", m_id);
    switch (m_config)
    {
    case X_AXIS:
    {
        run_X_AXIS();
        break;
    }
    case Y_AXIS:
    {
        run_Y_AXIS();
        break;
    }
    case QUAD13:
    {
        run_QUAD13();
        break;
    }
    case QUAD24:
    {
        run_QUAD24();
        break;
    }
    default:
    {
        ESP_LOGI(kinematic_tag, "%d : default running", m_id);
        run_X_AXIS();
        break;
    }
    }
};

void kinematic::set_kinematic_config(kinematic_config i_kinematic_config)
{
    ESP_LOGI(kinematic_tag, "%d | running set_kinematic_config()", m_id);
    m_config = i_kinematic_config;
};

kinematic_config kinematic::get_kinematic_config()
{
    ESP_LOGI(kinematic_tag, "%d | running get_kinematic_config()", m_id);
    return m_config;
};

void kinematic::run_X_AXIS()
{
    ESP_LOGI(kinematic_tag, "%d | running run_X_AXIS()", m_id);

    uint8_t ramp = 0;
    if (m_dir)
    {
        ledc_set_duty(LEDC_LOW_SPEED_MODE, m_channel_a_b, 0);
        ledc_set_duty(LEDC_LOW_SPEED_MODE, m_channel_b_b, 0);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, m_channel_a_b);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, m_channel_b_b);
        while (ramp < m_pwm)
        {
            ledc_set_duty(LEDC_LOW_SPEED_MODE, m_channel_a_a, ramp++);
            ledc_set_duty(LEDC_LOW_SPEED_MODE, m_channel_b_a, ramp++);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, m_channel_a_a);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, m_channel_b_a);
            vTaskDelay(pdMS_TO_TICKS(10));
        }
    }
    else
    {
        ledc_set_duty(LEDC_LOW_SPEED_MODE, m_channel_a_a, 0);
        ledc_set_duty(LEDC_LOW_SPEED_MODE, m_channel_b_a, 0);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, m_channel_a_a);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, m_channel_b_a);
        while (ramp < m_pwm)
        {
            ledc_set_duty(LEDC_LOW_SPEED_MODE, m_channel_a_b, ramp++);
            ledc_set_duty(LEDC_LOW_SPEED_MODE, m_channel_b_b, ramp++);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, m_channel_a_b);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, m_channel_b_b);
            vTaskDelay(pdMS_TO_TICKS(10));
        }
    }
};

void kinematic::run_Y_AXIS()
{
    ESP_LOGI(kinematic_tag, "%d | running run_Y_AXIS()", m_id);

    uint8_t ramp = 0;
    if (m_dir)
    {
        ledc_set_duty(LEDC_LOW_SPEED_MODE, m_channel_a_b, 0);
        ledc_set_duty(LEDC_LOW_SPEED_MODE, m_channel_b_a, 0);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, m_channel_a_b);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, m_channel_b_a);
        while (ramp < m_pwm)
        {
            ledc_set_duty(LEDC_LOW_SPEED_MODE, m_channel_a_a, ramp++);
            ledc_set_duty(LEDC_LOW_SPEED_MODE, m_channel_b_b, ramp++);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, m_channel_a_a);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, m_channel_b_b);
            vTaskDelay(pdMS_TO_TICKS(10));
        }
    }
    else
    {
        ledc_set_duty(LEDC_LOW_SPEED_MODE, m_channel_a_a, 0);
        ledc_set_duty(LEDC_LOW_SPEED_MODE, m_channel_b_b, 0);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, m_channel_a_a);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, m_channel_b_b);
        while (ramp < m_pwm)
        {
            ledc_set_duty(LEDC_LOW_SPEED_MODE, m_channel_a_b, ramp++);
            ledc_set_duty(LEDC_LOW_SPEED_MODE, m_channel_b_a, ramp++);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, m_channel_a_b);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, m_channel_b_a);
            vTaskDelay(pdMS_TO_TICKS(10));
        }
    }
};

void kinematic::run_QUAD13()
{
    ESP_LOGI(kinematic_tag, "%d | running run_QUAD13()", m_id);

    ledc_set_duty(LEDC_LOW_SPEED_MODE, m_channel_b_a, 0);
    ledc_set_duty(LEDC_LOW_SPEED_MODE, m_channel_b_b, 0);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, m_channel_b_a);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, m_channel_b_b);
    uint8_t ramp = 0;
    if (m_dir)
    {
        ledc_set_duty(LEDC_LOW_SPEED_MODE, m_channel_a_b, 0);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, m_channel_a_b);
        while (ramp < m_pwm)
        {
            ledc_set_duty(LEDC_LOW_SPEED_MODE, m_channel_a_a, ramp++);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, m_channel_a_a);
            vTaskDelay(pdMS_TO_TICKS(10));
        }
    }
    else
    {
        ledc_set_duty(LEDC_LOW_SPEED_MODE, m_channel_a_a, 0);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, m_channel_a_a);
        while (ramp < m_pwm)
        {
            ledc_set_duty(LEDC_LOW_SPEED_MODE, m_channel_a_b, ramp++);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, m_channel_a_b);
            vTaskDelay(pdMS_TO_TICKS(10));
        }
    }
};

void kinematic::run_QUAD24()
{
    ESP_LOGI(kinematic_tag, "%d | running run_QUAD24()", m_id);

    ledc_set_duty(LEDC_LOW_SPEED_MODE, m_channel_a_a, 0);
    ledc_set_duty(LEDC_LOW_SPEED_MODE, m_channel_a_b, 0);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, m_channel_a_a);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, m_channel_a_b);
    uint8_t ramp = 0;
    if (m_dir)
    {
        ledc_set_duty(LEDC_LOW_SPEED_MODE, m_channel_b_b, 0);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, m_channel_b_b);
        while (ramp < m_pwm)
        {
            ledc_set_duty(LEDC_LOW_SPEED_MODE, m_channel_b_a, ramp++);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, m_channel_b_a);
            vTaskDelay(pdMS_TO_TICKS(10));
        }
    }
    else
    {
        ledc_set_duty(LEDC_LOW_SPEED_MODE, m_channel_b_a, 0);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, m_channel_b_a);
        while (ramp < m_pwm)
        {
            ledc_set_duty(LEDC_LOW_SPEED_MODE, m_channel_b_b, ramp++);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, m_channel_b_b);
            vTaskDelay(pdMS_TO_TICKS(10));
        }
    }
};
