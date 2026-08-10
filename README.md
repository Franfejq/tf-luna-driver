# TF-Luna LiDAR Sensor Driver for STM32

## Abstract
A low-level C/C++ driver for the Benewake TF-Luna Time-of-Flight (ToF) LiDAR sensor, developed and validated on the STM32 NUCLEO-F303K8 platform. This repository provides a hardware-abstracted UART communication interface and is structured for direct integration into CMake-based embedded development environments.

## 1. Theory of Operation
Unlike standard ToF sensors that measure the direct time of flight of random optical pulses, the TF-Luna modulates the amplitude of an emitted infrared continuous wave. The internal processor calculates distance by measuring the angular phase shift ($\Delta\varphi$) between the emitted and the reflected optical waveforms. 

## 2. Technical Specifications

| Parameter | Specification |
| :--- | :--- |
| **Operating Range** | 0.2m – 8.0m (90% reflectivity, white surfaces) |
| **Maximum Range (Low Reflectivity)** | 2.5m (10% reflectivity, dark surfaces) |
| **Accuracy** | ±6cm (0.2m – 3.0m) / ±2% (3.0m – 8.0m) |
| **Field of View (FOV)** | 2° |
| **Frame Rate** | 1Hz – 250Hz (Configurable) |
| **Power Supply Voltage** | 3.7V – 5.2V |
| **Current Consumption** | ≤70mA (Average) / 150mA (Peak) |
| **Power Consumption** | ≤350mW (Continuous Mode) |
| **Logic Level** | LVTTL (3.3V) |

## 3. Hardware Interface (UART Configuration)
The driver communicates with the TF-Luna module via a standard UART protocol. The serial peripheral must be initialized with the following parameters:

* **Baud Rate:** 115200 bps
* **Data Length:** 8 Bits
* **Stop Bits:** 1
* **Parity:** None

**STM32 Pin Assignment:**
* `PA9`  -> `USART1_TX`
* `PA10` -> `USART1_RX`

## 4. Build System Integration (CMake)
The driver is designed as a modular static library. To integrate it into an existing STM32 project:

1. Allocate the source files (`tf_luna.c`, `tf_luna.h`, `CMakeLists.txt`) in the standard directory structure: `linxDrivers/TFLuna/`.
2. The internal `CMakeLists.txt` defines the static library and resolves dependencies:
   ```cmake
   #We create a static library using the .c source file.
   add_library(TFLuna_Driver STATIC Src/tf_luna.c)

   #We tell CMake where to find the library's headers.
   target_include_directories(TFLuna_Driver PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Inc)

   #If we end up using other libraries, we'll include it as well, just in case.
   target_include_directories(TFLuna_Driver PRIVATE stm32cubemx)
   ```

3. Append the library to the root compilation sequence in the main CMakeLists.txt:
   ```cmake
   #We add the subfolder to the build path.
   add_subdirectory(linxDriver/Sensors/TFLuna)
   
   # Add linked libraries
   target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx
    TFLuna_Driver
   )
   ```

## 5. Data Protocol and Parsing
The module transmits data in a 9-byte little-endian format.Frame Synchronization: Each valid data frame begins with a 0x59 0x59 header.Distance Reconstruction: Executed via bitwise shifting of the payload bytes: (Byte_H << 8) | Byte_L.Integrity Validation: The 9th byte contains a Checksum to identify electrical noise corruption.Internal Temperature Calculation: Derived using the formula: C° = (Temp / 8) - 256.
   
## 6. Operational Constraints and Edge Cases.
   
### 6.1. Minimum Range (Dead Zone)
The sensor exhibits a dead zone between 0 cm and 20 cm. Optical reflections within this range exceed the timing window of the internal processor, resulting in erratic or invalid data.

### 6.2. Spatial Resolution and Multi-Path Errors (Ghost Readings)
Due to the 2° beam divergence, the optical footprint expands relative to distance. If the beam is pointed at a corner or the edge of an object, the reflection is split across two different depths. The sensor averages these returns, yielding a "ghost reading" ($Dist_1 < Dist_{Output} < Dist_2$) that does not correlate to a physical surface.

### 6.3. Signal Attenuation and Dummy Values
The Amplitude data byte indicates signal reliability. If the optical return is critically weak (Amplitude < 100), the sensor defaults to a dummy distance output of 0 cm. Firmware implementations must evaluate the amplitude threshold prior to utilizing the distance parameter to prevent false collision triggers.
