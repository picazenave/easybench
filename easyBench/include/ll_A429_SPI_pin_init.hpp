#ifndef LL_A429_SPI_PIN_INIT_HPP

#define LL_A429_SPI_PIN_INIT_HPP

#include <ll_A429_SPI_common.hpp>

/**
 * @name init_SPI2_MRST_pin
 * @brief Initializes the Master Reset (MRST) pin for the SPI2 interface as an output.
 * @details
 * 1. Setting the SPI2_MRST pin mode to OUTPUT with "pinMode()".
 */
void init_SPI2_MRST_pin();

/**
 * @name init_SPI2_HCS_pin
 * @brief Initializes the Hardware Chip Select (HCS) pin for the SPI2 interface as an output.
 * @details
 * 1. Setting the SPI2_HCS pin mode to OUTPUT with "pinMode()".
 */
void init_SPI2_HCS_pin();

/**
 * @name init_SPI2_READY_pin
 * @brief Configures the READY pin for the SPI2 interface as an input.
 * @details
 * 1. Setting the SPI2_READY pin mode to INPUT with "pinMode()".
 */
void init_SPI2_READY_pin();

/**
 * @name init_SPI2_RUN_pin
 * @brief Sets the RUN pin of the SPI2 interface as an output to control the operation state.
 * @details
 * 1. Setting the SPI2_RUN pin mode to OUTPUT with "pinMode()".
 */
void init_SPI2_RUN_pin();

/**
 * @name init_SPI2_pins
 * @brief Initializes all necessary pins for SPI2 communication with the A429 shield.
 * @details
 * 1. Calling `init_SPI2_MRST_pin()` to set the Master Reset (MRST) pin as OUTPUT.
 * 2. Calling `init_SPI2_HCS_pin()` to configure the Hardware Chip Select (HCS) pin as OUTPUT.
 * 3. Calling `init_SPI2_READY_pin()` to set the READY pin as INPUT.
 * 4. Calling `init_SPI2_RUN_pin()` to set the RUN pin as OUTPUT.
 * @returns [-1] `SPI2_PINS_INIT` to indicate that all SPI2 related pins have been successfully initialized.
 */
int init_SPI2_pins();

#endif