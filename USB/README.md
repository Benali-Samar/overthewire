#Overthewire usb linux device driver
The VID and PID are used for my USB cable for my phone, you can check it with: 

	$ lsusb

Change the corresponding VID and PID in the "overthewire.c" , save and compile.

1- To compile: 

	$ sudo make

2- To load:

	$ sudo insmod overthewire.ko

3- To see logs:

	$ sudo dmesg

This should display that the driver is registred with the USB core, plug in the USB and retype the command and you'll see that all the interfaces are setted.

4- To remove:

	$sudo rmmod overthewire 


USB subsystem in linux overview:

In linux most of all protocols have subsystems in kernel for communication with the userspace.
USBs are usually showen as TTY devices in userspace.
The USB subsystem consists of the USB core, the USB Host Controller, and the USB host controller’s driver.

Here is a minimal overview of the USB subsystem:

				                User Space      
		----------------|--------------------
				| 	        Kernel Space
		       +--------|--------+
		       |        |        |
		       |   USB Driver    |
		       |                 |
		       +--------|--------+
				|    	      USB subsystem
		   +------------------------+        
		   |   +--------|--------+  |
		   |   |        |        |  |
		   |   |    USB Core     |  |
		   |   |                 |  |
		   |   +--------|--------+  |
		   |            |           |
		   |   +--------|--------+  |
		   |   |     USB Host    |  |
		   |   |    Controller   |  |
		   |   +--------|--------+  |
		   |            |           |
		   +------------|-----------+
				|
		           USB Device


-USB Host Controller:

The USB host controller is to control communications between he host system and the USB devices.
From power management to initial transaction once the USB device is detected in the system.

-USB Core:

It is a codebase consisting of routines and structures available to Host Controller Driver and USB drivers.

-USB Driver:

This is the USB driver we are developping.

USB Devices contains a number of descriptors that help define what device is capable of... Here are discpritors bellow:

    -Device Descriptor: includes information such as what USB revision the device complies with, the Product and Vendor IDs used to load the appropriate drivers, ...
    
    -Configuration Descriptor: specifies values such as the amount of power this particular configuration uses if the device is self or bus-powered and the number of interfaces it has.
    
    -Interface Descriptor: A device can have one or more interfaces. Each interface descriptor can have a number of endpoints and represents a functional unit belonging to a particular class.
    
    -Endpoint Descriptor: Each endpoint descriptor is used to specify the type of transfer, direction, polling interval, and maximum packet size for each endpoint. In other words, each endpoint is a source or sink of data.
