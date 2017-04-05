/*
 * Copyright (C) 2017 Mesotic SAS
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    drivers_adxl345 ADXL345 3-Axis accelerometer
 * @ingroup     drivers_sensors
 * @brief       Device driver interface for the ADXL345
 * @{
 *
 * @file
 * @brief       Interface definition for the ADXL345
 *
 * @author     Dylan Laduranty <dylan.laduranty@mesotic.com>
 */

#ifndef ADXL345_H
#define ADXL345_H

#ifdef __cplusplus
extern "C" {
#endif

#include "periph/i2c.h"

/**
 * @brief Possible ADXL345 hardware addresses (wiring specific)
 */
enum {
    ADXL345_ADDR_1D = 0x1D, /**< I2C device address if SDO / Alt address pin is high */
    ADXL345_ADDR_53 = 0x53, /**< I2C device address if SDO / Alt address pin is low */
};

/**
 * @brief Define ADXL345 sensitivity
 */
enum {
    ADXL345_RANGE_2G,      /**< +/- 2 g Full Scale Rang */
    ADXL345_RANGE_4G,      /**< +/- 4 g Full Scale Rang */
    ADXL345_RANGE_8G,      /**< +/- 8 g Full Scale Rang */
    ADXL345_RANGE_16G      /**< +/- 16 g Full Scale Rang */
};

/**
 * @brief List bandwidth rate
 */
enum {
    ADXL345_RATE_0HZ1 = 0,    /**< 0.1 Hz Output Data Rate */
    ADXL345_RATE_0HZ2 = 1,    /**< 0.2 Hz Output Data Rate */
    ADXL345_RATE_0HZ39 = 2,   /**< 0.39 Hz Output Data Rate */
    ADXL345_RATE_0HZ78 = 3,   /**< 0.78 Hz Output Data Rate */
    ADXL345_RATE_1HZ56 = 4,   /**< 1.56 Hz Output Data Rate */
    ADXL345_RATE_3HZ13 = 5,   /**< 3.13 Hz Output Data Rate */
    ADXL345_RATE_6HZ25 = 6,   /**< 6.25 Hz Output Data Rate */
    ADXL345_RATE_12HZ50 = 7,  /**< 12.5 Hz Output Data Rate */
    ADXL345_RATE_25HZ = 8,    /**< 25 Hz Output Data Rate */
    ADXL345_RATE_50HZ = 9,    /**< 50 Hz Output Data Rate */
    ADXL345_RATE_100HZ = 10,  /**< 100 Hz Output Data Rate */
    ADXL345_RATE_200HZ = 11,  /**< 200 Hz Output Data Rate */
    ADXL345_RATE_400HZ = 12,  /**< 400 Hz Output Data Rate */
    ADXL345_RATE_800HZ = 13,  /**< 800 Hz Output Data Rate */
    ADXL345_RATE_1600HZ = 14, /**< 1600 Hz Output Data Rate */
    ADXL345_RATE_3200HZ = 15  /**< 3200 Hz Output Data Rate */
};

/**
 * @brief List fifo mode
 */
enum {
    BYPASS  = 0,          /**< FIFO bypass mode */
    FIFO    = 1,          /**< FIFO mode */
    STREAM  = 2,          /**< FIFO stream mode */
    TRIGGER = 3           /**< FIFO trigger mode */
};
/**
 * @brief Output Interrupt selection
 */
enum {
    INT1,  /**< Output interrupt on INT1 pin */
    INT2   /**< Output interrupt on INT2 pin */
};

/**
 * @brief   Named return values
 */
enum {
    ADXL345_OK          =  0,       /**< everything was fine */
    ADXL345_DATA_READY  =  1,       /**< new data ready to be read */
    ADXL345_NOI2C       = -1,       /**< I2C communication failed */
    ADXL345_NODEV       = -2,       /**< no ADXL345 device found on the bus */
    ADXL345_NODATA      = -3        /**< no data available */
};

/**
 * @brief ADXL345 result vector struct
 */
typedef struct {
    int16_t x;             /**< X-Axis measurement result */
    int16_t y;             /**< Y-Axis measurement result */
    int16_t z;             /**< Z-Axis measurement result */
} adxl345_data_t;

/**
 * @brief Interrupt configuration struct for the ADXL345 sensor
 */
typedef struct {
    uint8_t source;       /**< Source of interrupts */
    uint8_t map;          /**< Interrupt mapping control */
    uint8_t enable;       /**< Interrupt enable control */
    uint8_t thres_tap;    /**< Tap threshold */
    uint8_t thres_dur;    /**< Tap duration */
    uint8_t thres_latent; /**< Tap latency */
    uint8_t thres_window; /**< Tap window */
    uint8_t thres_act;    /**< Activity threshold */
    uint8_t thres_inact;  /**< Inactivity threshold */
    uint8_t time_inact;   /**< Inactivity time */
    uint8_t thres_ff;     /**< Free-fall threshold */
    uint8_t time_ff;      /**< Time threshold */
    uint8_t act_inact;    /**< Axis enable control for activity and inactivity detection */
    uint8_t tap_axes;     /**< Axis control for single tap/double tap */
} adxl345_interrupt_t;

/**
 * @brief Configuration struct for the ADXL345 sensor
 */
typedef struct {
    i2c_t i2c;                     /**< I2C device which is used */
    uint8_t addr;                  /**< I2C address */
    adxl345_interrupt_t interrupt; /**< Interrupts configuration  */
    uint8_t offset[3];             /**< offset axis */
    uint8_t range;                 /**< Sensitivity configuration */
    uint8_t rate;                  /**< Configured sample rate for accel  */
    uint8_t full_res;              /**< Resolution bit */
    uint8_t scale_factor;          /**< Scale factor for converting value to mg */
} adxl345_params_t;

/**
 * @brief Device descriptor for the ADXL345 sensor
 */
typedef struct {
    adxl345_params_t params;      /**< Device configuration */
} adxl345_t;

/**
 * @brief   Initialize the ADXL345 accelerometer driver.
 *
 * @param[out] dev          device descriptor of accelerometer to initialize
 * @param[in]  params       configuration parameters
 *
 * @return                  ADXL345_OK on success
 * @return                  ADXL345_NOI2C if initialization of I2C bus failed
 * @return                  ADXL345_NODEV if accelerometer test failed
 */
int adxl345_init(adxl345_t *dev, const adxl345_params_t* params);

/**
 * @brief   Read accelerometer's data
 *
 * Acceleration will be calculated as:<br>
 *     \f$ accel = {value \times 3.9} \f$ if full scale is set to 2g<br>
 *     \f$ accel = {value \times 7.8} \f$ if full scale is set to 4g<br>
 *     \f$ accel = {value \times 15.6} \f$ if full scale is set to 8g<br>
 *     \f$ accel = {value \times 31.2} \f$ if full scale is set to 16g<br>
 *
 * @param[in]  dev          device descriptor of accelerometer
 * @param[out] data         the current acceleration data [in mg]
 */
void adxl345_read(adxl345_t *dev, adxl345_data_t *data);

/**
 * @brief   set ADXL345's interrupts configuration
 *
 * @param[in]  dev          device descriptor of accelerometer
 */
void adxl345_set_interrupt(adxl345_t *dev);

/**
 * @brief   set ADXL345's measure mode
 *
 * @param[in]  dev          device descriptor of accelerometer
 */
void adxl345_set_measure(adxl345_t *dev);

/**
 * @brief   Set standby mode
 *
 * @param[in]  dev          device descriptor of accelerometer
 */
void adxl345_set_standby(adxl345_t *dev);

/**
 * @brief   Set sleep mode
 *
 * @param[in]  dev          device descriptor of accelerometer
 */
void adxl345_set_sleep(adxl345_t *dev);

/**
 * @brief   Set autosleep mode
 *
 * @param[in]  dev          device descriptor of accelerometer
 */
void adxl345_set_autosleep(adxl345_t *dev);

/**
 * @brief   Set bandwidth rate
 *
 * @param[in]  dev          device descriptor of accelerometer
 * @param[in]  bw_rate      new datarate
 */
void adxl345_set_bandwidth_rate(adxl345_t *dev, uint8_t bw_rate);

/**
 * @brief   Set fifo mode with its configuration.
 *
 * @param[in]  dev          device descriptor of accelerometer
 * @param[in]  mode         fifo mode configuration
 * @param[in]  output       set trigger output
 * @param[in]  value        set trigger's value
 */
void adxl345_set_fifo_mode(adxl345_t *dev, uint8_t mode,
                           uint8_t output, uint8_t value);

#ifdef __cplusplus
}
#endif

#endif /* ADXL345_H */
/** @} */