/*
 * gpio.h
 *
 *  Created on: Jan 26, 2021
 *      Author: Jeff
 */

#ifndef GPIO_H_
#define GPIO_H_



#ifdef LD2_Pin
void toggleLed();
#endif


// GPIO Setup
#ifdef NDEBUG
// GPIO Simulation Constants
	#define FAKE_GPIO ((uint16_t)-1)

	#ifndef SENSE_A_Pin
		#define SENSE_A_Pin (FAKE_GPIO)
		#define SENSE_A_GPIO_Port (0)
	#endif

	#ifndef SENSE_B_Pin
		#define SENSE_B_Pin (FAKE_GPIO)
		#define SENSE_B_GPIO_Port (0)
	#endif

	#ifndef SENSE_C_Pin
		#define SENSE_C_Pin (FAKE_GPIO)
		#define SENSE_C_GPIO_Port (0)
	#endif

	#ifndef SENSE_D_Pin
		#define SENSE_D_Pin (FAKE_GPIO)
		#define SENSE_D_GPIO_Port (0)
	#endif

	#ifndef DISARM_INPUT_Pin
		#define DISARM_INPUT_Pin (FAKE_GPIO)
		#define DISARM_INPUT_GPIO_Port (0)
	#endif

	#ifndef CHECK_HARDWARE_DEPLOYMENT_DISABLE_Pin
		#define CHECK_HARDWARE_DEPLOYMENT_DISABLE_Pin (FAKE_GPIO)
		#define CHECK_HARDWARE_DEPLOYMENT_DISABLE_GPIO_Port (0)
	#endif

	#ifndef U1S_CHECK_Pin
		#define U1S_CHECK_Pin (FAKE_GPIO)
		#define U1S_CHECK_GPIO_Port (0)
	#endif

	#ifndef U2S_CHECK_Pin
		#define U2S_CHECK_Pin (FAKE_GPIO)
		#define U2S_CHECK_GPIO_Port (0)
	#endif

	#ifndef U3S_CHECK_Pin
		#define U3S_CHECK_Pin (FAKE_GPIO)
		#define U3S_CHECK_GPIO_Port (0)
	#endif

	#ifndef U4S_CHECK_Pin
		#define U4S_CHECK_Pin (FAKE_GPIO)
		#define U4S_CHECK_GPIO_Port (0)
	#endif

	#ifndef SENSOR_NOMINAL_Pin
		#define SENSOR_NOMINAL_Pin (FAKE_GPIO)
		#define SENSOR_NOMINAL_GPIO_Port (0)
	#endif

	#ifndef SENSOR_ERROR_Pin
		#define SENSOR_ERROR_Pin (FAKE_GPIO)
		#define SENSOR_ERROR_GPIO_Port (0)
	#endif

	#ifndef CS1_Pin
		#define CS1_Pin (FAKE_GPIO)
		#define CS1_GPIO_Port (0)
	#endif

	#ifndef CS2_Pin
		#define CS2_Pin (FAKE_GPIO)
		#define CS2_GPIO_Port (0)
	#endif

	#ifndef CS3_Pin
		#define CS3_Pin (FAKE_GPIO)
		#define CS3_GPIO_Port (0)
	#endif

#ifndef NAND_CS_Pin
#define NAND_CS_Pin (FAKE_GPIO)
#define NAND_CS_GPIO_Port (0)
#endif




#endif


#endif /* GPIO_H_ */
