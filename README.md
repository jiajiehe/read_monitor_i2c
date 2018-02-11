# read_monitor_i2c
## Description
An application in C to read the first 20 Bytes of the EDID (Extended Display Identification Data) of an HDMI display over i2c. From the EDID, the application can print the following information regarding the HDMI display:
1. Manufacturer ID (e.g., “IBM”)
2. Week and year of manufacture
3. Serial number
4. EDID version and revision (e.g., “1.3”)

## Usage
```sudo ./read <BUS_ID>```

## Tools
Before starting, it can be useful to know what bus the display is connected to and what the EDID looks like.
Us i2c-tools to get a list of all the devices on a given i2c bus (install using apt or build from [here](https://github.com/mozilla-b2g/i2c-tools)):
```sudo i2cdetect <BUS_ID>```
To get the EDID from command line, and then parse it (requires read-edid, install using apt):
```sudo get-edid -b <BUS_ID> | parse-edid```

## Useful links
1.(userspace) [Accessing i2c devices from userspace](https://www.kernel.org/doc/Documentation/i2c/dev-interface)
2.(kernelspace) [Kernel modules programming guide (Hello, World - Part 1)](http://www.tldp.org/LDP/lkmpg/2.6/html/x121.html)
3.(kernelspace) [Writing i2c client drivers](http://renjucnair.blogspot.ca/2012/01/writing-i2c-client-driver.html)
