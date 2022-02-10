#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/pci.h>
#include<linux/proc_fs.h>
#include <linux/seq_file.h>

static struct devId_pci my_pci_driver_ids[]={
    {PCI_DEVICE(PCI_ANY_ID, PCI_ANY_ID)},
    {0,},
};

MODULE_DEVICE_TABLE(pci, my_pci_driver_ids);


static void enum_pci(void) {
    struct pci_dev *pci_device;

    pci_device = NULL;
    while ((pci_device = pci_get_device(PCI_ANY_ID, PCI_ANY_ID, pci_device))){
        pr_info("PCI Driver: %s\t: Vendor ID: %02x\t Device ID: %02x\t Config Size:%d", pci_name(pci_device),\
                pci_device->vendor, pci_device->device, pci_device->cfg_size);
    }
}


int driver_probe(struct pci_dev *dev, const struct devId_pci *id) {
    pr_info("PCI Driver: My PCI Driver is Probed");
    enum_pci();
    return 0;
}

void  remove_driver(struct pci_dev *dev) {
    pr_info("PCI Driver: My PCI Driver is Removed");
}

static struct pci_driver pci_driver ={
    .name="my_pci_driver",
    .id_table=my_pci_driver_ids,
    .probe=driver_probe,
    .remove=my_pci_driver_remove,
};

static int __init driver_init(void)
{
   int ret;
   ret = pci_register_driver(&pci_driver);
   if (ret<0)
   {
       pr_err("PCI Driver: My PCI Driver registration failed");
       return ret;
   }

   pr_info("PCI Driver: My PCI Driver Registration Passed ret:%d", ret);

   return ret;
}

static void __exit driver_exit(void)
{
   pci_unregister_driver(&pci_driver);
   pr_info("PCI Driver: My PCI Driver is unregistered.... exiting");
}


module_init(driver_init);
module_exit(driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("VISHAL VERMA");

