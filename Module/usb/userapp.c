
#include <stdio.h>
#include <string.h>
#include <ftw.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <sys/ioctl.h>
#include <linux/usbdevice_fs.h>

#define	TEST_CASES	30

struct usbtest_param {
	unsigned		test_num;	
	unsigned		iterations;
	unsigned		length;
	unsigned		vary;
	unsigned		sglen;

	struct timeval		duration;
};
#define USBTEST_REQUEST	_IOWR('U', 100, struct usbtest_param)


struct usb_device_descriptor {
	__u8  bLength;
	__u8  bDescriptorType;
	__u16 bcdUSB;
	__u8  bDeviceClass;
	__u8  bDeviceSubClass;
	__u8  bDeviceProtocol;
	__u8  bMaxPacketSize0;
	__u16 idVendor;
	__u16 idProduct;
	__u16 bcdDevice;
	__u8  iManufacturer;
	__u8  iProduct;
	__u8  iSerialNumber;
	__u8  bNumConfigurations;
} __attribute__ ((packed));

enum usb_device_speed {
	USB_SPEED_UNKNOWN = 0,		
	USB_SPEED_LOW, USB_SPEED_FULL,	
	USB_SPEED_HIGH				

static char *speed (enum usb_device_speed s)
{
	switch (s) {
	case USB_SPEED_UNKNOWN:	return "unknown";
	case USB_SPEED_LOW:	return "low";
	case USB_SPEED_FULL:	return "full";
	case USB_SPEED_HIGH:	return "high";
	default:		return "??";
	}
}

struct testdev {
	struct testdev		*next;
	char			*name;
	pthread_t		thread;
	enum usb_device_speed	speed;
	unsigned		ifnum : 8;
	unsigned		forever : 1;
	int			test;

	struct usbtest_param	param;
};
static struct testdev		*testdevs;

static int is_testdev (struct usb_device_descriptor *dev)
{

	if (dev->idVendor == 0x0547 && dev->idProduct == 0x1002)
		return 1;

	if (dev->idVendor == 0x0547 && dev->idProduct == 0x2235)
		return 1;

	if (dev->idVendor == 0x04b4 && dev->idProduct == 0x8613)
		return 1;

	if (dev->idVendor == 0x0547 && dev->idProduct == 0x0080)
		return 1;

	if (dev->idVendor == 0x06cd && dev->idProduct == 0x010b)
		return 1;

	if (dev->idVendor == 0x0525 && dev->idProduct == 0xa4a0)
		return 1;

	if (dev->idVendor == 0x0525 && dev->idProduct == 0xa4a4)
		return 1;

	if (dev->idVendor == 0x0525 && dev->idProduct == 0xa4a3)
		return 1;

	if (dev->idVendor == 0xabcd && dev->idProduct == 0x1234)
		return 1;

	if (dev->idVendor == 0x0b62 && dev->idProduct == 0x0059)
		return 1;

	return 0;
}

static int find_testdev (const char *name, const struct stat *sb, int flag)
{
	int				fd;
	struct usb_device_descriptor	dev;

	if (flag != FTW_F)
		return 0;

	if (strrchr (name, '/')[1] == 'd')
		return 0;

	if ((fd = open (name, O_RDONLY)) < 0) {
		perror ("can't open dev file r/o");
		return 0;
	}
	if (read (fd, &dev, sizeof dev) != sizeof dev)
		fputs ("short devfile read!\n", stderr);
	else if (is_testdev (&dev)) {
		struct testdev		*entry;

		if ((entry = calloc (1, sizeof *entry)) == 0) {
			fputs ("no mem!\n", stderr);
			goto done;
		}
		entry->name = strdup (name);
		if (!entry->name) {
			free (entry);
			goto done;
		}

		entry->ifnum = 0;

		fprintf (stderr, "%s speed\t%s\n",
				speed (entry->speed), entry->name);

		entry->next = testdevs;
		testdevs = entry;
	}

done:
	close (fd);
	return 0;
}

static int
usbdev_ioctl (int fd, int ifno, unsigned request, void *param)
{
	struct usbdevfs_ioctl	wrapper;

	wrapper.ifno = ifno;
	wrapper.ioctl_code = request;
	wrapper.data = param;

	return ioctl (fd, USBDEVFS_IOCTL, &wrapper);
}

static void *handle_testdev (void *arg)
{
	struct testdev		*dev = arg;
	int			fd, i;
	int			status;

	if ((fd = open (dev->name, O_RDWR)) < 0) {
		perror ("can't open dev file r/w");
		return 0;
	}

restart:
	for (i = 0; i < TEST_CASES; i++) {
		if (dev->test != -1 && dev->test != i)
			continue;
		dev->param.test_num = i;

		status = usbdev_ioctl (fd, dev->ifnum,
				USBTEST_REQUEST, &dev->param);
		if (status < 0 && errno == EOPNOTSUPP)
			continue;

		if (status < 0) {
			char	buf [80];
			int	err = errno;

			if (strerror_r (errno, buf, sizeof buf)) {
				snprintf (buf, sizeof buf, "error %d", err);
				errno = err;
			}
			printf ("%s test %d --> %d (%s)\n",
				dev->name, i, errno, buf);
		} else
			printf ("%s test %d, %4d.%.06d secs\n", dev->name, i,
				(int) dev->param.duration.tv_sec,
				(int) dev->param.duration.tv_usec);

		fflush (stdout);
	}
	if (dev->forever)
		goto restart;

	close (fd);
	return arg;
}

int main (int argc, char **argv)
{
	int			c;
	struct testdev		*entry;
	char			*device;
	int			all = 0, forever = 0, not = 0;
	int			test = -1 /* all */;
	struct usbtest_param	param;

	param.iterations = 1000;
	param.length = 512;
	param.vary = 512;
	param.sglen = 32;

	device = getenv ("DEVICE");

	while ((c = getopt (argc, argv, "D:ac:g:hns:t:v:")) != EOF)
	switch (c) {
	case 'D':
		device = optarg;
		continue;
	case 'a':	
		device = 0;
		all = 1;
		continue;
	case 'c':	
		param.iterations = atoi (optarg);
		if (param.iterations < 0)
			goto usage;
		continue;
	case 'g':
		param.sglen = atoi (optarg);
		if (param.sglen < 0)
			goto usage;
		continue;
	case 'l':
		forever = 1;
		continue;
	case 'n':	
		not = 1;
		continue;
	case 's':
		param.length = atoi (optarg);
		if (param.length < 0)
			goto usage;
		continue;
	case 't':
		test = atoi (optarg);
		if (test < 0)
			goto usage;
		continue;
	case 'v':	
		param.vary = atoi (optarg);
		if (param.vary < 0)
			goto usage;
		continue;
	case '?':
	case 'h':
	default:
usage:
		fprintf (stderr, "usage: %s [-an] [-D dev]\n"
			"\t[-c iterations]  [-t testnum]\n"
			"\t[-s packetsize] [-g sglen] [-v vary]\n",
			argv [0]);
		return 1;
	}
	if (optind != argc)
		goto usage;
	if (!all && !device) {
		fprintf (stderr, "must specify '-a' or '-D dev', "
			"or DEVICE=/proc/bus/usb/BBB/DDD in env\n");
		goto usage;
	}

	if ((c = open ("/proc/bus/usb/devices", O_RDONLY)) < 0) {
		fputs ("usbfs files are missing\n", stderr);
		return -1;
	}

	if (ftw ("/proc/bus/usb", find_testdev, 3) != 0) {
		fputs ("ftw failed; is usbfs missing?\n", stderr);
		return -1;
	}

	if (!testdevs && !device) {
		fputs ("no test devices recognized\n", stderr);
		return -1;
	}
	if (not)
		return 0;
	if (testdevs && testdevs->next == 0 && !device)
		device = testdevs->name;
	for (entry = testdevs; entry; entry = entry->next) {
		int	status;

		entry->param = param;
		entry->forever = forever;
		entry->test = test;

		if (device) {
			if (strcmp (entry->name, device))
				continue;
			return handle_testdev (entry) != entry;
		}
		status = pthread_create (&entry->thread, 0, handle_testdev, entry);
		if (status) {
			perror ("pthread_create");
			continue;
		}
	}
	if (device) {
		struct testdev		dev;

		fprintf (stderr, "%s: %s may see only control tests\n",
				argv [0], device);

		memset (&dev, 0, sizeof dev);
		dev.name = device;
		dev.param = param;
		dev.forever = forever;
		dev.test = test;
		return handle_testdev (&dev) != &dev;
	}
	for (entry = testdevs; entry; entry = entry->next) {
		void	*retval;

		if (pthread_join (entry->thread, &retval))
			perror ("pthread_join");
	}

	return 0;
}
