/*
Nintendo Switch Fightstick - Proof-of-Concept

Based on the LUFA library's Low-Level Joystick Demo
	(C) Dean Camera
Based on the HORI's Pokken Tournament Pro Pad design
	(C) HORI

This project implements a modified version of HORI's Pokken Tournament Pro Pad
USB descriptors to allow for the creation of custom controllers for the
Nintendo Switch. This also works to a limited degree on the PS3.

Since System Update v3.0.0, the Nintendo Switch recognizes the Pokken
Tournament Pro Pad as a Pro Controller. Physical design limitations prevent
the Pokken Controller from functioning at the same level as the Pro
Controller. However, by default most of the descriptors are there, with the
exception of Home and Capture. Descriptor modification allows us to unlock
these buttons for our use.
*/

/** \file
 *
 *  Main source file for the posts printer demo. This file contains the main tasks of
 *  the demo and is responsible for the initial application hardware configuration.
 */

#include "Joystick.h"
#include "instructions.h"

extern const uint8_t image_data[0x12c1] PROGMEM;

// Main entry point.
int main(void) {
	// We'll start by performing hardware and peripheral setup.
	SetupHardware();
	// We'll then enable global interrupts for our use.
	GlobalInterruptEnable();
	// Once that's done, we'll enter an infinite loop.
	for (;;)
	{
		// We need to run our task to process and deliver data for our IN and OUT endpoints.
		HID_Task();
		// We also need to run the main USB management task.
		USB_USBTask();
	}
}

// Configures hardware and peripherals, such as the USB peripherals.
void SetupHardware(void) {
	// We need to disable watchdog if enabled by bootloader/fuses.
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	// We need to disable clock division before initializing the USB hardware.
	clock_prescale_set(clock_div_1);
	// We can then initialize our hardware and peripherals, including the USB stack.

	#ifdef ALERT_WHEN_DONE
	// Both PORTD and PORTB will be used for the optional LED flashing and buzzer.
	#warning LED and Buzzer functionality enabled. All pins on both PORTB and \
PORTD will toggle when printing is done.
	DDRD  = 0xFF; //Teensy uses PORTD
	PORTD =  0x0;
                  //We'll just flash all pins on both ports since the UNO R3
	DDRB  = 0xFF; //uses PORTB. Micro can use either or, but both give us 2 LEDs
	PORTB =  0x0; //The ATmega328P on the UNO will be resetting, so unplug it?
	#endif
	// The USB stack should be initialized last.
	USB_Init();
}

// Fired to indicate that the device is enumerating.
void EVENT_USB_Device_Connect(void) {
	// We can indicate that we're enumerating here (via status LEDs, sound, etc.).
}

// Fired to indicate that the device is no longer connected to a host.
void EVENT_USB_Device_Disconnect(void) {
	// We can indicate that our device is not ready (via status LEDs, sound, etc.).
}

// Fired when the host set the current configuration of the USB device after enumeration.
void EVENT_USB_Device_ConfigurationChanged(void) {
	bool ConfigSuccess = true;

	// We setup the HID report endpoints.
	ConfigSuccess &= Endpoint_ConfigureEndpoint(JOYSTICK_OUT_EPADDR, EP_TYPE_INTERRUPT, JOYSTICK_EPSIZE, 1);
	ConfigSuccess &= Endpoint_ConfigureEndpoint(JOYSTICK_IN_EPADDR, EP_TYPE_INTERRUPT, JOYSTICK_EPSIZE, 1);

	// We can read ConfigSuccess to indicate a success or failure at this point.
}

// Process control requests sent to the device from the USB host.
void EVENT_USB_Device_ControlRequest(void) {
	// We can handle two control requests: a GetReport and a SetReport.

	// Not used here, it looks like we don't receive control request from the Switch.
}

// Process and deliver data from IN and OUT endpoints.
void HID_Task(void) {
	// If the device isn't connected and properly configured, we can't do anything here.
	if (USB_DeviceState != DEVICE_STATE_Configured)
		return;

	// We'll start with the OUT endpoint.
	Endpoint_SelectEndpoint(JOYSTICK_OUT_EPADDR);
	// We'll check to see if we received something on the OUT endpoint.
	if (Endpoint_IsOUTReceived())
	{
		// If we did, and the packet has data, we'll react to it.
		if (Endpoint_IsReadWriteAllowed())
		{
			// We'll create a place to store our data received from the host.
			USB_JoystickReport_Output_t JoystickOutputData;
			// We'll then take in that data, setting it up in our storage.
			while(Endpoint_Read_Stream_LE(&JoystickOutputData, sizeof(JoystickOutputData), NULL) != ENDPOINT_RWSTREAM_NoError);
			// At this point, we can react to this data.

			// However, since we're not doing anything with this data, we abandon it.
		}
		// Regardless of whether we reacted to the data, we acknowledge an OUT packet on this endpoint.
		Endpoint_ClearOUT();
	}

	// We'll then move on to the IN endpoint.
	Endpoint_SelectEndpoint(JOYSTICK_IN_EPADDR);
	// We first check to see if the host is ready to accept data.
	if (Endpoint_IsINReady())
	{
		// We'll create an empty report.
		USB_JoystickReport_Input_t JoystickInputData;
		// We'll then populate this report with what we want to send to the host.
		GetNextReport(&JoystickInputData);
		// Once populated, we can output this data to the host. We do this by first writing the data to the control stream.
		while(Endpoint_Write_Stream_LE(&JoystickInputData, sizeof(JoystickInputData), NULL) != ENDPOINT_RWSTREAM_NoError);
		// We then send an IN packet on this endpoint.
		Endpoint_ClearIN();
	}
}

void reset_report(USB_JoystickReport_Input_t* const ReportData) {
	memset(ReportData, 0, sizeof(USB_JoystickReport_Input_t));
	ReportData->LX = STICK_CENTER;
	ReportData->LY = STICK_CENTER;
	ReportData->RX = STICK_CENTER;
	ReportData->RY = STICK_CENTER;
	ReportData->HAT = HAT_CENTER;
}

void take_action(action_t action, USB_JoystickReport_Input_t* const ReportData) {
	switch (action)
	{
		case get_off_bike:
		case get_on_bike:
		case press_plus:
			ReportData->Button |= SWITCH_PLUS;
			break;
		
		case turn_left:
			ReportData->RX = STICK_MIN;
			break;
		
		case turn_right:
			ReportData->RX = STICK_MAX;
			break;
		
		case move_forward:
			ReportData->LY = STICK_MIN;
			break;
		case move_backward:
			ReportData->LY = STICK_MAX;
			break;
		
		case move_left:
			ReportData->LX = STICK_MIN;
			break;

		case move_right:
			ReportData->LX = STICK_MAX;
			break;
		
		case open_menu:
		case close_menu:
		case press_x:
			ReportData->Button |= SWITCH_X;
			break;

		case press_a:
			ReportData->Button |= SWITCH_A;
			break;

		case press_b:
			ReportData->Button |= SWITCH_B;
			break;

		case press_y:
			ReportData->Button |= SWITCH_Y;
			break;

		case reset_view_angle:
			ReportData->Button |= SWITCH_L;
			break;
		
		case circle_around:
			ReportData->LX = STICK_MAX;
			ReportData->RX = STICK_MAX;
			break;

		case hang:
			reset_report(ReportData);
			break;

		default:
			reset_report(ReportData);
			break;
	}
}


typedef enum {
	SYNC_CONTROLLER,
	FLY_TO_NURSERY,
	APPROACH_NPC,
	PICK_SWAP_SLOT,
	SWAP_POKEMON,
	BREEDING_PREP,
	BREEDING,
	BREATHE,
	NEXT_ROUND,
	DONE
} State_t;

State_t state = SYNC_CONTROLLER;

#define ECHOES 2
int echoes = 0;
USB_JoystickReport_Input_t last_report;

int duration_count = 0;
int bufindex = 0;
int portsval = 0;
int swap_slot_number = 0;

static inline void do_steps(const command_t* steps, uint16_t steps_size, USB_JoystickReport_Input_t* const ReportData, State_t nextState, int add_swap_slot_number) {
	take_action(steps[bufindex].action, ReportData);
	duration_count ++;
	
	if (duration_count > steps[bufindex].duration)
	{
		bufindex ++;
		duration_count = 0;
	}

	if (bufindex > steps_size - 1) 
	{
		bufindex = 0;
		duration_count = 0;
		if (add_swap_slot_number) {
			swap_slot_number = (swap_slot_number + 1) % 5;
		}

		state = nextState;
		reset_report(ReportData);
	}
}


// Prepare the next report for the host.
void GetNextReport(USB_JoystickReport_Input_t* const ReportData) {

	// Prepare an empty report
	reset_report(ReportData);

	// Repeat ECHOES times the last report
	if (echoes > 0)
	{
		memcpy(ReportData, &last_report, sizeof(USB_JoystickReport_Input_t));
		echoes--;
		return;
	}

	// States and moves management
	switch (state)
	{
		case SYNC_CONTROLLER:
			bufindex = 0;
			state = BREATHE;
			break;
		
		case BREATHE:
			do_steps(wake_up_hang, ARRAY_SIZE(wake_up_hang), ReportData, FLY_TO_NURSERY, 0);
			break;

		case FLY_TO_NURSERY:
			do_steps(fly_to_breading_steps, ARRAY_SIZE(fly_to_breading_steps), ReportData, APPROACH_NPC, 0);
			// take_action(fly_to_breading_steps[bufindex], ReportData);
			// duration_count ++;
			
			// if (duration_count > fly_to_breading_steps[bufindex].duration)
			// {
			// 	bufindex ++;
			// 	duration_count = 0;
			// }

			// if (bufindex > (int)(sizeof(fly_to_breading_steps) / sizeof(fly_to_breading_steps[0])) - 1) 
			// {
			// 	bufindex = 0;
			// 	duration_count = 0;

			// 	state = APPROACH_NPC;

			// 	reset_report(ReportData);
			// }
			break;

		case APPROACH_NPC:
			do_steps(get_egg_steps, ARRAY_SIZE(get_egg_steps), ReportData, PICK_SWAP_SLOT, 0);
			// take_action(get_egg_steps[bufindex], ReportData);
			// duration_count ++;
			
			// if (duration_count > get_egg_steps[bufindex].duration)
			// {
			// 	bufindex ++;
			// 	duration_count = 0;
			// }

			// if (bufindex > (int)(sizeof(get_egg_steps) / sizeof(get_egg_steps[0])) - 1) 
			// {
			// 	bufindex = 0;
			// 	duration_count = 0;

			// 	state = PICK_SWAP_SLOT;

			// 	reset_report(ReportData);
			// }
		
			break;
	
		case PICK_SWAP_SLOT:
			// do_steps(swap_slot[swap_slot_number], swap_slot_size[swap_slot_number], ReportData, SWAP_POKEMON, 1);
			take_action(swap_slot[swap_slot_number][bufindex].action, ReportData);
			duration_count ++;
			
			if (duration_count > swap_slot[swap_slot_number][bufindex].duration)
			{
				bufindex ++;
				duration_count = 0;
			}

			if (bufindex > swap_slot_size[swap_slot_number] - 1) 
			{
				bufindex = 0;
				duration_count = 0;
				swap_slot_number = (swap_slot_number + 1) % 5;

				state = SWAP_POKEMON;

				reset_report(ReportData);
			}
			break;

			// take_action(swap_slot_3[bufindex], ReportData);
			// duration_count ++;
			
			// if (duration_count > swap_slot_3[bufindex].duration)
			// {
			// 	bufindex ++;
			// 	duration_count = 0;
			// }

			// if (bufindex > (int)(sizeof(swap_slot_3) / 
			//                      sizeof(swap_slot_3[0])) - 1) 
			// {
			// 	bufindex = 0;
			// 	duration_count = 0;
			// 	swap_slot_number = (swap_slot_number + 1) % 5;

			// 	state = SWAP_POKEMON;
			// }
			// break;

		case SWAP_POKEMON:
			do_steps(swap_pokemon_steps, ARRAY_SIZE(swap_pokemon_steps), ReportData, BREEDING_PREP, 0);
			// take_action(swap_pokemon_steps[bufindex], ReportData);
			// duration_count ++;

			// if (duration_count > swap_pokemon_steps[bufindex].duration)
			// {
			// 	bufindex ++;
			// 	duration_count = 0;
			// }

			// if (bufindex > (int)(sizeof(swap_pokemon_steps) / sizeof(swap_pokemon_steps[0])) - 1) 
			// {
			// 	bufindex = 0;
			// 	duration_count = 0;

			// 	state = BREEDING_PREP;

			// 	reset_report(ReportData);
			// }
			break;

		case BREEDING_PREP:
			do_steps(breeding_prep_steps, ARRAY_SIZE(breeding_prep_steps), ReportData, BREEDING, 0);
			break;

		case BREEDING:
			take_action(circle_around, ReportData);
			duration_count ++;
			
			if (duration_count % 100 >= 0 && duration_count % 100 <= 5) {
				take_action(press_a, ReportData);
			}
			
			if (duration_count > breeding_duration - 1) {
				duration_count = 0;
				bufindex = 0;

				state = NEXT_ROUND;
			}
			break;

		case NEXT_ROUND:
			do_steps(next_round_steps, ARRAY_SIZE(next_round_steps), ReportData, FLY_TO_NURSERY, 0);
			break;

		case DONE:
			#ifdef ALERT_WHEN_DONE
			portsval = ~portsval;
			PORTD = portsval; //flash LED(s) and sound buzzer if attached
			PORTB = portsval;
			_delay_ms(250);
			#endif
			return;
	}

	// // Inking
	// if (state != SYNC_CONTROLLER && state != SYNC_POSITION)
	// 	if (pgm_read_byte(&(image_data[(xpos / 8) + (ypos * 40)])) & 1 << (xpos % 8))
	// 		ReportData->Button |= SWITCH_A;

	// Prepare to echo this report
	memcpy(&last_report, ReportData, sizeof(USB_JoystickReport_Input_t));
	echoes = ECHOES;

}


