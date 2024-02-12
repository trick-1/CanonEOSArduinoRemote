# CanonEOSArduinoRemote
Welcome to Canon EOSArduinoRemote! I pulled this together to help taking astrophotographs using an old Canon D350. it will work for any other time delay/remote trigger application you may have. 
This will work with any Canon that has the 2.5mm socket for remote focus/shutter. It may work with other cameras with a similar focus/shutter setup. YMMV.

The code is provided as is, does what I need most of the time but probably needs some tweaks. It is functional and seems to work

<h2>Hardware</h2>
<ul></ul>
<li>rduino Uno R4 WiFi</li>
<li>Arduino R4 Relay Shield</li>
<li>3.5mm to 2.5mm Audio Cable</li>
</ul>

<h2>Construction</h2>
<ul>
<li>cut the 3.5mm end off of the Audio cable and trim back the wires.</li>
<li><Connect the white pair to relay 1 (focus) </li>
<li>Connect the red pair to relay 2 (shutter)</li>
<li>Load the code to the arduino<li>
<li>Plug the 2.5mm plug into the side of your Canon camera.</li>
<li>Monitor the serial output to see what <ip address> it is given</li>
<li>connect via http://<ip address>/</li>
</ul>
  
<h2>Arduino Libraries</h2>
<p>
If not already done so then you will need to install the libraries to support the Arduino Uno R4. I don't recall any other libraries being required.
<p></p>
