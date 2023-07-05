//the most used structs in a usb driver



// this struct will have all the informations about  the driver
struct usb_driver {
  const char * name; // driver name should be the same as the module name
  int (* probe) (struct usb_interface *intf,const struct usb_device_id *id);// called when a usb device is  connected
  void (* disconnect) (struct usb_interface *intf); //when a usb device is disconnected
  int (* ioctl) (struct usb_interface *intf, unsigned int code,void *buf);// to talk with userspace using "usbfs" filesystem
  int (* suspend) (struct usb_interface *intf, pm_message_t message); // when the device going to be suspended by the system
  int (* resume) (struct usb_interface *intf);// when the device is going to be resumed by the system
  int (* reset_resume) (struct usb_interface *intf);// when the suspended device has benn reset instead of being resumed
  int (* pre_reset) (struct usb_interface *intf);// called by the reset when the device is about to be reset
  int (* post_reset) (struct usb_interface *intf);// after it has benn reset
  const struct usb_device_id * id_table; // ID tables to support hotplugging
  struct usb_dynids dynids;// use internally to hold the list of dynamically added device ids for this driver
  struct usbdrv_wrap drvwrap;// driver model core structure wrapper
  unsigned int no_dynamic_id:1;// if set to 1, this will make the usb core not allow dynamic ids to be added to this driver
  unsigned int supports_autosuspend:1;//is set to 0, the usb core will not alow auto suspend for interfaces bound to this driver
  unsigned int soft_unbind:1;// if set to 1, the usb core will not kill URBs and disable endpoints before calling the driver's disconnect methode.
};



// for hotplugging, it holds  set of descriptors and specialized datat may be associated with each entry
const struct usb_device_id usb_table[] ={
	{USB_DEVICE (USB_VENDOR_ID, USB_PRODUCT_ID)//put your USB device's Vendor and Product ID
	{ } //terminating entry
	};
MODULE_DEVICE_TABLE(usb, usb_table);

// probe : matches the device ID pattern that the driver is regestred with the uSB core or not, if accepted it returns 0 if not returns an error code (-ENOMEM or -ENODEV)
static int usb_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
	dev_info(éinterface->dev, "USB Driver Probed: Vendor ID: 0x%02x, \t"
				  "Product ID : 0x%02x\n", id->idVendor, id->idProduct);
	return 0;
}
	// dev_info API is like pr_info but also prints some information about device ...
// diconnect : when a device is plugged out or removed
static void usb_disconnect(struct usb_interface *interface)
{
	dev_info(&interface->dev, "USB Driver Disconnected\n");
}


//the usb driver struct needs the addresses of the previous functions
static struct usb_driver usb_driver = {
	.name		= "USB NAME",
	.probe		= usb_probe,
	.disconnect	= usb_disconnect,
	.id_table	= usb_table,
};


//********************************************
// all the next can be replaced with :
module_usb_driver(__usb_driver);
// registration& desenregistration of the usb driver with isb subsystem (usb core)
usb_register(&usb_driver);
usb_deresgter(&usb_driver);

//init and exit functions where we call the registration routines
static int __int usb_init(void)
{
	return usb_driver(&usb_driver);
}

static void __exit usb_exit(void)
{
	usb_deregister(&usb_driver);
}

module_init(usb_init);
module_exit(usb_exit);
//******************************************




