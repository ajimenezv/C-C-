/*
 *  chardev.c: Creates a read-only char device that says how many times
 *  you've read from the dev file
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>	/* for put_user */
#include <linux/time.h>

/*  
 *  Prototypes - this would normally go in a .h file
 */
int init_module(void);
void cleanup_module(void);
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

#define SUCCESS 0
#define DEVICE_NAME "lolDev"	/* Dev name as it appears in /proc/devices   */
#define BUF_LEN 80		/* Max length of the message from the device */

/* 
 * Global variables are declared as static, so are global within the file. 
 */

static int Major;		/* Major number assigned to our device driver */
static int Device_Open = 0;	/* Is device open?  
				 * Used to prevent multiple access to device */
static char msg[BUF_LEN];	/* The msg the device will give when asked */
static char *msg_Ptr;
static struct timespec curr_tm;
static unsigned long thours;
static unsigned long tmin;
static unsigned long tsecs;
static unsigned long tnsecs;
static struct file_operations fops = {
	.read = device_read,
	.write = device_write,
	.open = device_open,
	.release = device_release
};

/*
 * This function is called when the module is loaded
 */
int init_module(void)
{
        Major = register_chrdev(0, DEVICE_NAME, &fops);

	if (Major < 0) {
	  printk(KERN_ALERT "Registering char device failed with %d\n", Major);
	  return Major;
	}

	getnstimeofday(&curr_tm);
	thours=(curr_tm.tv_sec / 3600) % (24);
	tmin=	(curr_tm.tv_sec / 60) % (60);
	tsecs= (curr_tm.tv_sec % 60);
	tnsecs=(curr_tm.tv_nsec / 1000);

	printk(KERN_INFO "TIME INIT: %.2lu:%.2lu:%.2lu:%.6lu \r\n",thours,tmin,tsecs,tnsecs);
	sprintf(msg, "TIME INIT: %.2lu:%.2lu:%.2lu:%.6lu \r\n",	((curr_tm.tv_sec / 3600) % (24))-thours,	((curr_tm.tv_sec / 60) % (60))-tmin,	(curr_tm.tv_sec % 60)-tsecs,	(curr_tm.tv_nsec / 1000)-tnsecs);
	msg_Ptr = msg;
//	printk(KERN_INFO "TIME INIT: %.2lu:%.2lu:%.2lu:%.6lu \r\n",	(curr_tm.tv_sec / 3600) % (24),	(curr_tm.tv_sec / 60) % (60),	curr_tm.tv_sec % 60,	curr_tm.tv_nsec / 1000);

	printk(KERN_INFO "I was assigned major number %d. To talk to\n", Major);
	printk(KERN_INFO "the driver, create a dev file with\n");
	printk(KERN_INFO "'mknod /dev/%s c %d 0'.\n", DEVICE_NAME, Major);
	printk(KERN_INFO "Try various minor numbers. Try to cat and echo to\n");
	printk(KERN_INFO "the device file.\n");
	printk(KERN_INFO "Remove the device file and module when done.\n");

	return SUCCESS;
}

/*
 * This function is called when the module is unloaded
 */
void cleanup_module(void)
{
	/* 
	 * Unregister the device 

	int ret = unregister_chrdev(Major, DEVICE_NAME);
	if (ret < 0)
		printk(KERN_ALERT "Error in unregister_chrdev: %d\n", ret);
	 */
	getnstimeofday(&curr_tm);
	printk(KERN_INFO "TIME CLEAN UP: %.2lu:%.2lu:%.2lu:%.6lu \r\n",	((curr_tm.tv_sec / 3600) % (24))-thours,	((curr_tm.tv_sec / 60) % (60))-tmin,	(curr_tm.tv_sec % 60)-tsecs,	(curr_tm.tv_nsec / 1000)-tnsecs);
	sprintf(msg, "TIME CLEAN UP: %.2lu:%.2lu:%.2lu:%.6lu \r\n",	((curr_tm.tv_sec / 3600) % (24))-thours,	((curr_tm.tv_sec / 60) % (60))-tmin,	(curr_tm.tv_sec % 60)-tsecs,	(curr_tm.tv_nsec / 1000)-tnsecs);
	msg_Ptr = msg;
 unregister_chrdev(Major, DEVICE_NAME);  // this function is always expected to succeed
}

/*
 * Methods
 */

/* 
 * Called when a process tries to open the device file, like
 * "cat /dev/chardev_OSD_1"
 */
static int device_open(struct inode *inode, struct file *file)
{
	static int counter = 0;

	if (Device_Open)
		return -EBUSY;
	getnstimeofday(&curr_tm);
	printk(KERN_INFO "TIME OPEN: %.2lu:%.2lu:%.2lu:%.6lu \r\n",	((curr_tm.tv_sec / 3600) % (24))-thours,	((curr_tm.tv_sec / 60) % (60))-tmin,	(curr_tm.tv_sec % 60)-tsecs,	(curr_tm.tv_nsec / 1000)-tnsecs);

	Device_Open++;
	sprintf(msg, "TIME OPEN: %.2lu:%.2lu:%.2lu:%.6lu \r\n",	((curr_tm.tv_sec / 3600) % (24))-thours,	((curr_tm.tv_sec / 60) % (60))-tmin,	(curr_tm.tv_sec % 60)-tsecs,	(curr_tm.tv_nsec / 1000)-tnsecs);
	msg_Ptr = msg;
	try_module_get(THIS_MODULE);				// increments the use counter

	return SUCCESS;
}

/* 
 * Called when a process closes the device file.
 */
static int device_release(struct inode *inode, struct file *file)
{
	Device_Open--;		/* We're now ready for our next caller */

	/* 
	 * Decrement the usage count, otherwise once you opened the file, you'll
	 * never get rid of the module. 
	 */
	getnstimeofday(&curr_tm);
	printk(KERN_INFO "TIME RELEASE: %.2lu:%.2lu:%.2lu:%.6lu \r\n",	((curr_tm.tv_sec / 3600) % (24))-thours,	((curr_tm.tv_sec / 60) % (60))-tmin,	(curr_tm.tv_sec % 60)-tsecs,	(curr_tm.tv_nsec / 1000)-tnsecs);
	sprintf(msg, "TIME RELEASE: %.2lu:%.2lu:%.2lu:%.6lu \r\n",	((curr_tm.tv_sec / 3600) % (24))-thours,	((curr_tm.tv_sec / 60) % (60))-tmin,	(curr_tm.tv_sec % 60)-tsecs,	(curr_tm.tv_nsec / 1000)-tnsecs);
	msg_Ptr = msg;
	module_put(THIS_MODULE);		// decrements. the use counter

	return 0;
}

/* 
 * Called when a process, which already opened the dev file, attempts to
 * read from it.
 */
static ssize_t device_read(struct file *filp,	/* see include/linux/fs.h   */
			   char *buffer,	/* USER buffer to fill with data */
			   size_t length,	/* length of the buffer     */
			   loff_t * offset)
{
	/*
	 * Number of bytes actually written to the buffer 
	 */
	int bytes_read = 0;
	/*
	 * If we're at the end of the message, 
	 * return 0 signifying end of file 
	 */
	if (*msg_Ptr == 0)
		return 0;

	/* 
	 * Actually put the data into the buffer 
	 */

	getnstimeofday(&curr_tm);
	printk(KERN_INFO "TIME READ: %.2lu:%.2lu:%.2lu:%.6lu \r\n",	((curr_tm.tv_sec / 3600) % (24))-thours,	((curr_tm.tv_sec / 60) % (60))-tmin,	(curr_tm.tv_sec % 60)-tsecs,	(curr_tm.tv_nsec / 1000)-tnsecs);
	sprintf(msg, "TIME READ: %.2lu:%.2lu:%.2lu:%.6lu \r\n",	((curr_tm.tv_sec / 3600) % (24))-thours,	((curr_tm.tv_sec / 60) % (60))-tmin,	(curr_tm.tv_sec % 60)-tsecs,	(curr_tm.tv_nsec / 1000)-tnsecs);
	msg_Ptr = msg;
//	printk(KERN_INFO "length =  %d.\n", length);  4096
	while (length && *msg_Ptr) {

		/* 
		 * The buffer is in the user data segment, not the kernel 
		 * segment so "*" assignment won't work.  We have to use 
		 * put_user which copies data from the kernel data segment to
		 * the user data segment. 
		 */
		put_user(*(msg_Ptr++), buffer++);

		length--;
		bytes_read++;
	}

	/* 
	 * Most read functions return the number of bytes put into the buffer
	 */
	return bytes_read;
}

/*  
 * Called when a process writes to dev file: echo "hi" > /dev/hello 
 */
static ssize_t
device_write(struct file *filp, const char *buff, size_t len, loff_t * off)
{
	getnstimeofday(&curr_tm);
	printk(KERN_INFO "TIME WRITE: %.2lu:%.2lu:%.2lu:%.6lu \r\n",	((curr_tm.tv_sec / 3600) % (24))-thours,	((curr_tm.tv_sec / 60) % (60))-tmin,	(curr_tm.tv_sec % 60)-tsecs,	(curr_tm.tv_nsec / 1000)-tnsecs);
	sprintf(msg, "TIME WRITE: %.2lu:%.2lu:%.2lu:%.6lu \r\n",	((curr_tm.tv_sec / 3600) % (24))-thours,	((curr_tm.tv_sec / 60) % (60))-tmin,	(curr_tm.tv_sec % 60)-tsecs,	(curr_tm.tv_nsec / 1000)-tnsecs);
	msg_Ptr = msg;
	return -EINVAL;
}

