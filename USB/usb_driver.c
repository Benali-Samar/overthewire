//**********************USB driver first try**********************


// This driver used my usb keyboard's VID and PID, to test another device they should be changed,
// Use "lsusb" command to see VID and PID numbers

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/usb.h>

#define IS_NEW_METHOD_USED ( 1 )

#define USB_VENDOR_ID	( 0x413c )
#define USB_PRODUCT_ID	( 2113 )

#define PRINT_USB_INTERFACE_DESCRIPTOR( i )				\
{									\
	pr_info("USB_INTERFACE_DESCRIPTOR:\n");				\
	pr_info("------------------------\n");				\
	pr_info("bLength: 0x%x\n",i.bLength);				\
	pr_info("bDescriptorType: 0x%x\n", i.bDescriptorType);		\
	pr_info("bInterfaceNumber: 0x%x\n",i.bInterfaceNumber);		\
	pr_info("bAlternateSetting:0x%x\n", i.bAlternateSetting);	\
	pr_info("bNumEndpoints: 0x%x\n", i.bNumEndpoints);		\
	pr_info("bInterfaceClass: 0x%x\n",i.bInterfaceClass);		\
	pr_info("bInterfaceSubClass: 0x%x\n", i.bInterfaceSubClass);	\
	pr_info("bInterfaceProtocol:0x%x\n", i.bInterfaceProtocol);	\
	pr_info("iInterface: 0x%x\n", i.iInterface);			\
	pr_info("\n");							\
}

#define PRINT_USB_ENDPOINT_DESCRIPTOR( e )	                        \
{	                                                                \
	pr_info("USB_ENDPOINT_DESCRIPTOR:\n");                          \
	pr_info("------------------------\n");                          \
	pr_info("bLength: 0x%x\n", e.bLength);                          \
	pr_info("bDescriptorType: 0x%x\n", e.bDescriptorType);          \
	pr_info("bEndPointAddress: 0x%x\n", e.bEndpointAddress);        \
	pr_info("bmAttributes: 0x%x\n", e.bmAttributes);                \
	pr_info("wMaxPacketSize: 0x%x\n", e.wMaxPacketSize);            \
	pr_info("bInterval: 0x%x\n", e.bInterval);                      \
	pr_info("\n");                                                  \
}


//----PROBE----
static int usb_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
	unsigned int i;
	unsigned int endpoints_count;
	struct usb_host_interface *iface_desc = interface -> cur_altsetting;

	dev_info(&interface -> dev, "USB DRIVER PROBED: VID: 0x%02x\t", "PID: 0x%02x\n", id -> idVendor, id -> idProduct);
	endpoints_count = iface_desc -> desc.bNumEndpoints;
	PRINT_USB_INTERFACE_DESCRIPTOR(iface_desc -> desc);
	for (i = 0; i< endpoints_count; i++)
	{
		PRINT_USB_ENDPOINT_DESCRIPTOR(iface_desc -> endpoint[i].desc);
	}
	return 0;
}

//----DISCONNECT----
static void usb_disconnect(struct usb_interface *interface)
{
	dev_info(&interface->dev, "USB Driver Disconnected\n");
}

//----IDTABLE----
const struct usb_device_id usb_table[] = {
	{ USB_DEVICE( USB_VENDOR_ID, USB_PRODUCT_ID ) },
	{ }
};

MODULE_DEVICE_TABLE(usb, usb_table);


//----USB_DRIVER-----
static struct usb_driver usb_driver = {
    .name       = "overthewire",
    .probe      = usb_probe,
    .disconnect = usb_disconnect,
    .id_table   = usb_table,
};


//----Module registration/deregistration----
// here using preprocessors directives is for making sure that the code is evaluated at compile time not at runtime!
#if ( IS_NEW_METHOD_USED == 0 )
module_usb_driver(usb_driver);

#else
static int __init usb_init(void)
{
	return usb_register(&usb_driver);
}
static void __exit usb_exit(void)
{
	usb_deregister(&usb_driver);
}
module_init(usb_init);
module_exit(usb_exit);
#endif


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Overthewire team");
MODULE_DESCRIPTION(" A start of simple usb driver");
MODULE_VERSION("0");
