# STM32F4-SDCARD-BMP-IMAGE-VIEWER

This project uses FATFS with SPI to Read .BMP images from the SD Card and display them on the TFT LCD. This project uses TouchGFX, HAL and FreeRTOS.

Steps:
1. Create a GUI Project with TouchGFX Designer and Setup your peripherals as follwos:
   1. Set SPI Speed to 2.624MBits/s (SPI1 in my case).
   2. Set GPIOB3 as SPI_CS.
   3. Enable FATFS (User Defined in my case)
   4. Setup UART for Debugging (UART5 in my case).
2. Generate the project and copt the files provided in your Folders.
3. Make the connections and you're ready to go.

NOTE: The Project currently only supports 24bpp .BMP Images (240x320)
NOTE: The Project does not support Screen Rotation, thus in order to display the Image (320x240) properly you must:
  1. Rotate Image Clockwise.
  2. Mirror The Image.
  3. Rotate Image 2 times clockwise.
