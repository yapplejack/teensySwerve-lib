// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <memory>

#include <WSProviderContainer.h>
#include <WSProvider_SimDevice.h>
#include <wpinet/EventLoopRunner.h>

#include "HALSimTeensy.h"

namespace teensyswerve {

class HALSimTeensyClient {
public:
	HALSimTeensyClient() = default;
	HALSimTeensyClient(const HALSimTeensyClient&) = delete;
	HALSimTeensyClient& operator=(const HALSimTeensyClient&) = delete;

	bool Initialize();

	wpilibws::ProviderContainer providers;
	wpilibws::HALSimWSProviderSimDevices simDevices { providers };
	wpi::EventLoopRunner runner;
	std::shared_ptr<HALSimTeensy> simteensy;
};

}  // namespace teensyswerve
