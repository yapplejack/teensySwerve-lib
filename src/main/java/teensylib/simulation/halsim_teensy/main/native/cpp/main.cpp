// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include <cstdio>
#include <memory>

#include <hal/Extensions.h>

#include "HALSimTeensyClient.h"

#if defined(Win32) || defined(_WIN32)
#pragma comment(lib, "Ws2_32.lib")
#endif

using namespace teensyswerve;

static std::unique_ptr<HALSimTeensyClient> gClient;

/*--------------------------------------------------------------------------
 ** Main Entry Point. Start up the listening threads
 **------------------------------------------------------------------------*/
extern "C" {
#if defined(WIN32) || defined(_WIN32)
__declspec(dllexport)
#endif

int HALSIM_InitExtension(void) {
	std::puts("HALSim Teensy Extension Initializing");

	HAL_OnShutdown(nullptr, [](void*) {
		gClient.reset();
	});

	gClient = std::make_unique<HALSimTeensyClient>();
	if (!gClient->Initialize()) {
		return -1;
	}

	std::puts("HALSim Teensy Extension Initialized");
	return 0;
}

}  // extern "C"
