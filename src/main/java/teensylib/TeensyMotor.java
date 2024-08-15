// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package teensylib;

import edu.wpi.first.hal.SimBoolean;
import edu.wpi.first.hal.SimDevice;
import edu.wpi.first.hal.SimDevice.Direction;
import edu.wpi.first.hal.SimDouble;
import edu.wpi.first.wpilibj.motorcontrol.MotorController;
import java.util.HashMap;
import java.util.HashSet;

public class TeensyMotor implements MotorController {
  private static HashMap<Integer, String> s_simDeviceNameMap = new HashMap<>();
  private static HashSet<Integer> s_registeredDevices = new HashSet<>();

  private static void checkDeviceAllocation(int deviceNum) {
    if (!s_simDeviceNameMap.containsKey(deviceNum)) {
      throw new IllegalArgumentException("Invalid TeensyMotor device number. Should be 0-7");
    }

    if (s_registeredDevices.contains(deviceNum)) {
      throw new IllegalArgumentException("TeensyMotor " + deviceNum + " already allocated");
    }

    s_registeredDevices.add(deviceNum);
  }

  static {
    s_simDeviceNameMap.put(0, "motorFLD");
    s_simDeviceNameMap.put(1, "motorFLA");
    s_simDeviceNameMap.put(2, "motorRLD");
    s_simDeviceNameMap.put(3, "motorRLA");
    s_simDeviceNameMap.put(4, "motorRRD");
    s_simDeviceNameMap.put(5, "motorRRA");
    s_simDeviceNameMap.put(6, "motorFRD");
    s_simDeviceNameMap.put(7, "motorFRA");
  }

  private final SimDouble m_simSpeed;
  private final SimBoolean m_simInverted;

  /** TeensyMotor. */
  public TeensyMotor(int deviceNum) {
    checkDeviceAllocation(deviceNum);

    // We want this to appear on the WS messages as type: "TeensyMotor", device: <motor name>
    String simDeviceName = "TeensyMotor:" + s_simDeviceNameMap.get(deviceNum);
    SimDevice teensyMotorSimDevice = SimDevice.create(simDeviceName);

    if (teensyMotorSimDevice != null) {
      teensyMotorSimDevice.createBoolean("init", Direction.kOutput, true);
      m_simInverted = teensyMotorSimDevice.createBoolean("inverted", Direction.kInput, false);
      m_simSpeed = teensyMotorSimDevice.createDouble("speed", Direction.kOutput, 0.0);
    } else {
      m_simInverted = null;
      m_simSpeed = null;
    }
  }

  @Override
  public void set(double speed) {
    if (m_simSpeed != null) {
      boolean invert = false;
      if (m_simInverted != null) {
        invert = m_simInverted.get();
      }

      m_simSpeed.set(invert ? -speed : speed);
    }
  }

  @Override
  public double get() {
    if (m_simSpeed != null) {
      return m_simSpeed.get();
    }

    return 0.0;
  }

  @Override
  public void setInverted(boolean isInverted) {
    if (m_simInverted != null) {
      m_simInverted.set(isInverted);
    }
  }

  @Override
  public boolean getInverted() {
    return m_simInverted != null && m_simInverted.get();
  }

  @Override
  public void disable() {
    set(0.0);
  }

  @Override
  public void stopMotor() {
    set(0.0);
  }
}
