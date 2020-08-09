# Library for receiving wireless weather sensor data with Arduino

Currently the "Prologue" temperature sensor protocol is supported. This is used by a large number of low cost 433 MHz based wireless thermometers.

For example: ThermoPro TPR60 and compatible sets.

This library adds a more robust decoding algorithm compared to similar solutions which should make the reception more resilient. As the library is very simple, it is also quite hackable, so feel free to help youself :)

# Hardware needed

Any 433 MHz receiver should work. They can usually be connected directly to your Arduino board. Please look up online how to do this to ensure your setup will work.

Please note reception may be weak if the transmitting temperature sensor is not nearby. An external antenna will likely be needed if reception is to be done at a distance.

# Disclaimer

This software is distributed "As is" for educational purposes only and comes without warranty. It is heavily inspired by other open source projects that allow reading of wirless weather sensor data:

- [homeGW](https://github.com/dgomes/homeGW)
- [PrologueDecoder](https://github.com/zoomx/PrologueDecoder)