#ifndef LL_A429_SPI_INIT_HPP

#define LL_A429_SPI_INIT_HPP

#include <ll_A429_SPI_pin_init.hpp>

/**
 * @name init_SPI2_start_up_state
 * @brief Initializes the A429 shield in start-up state.
 * @details
 * 1. Sets the Master Reset (MRST) pin to LOW, initiating the reset process for the SPI2 interface.
 * 2. Waits for at least 225 nanoseconds to ensure the SPI2 interface is fully reset.
 * 3. Sets the MRST pin to HIGH, signaling the SPI2 interface to exit the reset state and prepare for normal operation.
 * 4. Waits for an additional 10 milliseconds to ensure the SPI2 interface and the A429 shield are stable and fully operational.
*/
void init_SPI2_start_up_state();

/**
 * @name init_SPI2_idle_state
 * @brief Initializes the A429 shield in idle state, preparing it to receive SPI instructions.
 * @details
 * 1. Calls `init_SPI2_start_up_state()` to initialize the SPI2 interface and ensure the A429 shield is in its start-up state.
 * 2. Checks the state of the SPI2_READY pin:
 *    - If the SPI2_READY pin is HIGH, it indicates that the A429 shield is in idle state, successfully prepared to receive SPI instructions.
 *    - If the SPI2_READY pin is LOW, it signifies that the A429 shield is not ready to receive SPI instructions, indicating a failure to properly initialize.
 * @returns - [-2] `READY` (-2) if the A429 shield is successfully initialized and ready to receive SPI instructions
 * @returns - [-1001] `NOT_READY` (-1001) if the A429 shield is NOT successfully initialized and NOT ready to receive SPI instructions
 */
int init_SPI2_idle_state();

/**
 * @name init_SPI2_settings
 * @brief Initializes the SPI2 settings for communication with the A429 shield.
 * @details
 * 1. Initializes the SPI2 bus with specified SCK (clock), MISO (Master In Slave Out), and MOSI (Master Out Slave In) pins by calling `SPI.begin`.
 * 2. Begins a SPI transaction with the desired settings, including clock frequency, data order, and data mode, by calling `SPI.beginTransaction`.
 * 3. Sets the SPI2_HCS (Hardware Chip Select) pin to HIGH, indicating that the SPI2 settings are initialized and the device is ready to start communication.
 * @param {double} clock_frequency - The clock frequency for the SPI communication.
 * @param {unsigned char} data_order - The bit order for data transmission. MSBFIRST or LSBFIRST.
 * @param {unsigned char} data_mode - The SPI data mode (0, 1, 2, or 3) defining the clock polarity and phase.
 * @returns [-3] `SPI2_SETTINGS_INIT` to indicate that the SPI2 settings have been successfully initialized.
 */
int init_SPI2_settings(double clock_frequency, unsigned char data_order, unsigned char data_mode);

/**
 * @name init_SPI2_active_state
 * @brief Activates the SPI2 interface, placing the easyBench in an active state.
 * @details
 * 1. Sets the SPI2_RUN pin to HIGH, activating the SPI2 interface.
 * @returns [-4] `RUN` to indicate that the easyBench has successfully transitioned to an active state.
 */
int init_SPI2_active_state();  //TEMPORAIRE - NE SERA A L'ETAT HAUT QUE LORSQUE LA RASPI SERA PRETE

#endif