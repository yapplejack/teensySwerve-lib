// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <functional>
#include <map>
#include <span>
#include <string>

#include <wpi/json_fwd.h>
#include <wpinet/raw_uv_ostream.h>

#define TEENSY_TAG_MOTOR 0x12
#define TEENSY_TAG_DIO 0x13
#define TEENSY_TAG_ANALOG 0x14
#define TEENSY_TAG_GYRO 0x15
#define TEENSY_TAG_ENCODER 0x16

namespace teensyswerve {

using WPILibUpdateFunc = std::function<void(const wpi::json&)>;

class Teensy {
public:
	Teensy();

	void SetWPILibUpdateFunc(WPILibUpdateFunc func) {
		m_wpilib_update_func = func;
	}

	void HandleWPILibUpdate(const wpi::json &data);
	void HandleTeensyUpdate(std::span<const uint8_t> packet);

	void SetupTeensySendBuffer(wpi::raw_uv_ostream &buf);

private:
	// To Teensy Methods
	void SetupSendHeader(wpi::raw_uv_ostream &buf);
	void SetupMotorTag(wpi::raw_uv_ostream &buf);
	void SetupDigitalOutTag(wpi::raw_uv_ostream &buf);

	// WPILib Sim Update Handlers
	void HandleDriverStationSimValueChanged(const wpi::json &data);
	void HandleMotorSimValueChanged(const wpi::json &data);
	void HandleDIOSimValueChanged(const wpi::json &data);
	void HandleGyroSimValueChanged(const wpi::json &data);
	void HandleEncoderSimValueChanged(const wpi::json &data);

	// Teensy Packet Update Handlers
	void ReadGyroTag(std::span<const uint8_t> packet);
	void ReadDIOTag(std::span<const uint8_t> packet);
	void ReadEncoderTag(std::span<const uint8_t> packet);
	void ReadAnalogTag(std::span<const uint8_t> packet);

	// Robot State
	std::map<uint8_t, bool> m_digital_outputs;
	std::map<uint8_t, float> m_motor_outputs;

	// Might not need these
	std::map<uint8_t, bool> m_digital_inputs;
	std::map<uint8_t, float> m_analog_inputs;
	std::map<uint8_t, int32_t> m_encoder_inputs;

	// We need a map from Teensy encoder channels (0=left, 1=right etc)
	// to WPILib device ID
	// Key: Teensy encoder number, Value: WPILib channel
	// If no encoders are init-ed, this map is empty
	std::map<uint8_t, uint8_t> m_encoder_channel_map;

	uint16_t m_wpilib_bound_seq = 0;
	uint16_t m_xrp_bound_seq = 0;

	bool m_robot_enabled = false;

	std::string m_gyro_name;

	WPILibUpdateFunc m_wpilib_update_func;
};

}  // namespace teensyswerve
