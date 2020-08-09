# Library for parsing wireless weather sensor data with Arduino

Currently the "Prologue" temperature sensor is supported which covers a large number of low cost 433 MHz based wirless thermometers.

These include ThermoPro sensor TPR60.

This library adds a more robust decoding algorithm compared to similar solutions that makes the reception slightly more resilient. As it is very simple, the library is also quite hackable, so feel free to help youself :)

# Hardware needed

Any 433 receiver with its data pin connected to the Arduino as per library set up. You can find directions on how to use the library in the example folder.

# Disclaimer

This software is distributed "As is" for educational purposes only and comes without warranty. It is heavily inspired by other open source projects that allow reading of wirless weather sensor data:

- [homeGW](https://github.com/dgomes/homeGW)
- [PrologueDecoder](https://github.com/zoomx/PrologueDecoder)