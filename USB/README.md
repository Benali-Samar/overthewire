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

