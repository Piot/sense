/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include "stik_input_manager.h"
#include <sense/sense_input.h>
#include <clog/clog.h>

#include <stik/stik.h>

static bool key_is_pressed(const SenseNamedButtons *keys)
{
	return (keys->vertical != 0 || keys->horizontal != 0 ||  keys->a != 0 || keys->b != 0 || keys->x != 0 || keys->y != 0);
}

static int checkForNewGamepads(SenseStikInputManager* self)
{
    if (self->boundGamepadsCount == 0) {
        InputHandle_t handles[STIK_INPUT_MAX_COUNT];
        int controllerCount = stikGetConnectedControllers(&self->stik, handles);
        if (controllerCount > 0) {
            InputHandle_t selectedControllerHandle = handles[0];
            ESteamInputType inputDeviceType = stikGetInputTypeForHandle(&self->stik, selectedControllerHandle);
            CLOG_OUTPUT_STDERR("you have device type %d %s", inputDeviceType, stikGetInputTypeName(inputDeviceType));
            self->boundGamepadsCount = 1;
            self->boundGamepads[0].isUsed = 1;
            self->boundGamepads[0].inputHandle = selectedControllerHandle;

            int worked = stikActivateActionSet(&self->stik, selectedControllerHandle, self->actionSetHandle);
            if (worked < 0) {
                return worked;
            }
        } else {
            CLOG_SOFT_ERROR("No controllers attached")
        }
    }
/*
		knownState->isBound = true;
		const char* name = StikGetGamepadName(joystickId);
		CLOG_INFO("detected and bound gamepad %d %s", joystickId, name)
		if (self->boundGamepadsCount >= self->boundGamepadsMax) {
			CLOG_ERROR("too many gamepads")
			return;
		}
		BoundGamepad* boundGamepad = &self->boundGamepads[self->boundGamepadsCount++];
		boundGamepad->joystickId = joystickId;
		boundGamepad->isUsed = true;
	*/

    return 0;
}

static void setDigitalFromData(int* target, InputDigitalActionData_t data)
{
    *target = data.state == 0 ? 0 : 1;
}

static int setDigital(Stik* stik, int* target, InputHandle_t controllerHandle, const char* digitalActionName)
{
    InputDigitalActionHandle_t digitalActionHandle = stikGetDigitalActionHandle(stik, digitalActionName);
    CLOG_OUTPUT_STDERR("stik digital action '%s' %d", digitalActionName, digitalActionHandle);
    if (digitalActionHandle == 0) {
        return -5;
    }

    InputDigitalActionData_t data = stikGetDigitalActionData(stik, controllerHandle, digitalActionHandle);
    CLOG_OUTPUT_STDERR("stik %d digital action data %d (active:%d)", digitalActionHandle, data.state, data.active);
    setDigitalFromData(target, data);

    return 0;
}

static int getGamepadState(Stik* stik, InputHandle_t inputHandle, SenseNamedButtons* button)
{
    setDigital(stik, &button->leftShoulder, inputHandle, "Interact");
    setDigital(stik, &button->a,  inputHandle,  "Ability1");

    return 0;
}

static void scanGamepads(SenseStikInputManager* self, SenseButtons gamepadStates[8])
{
	for (int i=0; i<self->boundGamepadsCount; ++i)
	{
		BoundGamepad* gamepad = &self->boundGamepads[i];
		if (!gamepad->isUsed) {
			tc_mem_clear_type(&gamepadStates[i]);
			continue;
		}

		int worked = getGamepadState(&self->stik, gamepad->inputHandle, &gamepadStates[i].named);
		if (worked < 0) {
			tc_mem_clear_type(&gamepadStates[i]);
			continue;
		}
	}
}

int senseStikInputManagerInit(SenseStikInputManager*self, Atheneum *atheneum)
{
	tc_mem_clear_type(self);
	self->boundGamepadsMax = 16;

   int stikResult = stikInit(&self->stik, atheneum);
   if (stikResult < 0) {
       return stikResult;
   }

   stikUpdate(&self->stik);

   return 0;
}

void senseStikInputManagerUpdate(SenseStikInputManager*self, SenseInput*input)
{
	tc_mem_clear_type(input);

    stikUpdate(&self->stik);

    if (self->actionSetHandle == 0) {
        self->actionSetHandle = stikGetActionSetHandle(&self->stik, "InGame");
        if (self->actionSetHandle == 0) {
            CLOG_ERROR("action set not found %d", self->actionSetHandle)
            return;
        }
        CLOG_WARN("action set found:%d", self->actionSetHandle)
    }

	int worked = checkForNewGamepads(self);
    if (worked < 0) {
        CLOG_ERROR("can not check for gamepads %d", worked)
    }
	scanGamepads(self, &input->devices[0]);
}
