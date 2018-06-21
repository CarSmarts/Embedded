Things to do:
- Timeout on sending.. right now we wait until \n or 20 characters before sending
so if just a little bit is sent, it will wait forever.. not a problem with continuous
data, but could be an issue
- add readme (to all projects)
- maybe consider breaking this up into separate projects?
(or just rename this one to not be EMBEDED, but BLE gateway or something?)

## SmartCar
- Update SmartCar to use the UART communication
- Add test UART view
- Add m2ret config? (or just use UART)
- Start reading data from m2ret into canhack

## security/refinement stuff
- Add DFU ability (from SmartCar?)
- Add ability to update m2ret over UART? i guess its already possible if i have the 
UART passthrough
- Make a script to expose the BLE service as a standard mac serial port
- make a more application specific protocol (some way to turn off raw can sending)
- + the ability to send smartcar specific commands 
- the smartcar com protocol shouldn't?(maybe) depend on can, 