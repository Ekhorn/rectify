# Rectify

Hacking a stupid AI cloud connected camera.

## Console access

### Attempt 1 - USB ports

Plugging in to the camera USB ports didn't work. Didn't get anything back from `lsusb`.

```
sudo dmesg | grep tty
sudo dmesg | tail -n 10
[281392.598129] usb usb1-port3: Cannot enable. Maybe the USB cable is bad?
[281394.594252] usb usb1-port3: attempt power cycle
[281397.031118] usb 1-3: new full-speed USB device number 22 using xhci_hcd
[281397.238466] usb 1-3: Device not responding to setup address.
[281397.446123] usb 1-3: device not accepting address 22, error -71
[281397.446215] usb 1-3: WARN: invalid context state for evaluate context command.
[281397.450310] usb usb1-port3: unable to enumerate USB device
[281399.726134] usb usb1-port3: Cannot enable. Maybe the USB cable is bad?
[281401.722261] usb usb1-port3: attempt power cycle
```

### Attempt 2 - UART controllers

I tried the UART controller(s) instead.

Configured a Raspberry PI 4 for serial communication.

```sh
sudo raspi-config
```

Choose `Interface Options → Serial Port` (shell messages on the serial connection).

> Would you like a login shell to be accessible over serial?

Select No. The PI is the client not the login shell.

> Would you like the serial port hardware to be enabled?

Select Yes. Then reboot the PI.

Connect the following pins:

- Pi Pin 8 (TXD) → Target Device RX
- Pi Pin 10 (RXD) → Target Device TX
- Pi Pin 9 (GND) → Target Device GND

Pinout Raspberry PI 4

```
3v3 Power              01-02    5v Power
GPIO 2 (I2C1 SDA)      03-04    5v Power
GPIO 3 (I2C1 SCL)      05-06    Ground
GPIO 4 (GPCLK0)        07-08    GPIO 14 (UART0 TX)
Ground                 09-10    GPIO 15 (UART0 RX)
GPIO 17                11-12    GPIO 18 (PCM CLK)
GPIO 27                13-14    Ground
GPIO 22                15-16    GPIO 23
3v3 Power              17-18    GPIO 24
GPIO 10 (SPI0 MOSI)    19-20    Ground
GPIO 9 (SPI0 MISO)     21-22    GPIO 25
GPIO 11 (SPI0 SCLK)    23-24    GPIO 8 (SPI0 CE0)
Ground                 25-26    GPIO 7 (SPI0 CE1)
GPIO 0 (EEPROM SDA)    27-28    GPIO 1 (EEPROM SCL)
GPIO 5                 29-30    Ground
GPIO 6                 31-32    GPIO 12 (PWM0)
GPIO 13 (PWM1)         33-34    Ground
GPIO 19 (PCM FS)       35-36    GPIO 16
GPIO 26                37-38    GPIO 20 (PCM DIN)
Ground                 39-40    GPIO 21 (PCM DOUT)
```

I tried the following baudrates, but no luck. There are 2 UART controllers on the board. Holding jumper wires against the GND, TX and RX copper pads does show something, but only gibberish. I tried to solder the jumper wires to the board but they didn't stick. Holding the wires against the pads doesn't work reliably and it scratches the pads.

```sh
screen /dev/serial0 1200
screen /dev/serial0 1800
screen /dev/serial0 2400
screen /dev/serial0 4800
screen /dev/serial0 9600
screen /dev/serial0 19200
screen /dev/serial0 28800
screen /dev/serial0 38400
screen /dev/serial0 57600
screen /dev/serial0 76800
screen /dev/serial0 115200
screen /dev/serial0 230400
screen /dev/serial0 460800
screen /dev/serial0 576000
screen /dev/serial0 921600
```

### Attempt 3 - Getting root access

Decided to solder jumper wires to both UART controllers after they had been scratched and it works! After testing with the PI I no longer got any output.

So decided to use my Arduino as USB-TTL device, which worked after uploading the following sketch to the Arduino.

See sketch [serial_software.ino](sketches/serial_software/serial_software.ino).

Then wiring the Arduino to the board like so:

- Arduino Pin 10 (RXD) → Target Device RX
- Arduino Pin 11 (TXD) → Target Device TX
- Arduino Pin GND → Target Device GND

Lastly, I just used `sudo picocom -b 115200 /dev/ttyUSB0` and pressed `Enter` to gain console access.

```
uname -a
Linux netvue 3.10.14-Archon #1 PSEEMPT Mon Jun 24 14:18:03 CST 2024 miLoe
```

Though I'm was still getting some gibberish. I tried pins 0 and 1 with the following sketch and got clean output, but no console access this time..

See sketch [serial_pass_through.ino](sketches/serial_pass_through/serial_pass_through.ino).

## References

- https://github.com/WelterRocks/Netvue-Hacks
