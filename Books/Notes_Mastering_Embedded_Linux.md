
# Chapter- 9

In Linux, there are three main types of device driver:
1. Character
2. Block
3. Network
   
> Character devices are identified in user space by a special file called a device
node. This file name is mapped to a device driver using the major and minor
numbers associated with it. Broadly speaking, the major number maps the
device node to a particular device driver, and the minor number tells the driver
which interface is being accessed.

> Block devices are also associated with a device node, which also has major and
minor numbers.
Although character and block devices are identified using major
and minor numbers, they are in different namespaces. A character
driver with a major number 4 is in no way related to a block driver
with a major number 4.

> Network devices are not accessed through device nodes, and they do not have
major and minor numbers. Instead, a network device is allocated a name by the
kernel, based on a string and an instance number.

## Finding out about drivers at runtime

1. You can list the character and block device drivers currently loaded
and active by reading /proc/devices:

2. For each driver, you can see the major number and the base name. However, this
does not tell you how many devices each driver is attached to.

3. network devices do not appear in this list, because they do not have
device nodes. Instead, you can use tools such as ifconfig or ip to get a list of
network devices:

4. You can also find out about devices attached to USB or PCI buses using the
well-known commands: lsusb and lspci.

5. since the Linux device driver model represents all
devices and drivers as kernel objects, you can see the kernel's view of the system
laid out before you by looking in /sys, as shown here:

6. In the context of discovering information about devices and drivers, I will look
at three of these directories: devices, class, and block.

## The devices: /sys/devices

> platform software system tracepoint virtual
There are three directories that are present on all systems:
system/: This contains devices at the heart of the system, including CPUs and
clocks.
virtual/: This contains devices that are memory-based. You will find the
memory devices that appear as /dev/null, /dev/random, and /dev/zero in
virtual/mem. You will find the loopback device, lo, in virtual/net.
platform/: This is a catch-all for devices that are not connected via a
conventional hardware bus. This maybe almost everything on an embedded
device.

## The drivers: /sys/class

> This is a view of the device drivers presented by their type. In other words, it is a
software view rather than a hardware view. Each of the subdirectories represents
a class of driver and is implemented by a component of the driver framework.
For example, UART devices are managed by the tty layer, and you will find
them in /sys/class/tty. Likewise, you will find network devices in /sys/class/net,
input devices such as the keyboard, the touchscreen, and the mouse in
/sys/class/input, and so on.

## The block drivers: /sys/block

> There is one more view of the device model that is important to this discussion:
the block driver view that you will find in /sys/block. There is a subdirectory for
each block device.


# Finding the right device driver

> 1. A typical embedded board is based on a reference design from the manufacturer
with changes to make it suitable for a particular application. The BSP that comes
with the reference board should support all of the peripherals on that board. But,
then you customize the design, perhaps by adding a temperature sensor attached
via I2C, some lights and buttons connected via GPIO pins, a display panel via a
MIPI interface, or many other things. Your job is to create a custom kernel to
control all of these, but where do you start to look for device drivers to support
all of these peripherals?

# Device drivers in user space 

> Before you start writing a device driver, pause for a moment to consider whether
it is really necessary. There are generic device drivers for many common types
of device that allow you to interact with hardware directly from user space
without having to write a line of kernel code. User space code is certainly easier
to write and debug.

## GPIO
> 1. General-Purpose Input/Output (GPIO) is the simplest form of digital
interface since it gives you direct access to individual hardware pins, each of
which can be in one of two states: either high or low. In most cases you can
configure the GPIO pin to be either an input or an output.
> 2. The
main limitation is the speed and accuracy of the software loops and the number
of CPU cycles you want to dedicate to them. Generally speaking, it is hard to
achieve timer accuracy better than a millisecond unless you configure a real-time
kernel, as we shall see in Chapter 16, Real-Time Programming.
> 3. Most SoCs have a lot of GPIO bits, which are grouped together in GPIO
registers, usually 32 bits per register. On-chip GPIO bits are routed through to
GPIO pins on the chip package via a multiplexer, known as a pin mux. There
maybe additional GPIO pins available off-chip in the power management chip,
and in dedicated GPIO extenders, connected through I2C or SPI buses. All this
diversity is handled by a kernel subsystem known as gpiolib, which is not
actually a library but the infrastructure GPIO drivers use to expose I/O in a
consistent way. There are details about the implementation of gpiolib in the
kernel source in Documentation/gpio and the code for the drivers themselves is in
drivers/gpio.
Applications can interact with gpiolib through files in the /sys/class/gpio directory.

## I2C
> 1. I2C is a simple low speed 2-wire bus that is common on embedded boards,
typically used to access peripherals that are not on the SoC, such as display
controllers, camera sensors, GPIO extenders, and so on. There is a related
standard known as system management bus (SMBus) that is found on PCs,
which is used to access temperature and voltage sensors. SMBus is a subset of
I2C.

> 2. The device interface provides a series of ioctl commands that query the host
controller and send the read and write commands to I2C slaves. There is a package
named i2c-tools, which uses this interface to provide basic command-line tools to
interact with I2C devices. The tools are as follows:
i2cdetect: This lists the I2C adapters, and probes the bus

>i2cdump: This dumps data from all the registers of an I2C peripheral

>i2cget: This reads data from an I2C slave

>i2cset: This writes data to an I2C slave

The i2c-tools package is available in Buildroot and the Yocto Project as well as
most mainstream distributions.

> 3. There is more information about the Linux implementation of I2C in
Documentation/i2c/dev-interface. The host controller drivers are in
drivers/i2c/busses.
