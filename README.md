# CanonEOSArduinoRemote
Welcome to Canon EOSArduinoRemote! I pulled this together to help taking astrophotographs using an old Canon D350.
This will work with any Canon that has the 2.5mm socket for remote focus/shutter. It may work with other cameras with a similar focus/shutter setup. YMMV.

The code is provided as is, does what I need most of the time but probably needs some tweaks. It is functional and seems to work

Hardware
- Arduino Uno R4 WiFi
- Arduino R4 Relay Shield
- 3.5mm to 2.5mm Audio Cable

Construction
- cut the 3.5mm end off of the Audio cable and trim back the wires.
- Connect the white pair to relay 1 (focus)
- Connect the red pair to relay 2 (shutter)
- Load the code to the arduino
- Plug the 2.5mm plug into the side of your Canon camera. 
- Monitor the serial output to see what <ip address> it is given
- connect via http://<ip address>/

Arduino Libraries
If not already done so then you will need to install the libraries to support the Arduino Uno R4. I don't recall any other libraries being required.
