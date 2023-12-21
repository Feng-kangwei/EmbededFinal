# Embedded Challenge
## Introduction

This README provides guidance on how to set up and configure the necessary components for this project, specifically focusing on the integration of the `GYRO_DISCO_F429ZI` package.

## Installation

1. **Importing the GYRO_DISCO_F429ZI Package**

   Begin by installing the `GYRO_DISCO_F429ZI` package. Navigate to the project folder in your command line tool and execute the following command:
   ```
   pio pkg install --library "mbed-st/GYRO_DISCO_F429ZI"
   ```
   This command will download and install the `GYRO_DISCO_F429ZI` package into your project.

2. **Configuring the GYRO_DISCO_F429ZI.h File**

After installing the package, you need to configure the `GYRO_DISCO_F429ZI.h` file:

- Locate and open the `GYRO_DISCO_F429ZI.h` file in your project directory.
- Find the line that includes the `stm32f429i_discovery_gyroscope.h` header file. It will look something like this:

  ```cpp
  #include "/Users/daniel_feng/Desktop/EmbededFinal/src/drivers/stm32f429i_discovery_gyroscope.h"
  ```

- Replace the path (`/Users/daniel_feng/Desktop/EmbededFinal/src/drivers/stm32f429i_discovery_gyroscope.h`) with the absolute path to this file in your own project folder. The relative path part (`src/drivers/stm32f429i_discovery_gyroscope.h`) should remain unchanged.

## Final Steps

Ensure that all changes are saved and recompile your project to reflect these updates. If you encounter any issues, refer to the package documentation or seek help from the community forums.



