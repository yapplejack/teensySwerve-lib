# This is largely derived from [halsim_xrp](https://github.com/wpilibsuite/allwpilib/blob/main/simulation/halsim_xrp/README.md?plain=1)

# HAL Teensy Client

This is an extension that provides a client version of the Teensy protocol for transmitting robot hardware interface state to an Teensy robot over UDP.

## Configuration

The Teensy client has a number of configuration options available through environment variables.

``HALSIMTEENSY_HOST``: The host to connect to.  Defaults to localhost.

``HALSIMTEENSY_PORT``: The port number to connect to.  Defaults to 3540.

## Teensy Protocol

The WPILib -> Teensy protocol is binary-based to save on bandwidth due to hardware limitations of the Teensy robot. The messages to/from the Teensy follow a the format below:

| 2 bytes             | 1 byte            | n bytes                             |
|---------------------|-------------------|-------------------------------------|
| _uint16_t_ sequence | _uint8_t_ control | [&lt;Tagged Data&gt;](#tagged-data) |

### Control Byte

The control byte is used to indicate the current `enabled` state of the WPILib robot code. When this is set to `1`, the robot is enabled, and when it is set to `0` it is disabled.

Messages originating from the Teensy have an unspecified value for the control byte.

### Tagged Data

The `Tagged Data` section can contain an arbitrary number of data blocks. Each block has the format below:

| 1 byte         | 1 byte          | n bytes         |
|----------------|-----------------|-----------------|
| _uint8_t_ size | _uint8_t_ tagID | &lt;payload&gt; |

The `size` byte encodes the size of the data block, _excluding_ itself. Thus the smallest block size is 2 bytes, with a size value of 1 (1 size byte, 1 tag byte, 0 payload bytes). Maximum size of the payload is 254 bytes.


Utilizing tagged data blocks allows us to send multiple pieces of data in a single UDP packet. The tags currently implemented for the Teensy are as follows:

| Tag  | Description                   |
|------|-------------------------------|
| 0x12 | [TeensyMotor](#teensymotor)   |
| 0x13 | [DIO](#dio)                   |
| 0x14 | [AnalogIn](#analogin)         |
| 0x15 | [TeensyGyro](#teensygyro)     |
| 0x16 | [Encoder](#encoder)           |


#### TeensyMotor

| Order | Data Type | Description       |
|-------|-----------|-------------------|
| 0     | _uint8_t_ | ID                |
| 1     | _float_   | Value [-1.0, 1.0] |

IDs:
| ID | Description                |
|----|----------------------------|
| 0  | Front Left Drive Motor     |
| 1  | Front Left Azimuth Motor   |
| 2  | Rear Left Drive Motor      |
| 3  | Rear Left Azimuth Motor    |
| 4  | Rear Right Drive Motor     |
| 5  | Rear Right Azimuth Motor   |
| 6  | Front Right Drive Motor    |
| 7  | Front Right Azimuth Motor  |

#### DIO

| Order | Data Type | Description        |
|-------|-----------|--------------------|
| 0     | _uint8_t_ | ID                 |
| 1     | _uint8_t_ | Value (True/False) |

#### AnalogIn

| Order | Data Type | Description |
|-------|-----------|-------------|
| 0     | _uint8_t_ | ID          |
| 1     | _float_   | Value       |

#### TeensyGyro

| Order | Data Type | Description   |
|-------|-----------|---------------|
| 0     | _float_   | rate_x (dps)  |
| 1     | _float_   | rate_y (dps)  |
| 2     | _float_   | rate_z (dps)  |
| 3     | _float_   | angle_x (deg) |
| 4     | _float_   | angle_y (deg) |
| 5     | _float_   | angle_z (deg) |

#### Encoder

| Order | Data Type  |     Description    |
|-------|------------|--------------------|
| 0     | _uint8_t_  | ID                 |
| 1     | _int32_t_  | Count              |
| 2     | _uint32_t_ | Period Numerator   |
| 3     | _uint32_t_ | Period Denominator |

IDs:
| ID | Description                  |
|----|------------------------------|
| 0  | Front Left Drive Encoder     |
| 1  | Front Left Azimuth Encoder   |
| 2  | Rear Left Drive Encoder      |
| 3  | Rear Left Azimuth Encoder    |
| 4  | Rear Right Drive Encoder     |
| 5  | Rear Right Azimuth Encoder   |
| 6  | Front Right Drive Encoder    |
| 7  | Front Right Azimuth Encoder  |
