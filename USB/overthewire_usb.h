// overthewire usb driver header file defining the needed struct 


#ifndef OVERTHEWIRE_USB_H
#define OVERTHEWIRE_USB_H

struct overthewire_usb {
  struct usb_device *udev;
  struct cdev cdev;
  dev_t devno;
  struct kobject kobj;
};

#endif


