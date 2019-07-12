# OctoScreen

Purpose of this project is to provide an altrernative firmware for an MKS TFT touchscreen device for 3D printers. It get connected to an OctoPrint server via a MKS WIFI device (which is basically an ESP8266), running OctoHub (link TBD).

## Technical Requirements
- Supports the display **MKS TFT 3.2 V4.0** only. Older hardware revisions of the display use different components and are not supported. Other sizes of the display might use similar hardware, but they would require some code changes due to different resolution etc.
- You'll need a running OctoPrint server which is accessbile via API.
- You'll need an MQTT server (which is used to push information from OctoPrint via OctoHub to the display)
- OctoPrint needs to run a custom plugin (link TBD) to serve some of the functions offered by OctoScreen/OctoHub.

## Development Environment
- This project was written using the System Workbench for STM32, https://www.st.com/en/development-tools/sw4stm32.html
- You'll need an ST-LINK v2 adapter to transfer the firmware from your development environment to the MKS TFT display.
- The IDE plugin generated some of the coding specifically for the MKS TFT 3.2 device, so take care not to overwrite these generated coding parts during development.
- OctoScreen uses LVGL as GUI library. It's included as a submodule in the git repository, so make sure to also clone the submodules when cloning this repository

### Connecting ST-LINK v2 to the MKS TFT:
| ST-LINK | MKS-TFT32 |
|---------|-----------|
| 5V      | AUX-1 5V (optional if you power the display from another power source)         |
| GND     | AUX-1 GND or JTAG GND     |
| SWDIO   | JTAG DIO      |
| SWCLK   | JTAG CLK      |

## WARNING
Flashing custom firmware to your MKS TFT requires to unlock the flash memory, which erases all contents. I.e., you won't be able to save any backup of the existing MKS firmware and bootloader. So, to summarize: **Once you decide to flash this custom firmware, there's no easy way back to the stock firmware.**


## Screenshots

(Note that these images are from the LVGL simulator, so there's no actual printer data displayed. Real life display contents are more exciting... ;-) )

### Home Tab
![Home Tab](https://github.com/sidddy/OctoScreen/raw/master/screenshots/Home.png)

### Move Tab
![Move Tab](https://github.com/sidddy/OctoScreen/raw/master/screenshots/Move.png)

### Temperature Tab
![Temperature Tab](https://github.com/sidddy/OctoScreen/raw/master/screenshots/Temperature.png)

### Files Tab
![Files Tab](https://github.com/sidddy/OctoScreen/raw/master/screenshots/Files.png)
