#ifndef LL_HI3220_INIT_HPP
#define LL_HI3220_INIT_HPP

#include <ll_HI3220_common.hpp>



/**
 * @name init_SPI_pins
 * @brief Initializes all necessary pins for SPI communication with the A429 shield.
 * @details
 * 1. Calling `init_SPI_MRST_pin()` to set the Master Reset (MRST) pin as OUTPUT.
 * 2. Calling `init_SPI_HCS_pin()` to configure the Hardware Chip Select (HCS) pin as OUTPUT.
 * 3. Calling `init_SPI_READY_pin()` to set the READY pin as INPUT.
 * 4. Calling `init_SPI_RUN_pin()` to set the RUN pin as OUTPUT.
 */
void init_SPI_pins();


int HI3220_set_state(int state);

/**
 * @name init_SPI_settings
 * @brief Initializes the SPI settings for communication with the A429 shield (HI-3220).
 * @details
 * 1. Initializes the SPI bus with specified SCK (clock), MISO (Master In Slave Out), and MOSI (Master Out Slave In) pins by calling `SPI.begin`.
 * 2. Begins a SPI transaction with the desired settings, including clock frequency, data order, and data mode, by calling `SPI.beginTransaction`.
 * 3. Sets the HCS pin to HIGH, indicating that the SPI settings are initialized and the device is ready to start communication.
 * @param {double} clock_frequency - The clock frequency for the SPI communication. Recommendation : 40MHz (max value)
 * @param {unsigned char} data_order - The bit order for data transmission. MSBFIRST or LSBFIRST. Recommendation : MSBFIRST
 * @param {unsigned char} data_mode - The SPI data mode (0, 1, 2, or 3) defining the clock polarity and phase. Recommendation : Mode 0
 */
void init_SPI_settings(double clock_frequency, unsigned char data_order, unsigned char data_mode);



#endif