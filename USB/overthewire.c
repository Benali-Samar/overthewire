#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/usb.h>
#include <linux/fs.h> // for file operations and related functions
#include <linux/cdev.h> // for cdev data structure and functions
//#include "overthewire_usb.h"

//-------TO DO--------
// [*]Change major and minor numbers allocation dynamically by malloc ....
// [*]Prepare the data struct for device data analysis

#define MINOR_BASE 250          // change it to 0 maybe ????
#define NUM_MINORS 1 


#define IS_NEW_METHOD_USED ( 0 )

#define USB_VENDOR_ID	( 0x2717 )
#define USB_PRODUCT_ID	( 0xff40 )

#define PRINT_USB_INTERFACE_DESCRIPTOR( i )				\
{									\
	printk("USB_INTERFACE_DESCRIPTOR:\n");				\
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


static struct cdev cdev;
struct overthewire_usb {
  struct usb_device *udev;
  struct cdev cdev;
};

//----Open file function----
static int overthewire_open(struct inode *inode, struct file *file)
{
  pr_info("%s\n", __func__);
  //needed  open func implemeintation
  return 0;
}

//---- Realese file function----
static int overthewire_release(struct inode *inode, struct file *file)
{
  pr_info("%s\n", __func__);
  //needed release func implementation
  return 0;
}

//----Write file function----
static ssize_t overthewire_write(struct file *file, const char *user_buffer, size_t count, loff_t *ppos)
{
  pr_info("%s: recieved data from user space : %*s\n", __func__,count ,user_buffer);
  //needed write func implementation
  return 0;
}

//----Read file function----
static ssize_t overthewire_read(struct file *file, char *buffer, size_t count, loff_t *ppos)
{
  pr_info("%s\n", __func__);
  //needed read func implementation
  return 0;
}


//----File operation----
static const struct file_operations overthewire_fops =
{
  .owner   = THIS_MODULE, 
  .open    = overthewire_open, 
  .release = overthewire_release, 
  .read    = overthewire_read, 
  .write   = overthewire_write,
};


//----class driver----
static struct usb_class_driver overthewire_class = 
{
  .name ="overthewire",
  .fops = &overthewire_fops,
  .minor_base= MINOR_BASE,
};

//----IDTABLE----
const struct usb_device_id usb_table[] = {
	{ USB_DEVICE(USB_VENDOR_ID,USB_PRODUCT_ID) },
	{ }
};

MODULE_DEVICE_TABLE(usb, usb_table);


//----PROBE----
static int usb_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
	int err;
  	unsigned int i;
	unsigned int endpoints_count;
	struct overthewire_usb *dev;

  
  dev_t devno;


	struct usb_host_interface *iface_desc = interface -> cur_altsetting;

	dev_info(&interface -> dev, "USB DRIVER PROBED: VID: 0x%02x\tPID: 0x%02x\n", id->idVendor, id->idProduct);
	endpoints_count = iface_desc -> desc.bNumEndpoints;
	PRINT_USB_INTERFACE_DESCRIPTOR(iface_desc -> desc);
	for (i = 0; i< endpoints_count; i++)
	{
		PRINT_USB_ENDPOINT_DESCRIPTOR(iface_desc -> endpoint[i].desc);
	}
  
  
  
  //allocate memory for the overthewire_usb struct 
  dev = kzalloc(sizeof(struct overthewire_usb),GFP_KERNEL);
  if (!dev)
  {
    dev_err(&interface ->dev ,"Memory allocation failed\n");
    return -ENOMEM;
  }
  
  
  dev -> udev = usb_get_dev((interface_to_usbdev(interface)));

  //init cdev
  cdev_init(&cdev,&overthewire_fops);
  cdev.owner= THIS_MODULE;

  // Dynamically allocate minor number
  err = alloc_chrdev_region(&devno, 0, NUM_MINORS, "overthewire");
    if (err) {
        dev_err(&interface->dev, "Error allocating character device region\n");
        usb_put_dev(dev->udev);
        kfree(dev);
        return err;
    }
 
  
  // cdev registration with the kernel
  err = cdev_add(&dev -> cdev,devno,NUM_MINORS);
  if(err)
  {
    dev_err(&interface->dev,"Error adding character device\n");
    usb_put_dev(dev -> udev);
    kfree(dev);
    unregister_chrdev_region(devno,NUM_MINORS);
    return err;
  }



  usb_set_intfdata(interface,dev);

	return 0;
}

//----DISCONNECT----
static void usb_disconnect(struct usb_interface *interface)
{
	struct overthewire_usb *dev = usb_get_intfdata(interface);
  dev_t devno;
  
  devno = MKDEV(MAJOR(devno), MINOR_BASE);

  usb_deregister_dev(interface,&overthewire_class);
  cdev_del(&dev -> cdev);
  unregister_chrdev_region(devno,NUM_MINORS);

  dev_info(&interface->dev, "USB Driver Disconnected\n");
  
  usb_put_dev(dev -> udev);
  kfree(dev);
}



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
  int err=usb_register(&usb_driver);
  if (err)
  {
    return err;
  }

  //init the character dev
  cdev_init(&cdev,&overthewire_fops);
  return 0;
}

static void __exit usb_exit(void)
{
    usb_deregister(&usb_driver);
    cdev_del(&cdev);
    class_destroy(overthewire_class);
}

module_init(usb_init);
module_exit(usb_exit);
#endif


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Overthewire team");
MODULE_DESCRIPTION(" A start of simple usb driver");
MODULE_VERSION("0.2");
