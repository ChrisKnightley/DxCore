# DxCore - Arduino support for the NEW AVR DA-series, DB-series and upcoming DD-series
**Please be on watch for typos in the docs and report them. Codespell is not a very good spellchecker. (Obviously, PR's to fix are even better!)**
**~1.3.7~ 1.3.8 release is here! This has a _LOT_ of changes. Be sure you report problems before you downgrade to 1.3.6 so I can fix them**
(1.3.7 was "released" but contained some issues and was pulled before being added to board manager)

This is an Arduino core to support the exciting new AVR DA, DB, and "coming soon" DD-series microcontrollers from Microchip. These are the latest and highest spec 8-bit AVR microcontrollers from Microchip. It's unclear whether these had been planned to be the "1-series" counterpart to the megaAVR 0-series, or whether such a thing was never planned and these are simply the successor to the megaAVR series. But whatever the story of their origin, these take the AVR architecture to a whole new level.  With up to 128k flash, 16k SRAM, 55 I/O pins, 6 UART ports, 2 SPI and I2C ports, and all the exciting features of the tinyAVR 1-series and megaAVR 0-series parts like the event system, type A/B/D timers, and enhanced pin interrupts... Yet for each of these systems they've added at least one small but significant improvement of some sort (while largely preserving backwards compatibility - the tinyAVR 2-series also typically adds the new features that the Dx-series gt , giving the impression that these reflect a "new version" of . You like the type A timer, but felt constrained by having only one prescaler at a time? Well now you have two of them (on 48-pin parts and up)! You wished you could make a type B timer count events? You can do that now! (this addresses something I always thought was a glaring deficiency of the new peripherals and event system). We still don't have more prescale options (other than having two TCA's to choose from) for the TCB - but you can now combine two TCB's into one, and use it to do 32-bit input capture. Time a pulse or other event up to approximately 180 seconds long... to an accuracy of 24th's of a microsecond! And of course, like all post-2016 AVR devices, these use the latest incarnation of the AVR instruction set, AVRxt, with the slightly-improved instruction timing compared to "classic" AVRs

## Big Picture
These parts depart from the naming scheme used for AVR devices in the past; these are named AVR followed by the size of the flash, in KB, followed by DA, DB, or DD (depending on the "series" or "family", then the number of pins. Note that the pin count also determines how many of certain peripherals the parts have available - parts with more pins have more peripherals to do things with those pins. 64-pin parts are not available in 32k flash size. The 128k flash size is the highest that can be supported with a 16-bit program counter (above that, a number of instructions become slower, and everything gets more complicated), and with the current scheme for interacting with the pins, the 55 I/O pins (56 less the UPDI pin which takes the place of PF7) are the limit of what a modern AVR can accommodate while allowing single cycle access to all pins - so these take them to the top end of what is possible without extensions to the architecture.

At present, it appears that there will be at least four lines of AVR Dx-series parts: The "basic" DA, the DB with improved featureset, the as-yet unreleased low pincount DD-series and potentially a new "DU" series with DD-like low pincounts, which instead of `MVIO`, sacrifice all of `PORTC` in the name of **native USB**. A product brief was available, but only, as the name suggests, briefly, which implies that the retracted product brief may not reflect what will actually come to exist - but the prospect of a new, more capable AVR with native USB is exciting indeed, since the 32u4 was getting a little long in the tooth. This core supports the DA and DB-series parts, and will support the DD-series once those parts become available and the EA-series unless the scale of changes there is greater than expected and a separate core is required.. If/when The DU-series materializes, they would also fall under the perview of this core.

### DA-series
The "basic" full-size line - however much I was in awe of these when they were first released, having seen the DB-series, it now appears that these are more akin to a 0-series than a 1-series - by almost any measure, the DB-series is the same or slightly better (and barely more expensive!). They do not support using an external crystal for the main clock, like the other Dx parts do, but the internal oscillator on these parts is still WAY better than the classic AVRs had - all the ones I've tested are weithin half a percent at room temp and typical operating voltages, even without autotune... To make sure autotune was working, I had to point a torch at it, because I couldn't get enough of a change in the internal oscillator frequency from changing the supply voltage. It is also the only currently announced Dx series without `MVIO`. While they may not shine as brightly next to the other Dx lines, these are still far above any AVR released before the year 2020.

### DB-series
The DB-series is almost an exact copy of the DA-series (they fixed some of the most egregious silicon bugs, though they have hardly been a paragon of rigor ), only with a few MORE exciting features tacked on: Support for a real high-frequency crystal as clock source (seen for the first time since the modern AVR architecture was released in 2016), "MVIO" (multivoltage I/O), where PORTC can run at a different voltage than the rest of the chip (essentially, a builtin bidirectional level shifter). The other "headline feature", is the  two (28/32-pin parts) or three (48/64-pin parts) on-chip opamps, with software-controlled MUX for the inputs and an on-chip feedback resistor ladder. These can be used as gain stage for the ADC, for example, or to buffer the DAC output (though these opamps can't supply much current, they can supply tens of mA, instead of ~ 1 like the unaided DAC), connected together like the CCL LUTs. etc (on parts with 3, you can even connected them together as an instrumentation amplifier). The included `<Opamp.h>` library provides a minimalist wrapper around these, though you should still expect to refer to the datasheet when working with them..

### DD-series
The DD-series is a smaller-pincount line; parts will be available with 14-32 pins. They've got the `MVIO` (3 or 4 MVIO pins depending on pincount). The product brief claims 10 output pins, 11 input pins,  on the 14-pin package. With VDD, VDDIO, and GND. That implies that there will be a way to configure the UPDI pin to act as an I/O pin, and Reset to act as an input only if configured appropirately in the fuses. We'll have to wait until more information is available, but it sounds like the reset pin on these parts will be the pin that needs the HV pulse. One thing worth noting is that the 28-pin and 32-pin DD-series parts are, but for expanded port multiplexing options for `SPI0`, `USART0`, and `TCD0`, and the addition of ADC input functionality on pins in `PORTA` and `PORTC`,  One imagines that, since these do have enhanced `PORTMUX` options for TCD0, and since Microchip has been aware since the summer of 2020 that the TCD0 `PORTMUX` options other than the default were broken on DA-series and DB-series, we should anticipate that this issue should be fixed in the DD-series.

### EA-series
The EA-series is known only from it's product brief. Word is that it will be available only in 28-48 pin packages, with up to 64k of flash, no DAC or Type D timer, but in exchange, there will be dual type A timers in all pincounts, and it will feature the 12-bit true differential ADC with programmable gain amplifier that the tinyAVR 2-series has.
The most puzzling thing about these, though is that some of their specs like the oscillator seem to represent a step backwards. On the third hand, they will represent A NEW version of the NVM controller, being the first modern AVR to advertise NRWW and RWW sections of flash. As more information becomes available the determination of whether support for these partswill be through DxCore or a new core will be made, but unless there are substantial changes to other systems, we will likely keep it in DxCore.

### DU-series (details unconfirmed)
The DU-series was described by a product brief which was almost immediately pulled down. So at least part of what follows (which is based on that document) is likely not consistent with what will be released, because otherwise, why retract the product brief. The product brief described a part appearing similar to the DD-series (with similar 14, 20, 28, and 32 pin packages) - but where the DD's had a group of MVIO pins, The DU has USB pins - that is, the DU-series is the long awaited modern AVR with native USB! (and it would still have the MVIO functionality on those pins, just hardwired for 3.3v) The DAC is gone, as is the TCD (TCD and it's PLL presumably swallowed by the USB interface to generate the 48 MHz internal clock needed compliant USB 2.0). In it's place is real proper USB! Features included optional 3.3v regulator (USB data is 3.3v logic level, even though USB power is 5v), 16 endpoints. "Multi-packet transfer" and "crystalless USB" by tuning the system clock against the USB clock (just like VUSB does - only done more rigorouslty and in dedicated silicon - one hopes at least). Otherwise, it looks much like the DD-series, with 2 TCBs and 1 TCA, 6 event channels, 4 CCL blocks. They also list a few intriguing self-programming features, "immutable boot" and a "boot row" Pin mapping of peripherals matched the DD-series other than the USB pins, and it was shown with 16, 32, or 64k of flash and 2/4/8k RAM, with the largest memory version being available only in the two largest pincounts. Since the document was pulled, that suggests that there may be some significant difference between this and what is eventually released. Might I suggest making the large memory version available in low pincounts? people are going to go crazy over those 14-pin parts with native USB. It'd be like a digispark with some extra pins, 4 or 8 times as much flash, and USB that actually works.

## Timeline
128k DA-series parts were released in April 2020, followed by 32k in early summer (though the initial release of the 32k parts suffered from a fatal flaw and was recalled, working ones were not available until the end of 2020), while the 64k parts became available late in summer of 2020 without such brutal heat), around the same time as the first AVR128DB-series parts became available, while the 32 snd 64k versiond arrived in Q2 2021. The AVR DD-series product brief was released in spring of 2020, while headers for the 64k-flash version have been released, essentially confiming what was in the product brief with no serious surprises. The DD was expected to launch in the first half of 2021, but so far, there's been no further news. It's a similar story with the EA-series. Likely the current capacity shortfall is a big part of that. I like to imagine that they're also taking more care to get all the kinks worked out. Errata lists like the DA has are not cool.

## Lots of errata
The silicon errata list in the initial versions of these parts - the DA-series in particular - is... longer than you may be used to if you were accustomed to the classic AVRs. The initial DB silicon looked bad too, but right after the release, a new silicon rev was added to the errata listing which fixed most of the bad ones **AVR128DB** (all the parts I've gotten had the fixes, even the ones I got delivered before they'd released the datasheet....). Sadly, the AVR128DA never got a corresponding pack of fixes. In both cases, the smaller flash versions, released later, incorporated a bunch of fixes. If you've been working with the tinyAVR 0/1-series, many of these errata may be old friends of yours by now. A large number of those issues appear to have sailed through the new chip design process without a remedy - and in late 2020, new silicon bugs appeared in the tinyAVR 0/1 errata and DA/DB errata simultaneously - apparently issues that were discovered on the Dx-series and then found to impact the tinyAVR 0/1-series as well. See [errata and extras](https://github.com/SpenceKonde/DxCore/blob/master/megaavr/extras/Errata.md) for more information.

## Supported Parts (click link for pinout diagram and details)
Note that you must install via board manager or replace your tool chain with the azduino3 version pulled in by board manager in order to work with anything other than an AVR128DA. Note also that there is a defect in the AVR32DA parts: interrupts do not work correctly (the chip has 2-byte vectors in the hardware, instead of 4-byte ones... it's got more than 8k flash, so that's not going to work no matter what - but it *really* doesn't work with the compiler making 4-byte vector binaries!). The AVR32DA parts in circulation have been recalled - It would be mighty nice if they had updated the silicon errata sheet though!
* [AVR128DA28, AVR64DA28, AVR32DA28](https://github.com/SpenceKonde/DxCore/blob/master/megaavr/extras/DA28.md)
* [AVR128DA32, AVR64DA32, AVR32DA32](https://github.com/SpenceKonde/DxCore/blob/master/megaavr/extras/DA32.md)
* [AVR128DA48, AVR64DA48, AVR32DA48](https://github.com/SpenceKonde/DxCore/blob/master/megaavr/extras/DA48.md)
* [AVR128DA64, AVR64DA64](https://github.com/SpenceKonde/DxCore/blob/master/megaavr/extras/DA64.md)
* [AVR128DB28, AVR64DB28, AVR32DB28](https://github.com/SpenceKonde/DxCore/blob/master/megaavr/extras/DB28.md)
* [AVR128DB32, AVR64DB32, AVR32DB32](https://github.com/SpenceKonde/DxCore/blob/master/megaavr/extras/DB32.md)
* [AVR128DB48, AVR64DB48, AVR32DB48](https://github.com/SpenceKonde/DxCore/blob/master/megaavr/extras/DB48.md)
* [AVR128DB64 and AVR64DB64](https://github.com/SpenceKonde/DxCore/blob/master/megaavr/extras/DB64.md)
* AVR64DD14, AVR32DD14, AVR16DD14 (pending release)
* AVR64DD20, AVR32DD20, AVR16DD20 (pending release)
* AVR64DD28, AVR32DD28, AVR16DD28 (pending release)
* AVR64DD32, AVR32DD32, AVR16DD32 (pending release)
**PINOUT CHART NOTE** - the pinout diagrams do not show the TCA0, TCA1, or TCD0 remapping options currently, only the default ones.

My personal opinion is that the 48-pin parts are the "sweet spot" for the DA and DB-series parts - they have the real gems of the product line - the second Type A timer, the two extra CCL LUTs, and enough pins to take full advantage of these peripherals. Most people can't really find something to do with a whole 64 pins in one project - short of indulging in kitchen-sink-ism just to take up pins. But the 27 I/O pins on the 32-pin parts can go faster than one might think (I had one project a while back where I switched to a '328PB instead of a '328P for the Rev. B, because otherwise I was 1 pin short of being able to lose the I2C backpack on the '1602 LCD, and if I did that, I could integrate the whole thing onto one PCB, and have a rigid connection between the LCD and main PCB - though I think I could just squeeze that project into a DA32).

For the upcoming DD-series, the 28 and 32-pin parts are of questionable utility considering the existence of the more capable DA and DB parts with the same number of pins. With AVR Dx parts already priced like the higher-end classic tinyAVR devices, and barely twice the price of the top edge of the tinyAVR 0/1/2-series, there is a pretty narrow range of prices that the DD-series would make sense at. The 14-pin and 20-pin packages are far more interesting, packing Dx-level capabilities intosizes and pincounts that are normally the province of the less capable tinyAVR product line.

## Supported Clock Speeds
All speeds are supported across the whole 1.8V ~ 5.5V operating voltage range!
**Crystal** is not supported as a clock source on the DA-series, but is or will be supported on everything else.
* Within Manufacturer Speciications
  * 24MHz Internal, Ext. Clock or Crystal
  * 20MHz Internal, Ext. Clock or Crystal
  * 16MHz Internal, Ext. Clock or Crystal
  * 12MHz Internal, Ext. Clock or Crystal
  *  8MHz Internal, Ext. Clock or Crystal
  *  4MHz Internal, Ext. Clock or Crystal
  *  1MHz Internal or Ext. Clock
* Overclocked (No guarantees on functioning)
  * 25MHz Ext Clock or Crystal
  * 28MHz Internal, Ext. Clock or Crystal
  * 30MHz Ext. Clock or Crystal
  * 32MHz Internal, Ext. Clock or Crystal
  * 36MHz Ext. Clock or Crystal
  * 40MHz Ext. Clock or Crystal
  * 48MHz Ext. Clock or Crystal

If a watch crystal (but no high frequency one) is installed, there is an option to "Auto-tune" the internal oscillator based on that, though the imoprovement is small except at extreme temperatures. See the [Clock Options reference](https://github.com/SpenceKonde/DxCore/blob/master/megaavr/extras/Ref_Clocks.md)

## UPDI programming
The UPDI programming interface is a single-wire interface for programming (and debugging - **U**niversal **P**rogramming and **D**ebugging **I**nterface) used on the AVR Dx-series, tinyAVR 0/1/2-series, megaAVR 0-series, and which will likely be used for all AVR microcontrollers for the foreseeable future. In addition to purchasing a purpose-made UPDI programmer (such as the ones produced by Microchip), there are two very low-cost approaches to creating a UPDI programmer:

### From a USB-Serial adapter with included SerialUPDI (recommended)
Before megaTinyCore existed, there was a tool called [pyupdi](https://github.com/mraardvark/pyupdi) - a simple python program for uploading to UPDI-equipped microcontrollers using a serial adapter modified by the addition of a single resistor. But pyupdi was not readily usable from the Arduino IDE, and so this was not an option. As of 2.2.0, megaTinyCore brings in a portable Python implementation, which opens a great many doors; Originally we were planning to adapt pyupdi, but at the urging of the pyupdi author and several Microchip employees, we have instead based this functionality on [pymcuprog](https://pypi.org/project/pymcuprog/), a tool developed and maintained by Microchip which includes the same serial-port upload mechanism (though within that context the SerialUPDI capacity is rather hidden - that tool is more about uploading via the native-USB debuggers sold by Microchip. I had initially hoped to use it for that as well, but it appeared that wrangling the USB/driver angle was going to be a problem - so when the fix for avrdude to support the 24-bit firmware on the Microchip Curiosity boards came out, we lost interest in that. **If installing manually** you must [add the python package](https://github.com/SpenceKonde/DxCore/blob/master/megaavr/tools/ManualPython.md) appropriate to your operating system in order to use this upload method (a system python installation is not sufficient, nor is one necessary).

With the release of 2.3.6 and the dramatic improvements in performance of SerialUPDI, **this is now the recommended method to program these parts**
[Instructions for wiring a serial adapter for SerialUPDI](https://github.com/SpenceKonde/AVR-Guidance/blob/master/UPDI/jtag2updi.md)

Note that this does not give you serial monitor - you need to connect a serial adapter the normal way for that (I suggest using two, along with an external serial terminal application). This technique works with those $1 CH340 serial adapters from ebay, aliexpress, etc. Did you accidentally buy some that didn't have a DTR pin broken out, and so weren't very useful with the Pro Minis you hoped to use them with?

### With an Arduino using jtag2updi
One can be made from a classic AVR Uno/Nano/Pro Mini; inexpensive Nano clones are the usual choice, being cheap enough that one can be wired up and then left like that - see [Making a UPDI programmer](https://github.com/SpenceKonde/AVR-Guidance/blob/master/UPDI/jtag2updi.md); using these, you should select jtag2updi from the tools->programmer menu. Prior to the release of 1.3.6  this was the recommended method, despite the balky jtag2updi firmware and incompatibility with converting an Arduino Micro because of the fiddliness and poor performance of SerialUPDI. Those issues have been addressed with a more robusty wiring scheme (replacing the 4.7k resistor with a schottky diode greatly widens the operating window) and software improvements have increased the speed *by a factor of 20-30* for uploading to Dx parts.

### Using a Curiosity Nano
Choose the nEDBG programmer option, and things should just work. There are a lot of things I'm not s huge fan of on those boards, but programming them really should work without particular issue.

### Troubleshooting uploads - quick tips
From experience, there are a few quick things to check before you conclude that there's a more serious problem. These only apply to jtag2updi and SerialUODI
* Check the serial port selected.
* If you get an error about bootloader.BOOTEND, you were using megaTinyCore, with SerialUPDI, and then came to DxCore and didn't select a programmer. The IDE is still set to use the other core's programmer... which is done *using that core's platform.txt*. When you open the tools menu you'll see a that no programmer name is listed next to "programmer". Select one, amd it should work.
* One of the most common problems though is still poor connections. It seems like it would be very hard to have wiring problems with just 1 pin and power and ground (usually one knows if those aren't right). I've been having a lot of problems with crap dupont line.

### Optiboot-derived bootloader (optional)
There is an included Optiboot derived bootloader included with the core. You may use it instead of UPDI programming it it suits your application or preferences better. The bootloader, of course, requires a UPDI programmer to install.
[Optiboot Reference](https://github.com/SpenceKonde/DxCore/blob/master/megaavr/extras/Ref_Optiboot.md)

## Ways to refer to pins
This core uses a simple scheme for assigning the Arduino pin numbers, the same one that [MegaCoreX](https://github.com/MCUDude/MegaCoreX) uses for the pin-compatible megaAVR 0-series parts - pins are numbered starting from PA0, proceeding counterclockwise, which seems to be how the Microchip designers imagined it too.

#### PIN_Pxn Port Pin Numbers (recommended)
**This is the recommended way to refer to pins** Defines are provided of form PIN_Pxn, where x is the letter of the port (A through G), and n is a number 0 ~ 7 - (Not to be confused with the PIN_An defines described below). These just resolve to the digital pin number of the pin in question - they don't go through a different code path. However, they have particular utility in writing code that works across the product line with peripherals that are linked to certain pins (by port), making it much easier to port code between devices with the modern peripherals. Several pieces of demo code in the documentation take advantage of this.

Direct port manipulation is possible on the parts (and is easier to write with if you use PIN_Pxn notation!) - in fact, in some ways direct port manipulation is more powerful than it was in the past. several powerful additional options are available for it - see [direct port manipulation](https://github.com/SpenceKonde/DxCore/blob/master/megaavr/extras/DirectPortManipulation.md).

#### Arduino Pin Numbers (if you must)
When a single number is used to refer to a pin - in the documentation, or in your code - it is always the "Arduino pin number". These are the pin numbers shown on the pinout charts. All of the other ways of referring to pins are #defined to the corresponding Arduino pin number.

#### An and PIN_An constants (for compatibility)
The core also provides An and PIN_An constants (where n is a number from 0 to the number of analog inputs). These refer to the ADC0 *channel* numbers. This naming system is similar to what was used on many classic AVR cores - on some of those, it is used to simplify the code behind `analogRead()` - but here, they are just #defined as the corresponding Arduino pin number. The An names are intentionally not shown on the pinout charts, as this is a deprecated way of referring to pins. However, these channels are shown on the pinout charts as the ADCn markings, and full details are available in the datasheet under the I/O Multiplexing Considerations chapter. There are additionally PIN_An defines for compatibility with the official cores - these likewise point to the digital pin number associated with the analog channel.

#### There is no A0 aka PIN_PD0 aka 12 on DB-series parts with less than 48 pins.
DB-series parts with 32 or 28 pins don't have a an analog channel 0. It's located on pin PD0, which was displaced by the `VDDIO`2 pin. Based on the errata - the PD0 pad exists on the chip... but doesn't have any bond wire attached to it. Per manufacturer recommendations we disable the digital input buffer to save power.

## Exposed Hardware Features

### ADC Support
These parts all have a large number of analog inputs - DA and DB-series have up to 22 analog inputs, while the DD-series has analog input on every pin that is not used to drive the HF crystal (though the pins on PORTC are only supported when MVIO is turned off). They can be read with `analogRead()` like on a normal AVR, and we default to 10-bit resolution; you can change to the full 12-bit with `analogReadResolution()`, and use the enhanced analogRead functions to take automatically oversampled, decimated readings for higher resolution and to take differential measurements. There are 4 internal voltage references in 1.024, 2.048, 4.096 and 2.5V, plus support for external reference voltage (and Vdd of course). ADC readings are taken 3 times faster than an classic AVR, and that speed can be doubled again if what you are measuring is low impedance, or extend the sampling time by a factor greatly for reading very high impedance sources. This is detailed in the analog reference.

### DAC Support
The Dx-series parts have a 10-bit DAC which can generate a real analog voltage (note that this provides low current and can only be used as a voltage reference or control voltage, it cannot be used to power other devices). This generates voltages between 0 and the selected `VREF` (unlike the tinyAVR 1-series, this can be Vcc!). Set the DAC reference voltage via the DACR`eference()` function - pass it any of the ADC reference options listed under the ADC section above (including VDD!). Call `analogWrite()` on the DAC pin (PD6) to set the voltage to be output by the DAC (this uses it in 8-bit mode). To turn off the DAC output, call `digitalWrite()` or `turnOffPWM()` on that pin.

See the [ADC and DAC Reference](https://github.com/SpenceKonde/DxCore/blob/master/megaavr/extras/Ref_Analog.md)

Using the `An` constants is deprecated - the recommended practice is to just use the digital pin number, or better yet, use `PIN_Pxn` notation when calling `analogRead()`. Particularly since the release of 1.3.0 and megaTinyCore 2.3.0, a number of enhanced ADC features have been added to expose more of the power of the sophisticated ADC in these parts to users.


### Watchdog timer, software reset
There are more options than on classic AVR for resetting, including if the code gets hung up somehow. The watchdog timer can only reset (use the RTC and PIT for timed interrupts)
See the [Reset and Watchdog (WDT) Reference](https://github.com/SpenceKonde/DxCore/blob/master/megaavr/extras/Ref_Reset.md)

#### Serial (UART) Support
All of these parts have a several hardware serial ports (USART) - from 3 on the 28-pin parts to SIX on the 64-pin parts! They work exactly like the ones on official Arduino boards. See the pinout charts for the location of the serial pins. On my breakout boards, we provide autoreset support as well (again, just like official Arduino boards). Starting in 1.3.3, USART0 is projected as Serial0. Serial is #defined as Serial0 (so the user need not think about this) whenever using a bare chip, however as explicit support is added for specific development boards where a different port is tied to the USB serial adapter, this will be defined differently for those boards allowing a more transparent user experience, that is Serial.print() will always output to the serial monitor.

On all supported devices, where the appropriate pins are present, they can be pin-swapped - each PORT except PORTD (AVR DD parts don't get one one PORT) gets a USART, which defaults to pins 0 and 1 for RX, TX (2 and 3 for XCK and XDIR - though these are not supported through the Serial class), and 4, 5, 6 and 7 when pinswapped. This is configured using the Serial.swap() or Serial.pins() methods. Both of them achieve the same thing, but differ in how you specify the set of pins to use. This should be called **before** calling Serial.begin().

`Serial.swap(1) or Serial.swap(0)` will set the the mapping to the alternate (1) or default (0) pins. It will return true if this is a valid option, and false if it is not (you don't need to check this, but it may be useful during development). If an invalid option is specified, or if the specified option does not exist, it will be set to the default one. If a mapping option is not available for a port on a specific pincount part

The next release, 1.4.0, will have a very different implementation of Serial.swap() as the current one will not extend to the number of optiosn that the DD-series will have, and there did not appear to be a way to salvage any of the original implementation. Initialize USART with a mux_mask with valid options as 1, and a function not associated in class-y ways with Serial will be used to handle the pin lookups (as well as any lookups user code might need in order to perform additional USART configuration). It is hoped and expected that this will be possible without any changes in behavior of this method..

* On AVR DD-series parts, where pins are scarce and competition for pins is intense, they added a ton of pin mapping options for Serial and SPI in particular. Serial0.swap() can take 0 (default), 1 (alt1), 2 (alt2), 3 (alt3), 4 (alt4), or 5 (none). Serial1.swap() "just" has 0 (default), 1 (alt1), 2 (alt2) and 3 (none)

`Serial.pins(TX pin, RX pin)` - this will set the mapping to whichever mapping has the specified pins as TX and RX. If this is not a valid mapping option, it will return false and set the mapping to the default. This uses more flash than Serial.swap(); that method is preferred.

When operating at 1MHz, the USARTs can run at 115200 baud! (note: prior to 1.3.3, Serial.flush() could hang the system if TCA0 or TCA1 was used as millis timing source - there was a 1 clock cycle window during which, if the millis ISR fired, if the byte finished transmitting before the ISR, flush() would hang - but only the type A timer millis ISR took longer to execute than a byte at 115.2kbaud took to transmit).

Do also note that at relatively high baud rates, Serial.print() may no longer use the buffer, as the relatively slow `print` class begins to take a similar amount of time as sending data down the wire (Serial.print - and all things serial adjacent to them, seemingly, are surprisingly slow); there is no set point where this switches, and it the buffer may be used for short stretches while sending a long string..

### SPI support
[SPI documentation](https://github.com/SpenceKonde/DxCore/blob/master/megaavr/libraries/SPI/README.md) A compatible SPI.h library is included; it provides one SPI master interface which can use either of the underlying SPI modules - they are treated as if they are pin mapping options.

### I2C (TWI) support
All of these parts have two hardware I2C (TWI) peripherals, except the 28-pin version, which has one. The included copy of the Wire library works almost exactly like the one on official Arduino boards, except that it does not activate the internal pullups unless they are specifically requested as described in the documentation linked below. The TWI pins can be swapped to an alternate location; this is configured using the Wire.swap() or Wire.pins() methods. Both of them achieve the same thing, but differ in how you specify the set of pins to use. This should be called **before** Wire.begin().

See **[Wire.h documentation](https://github.com/SpenceKonde/DxCore/blob/master/megaavr/libraries/Wire/README.md)** for full description and details.

Good news: support for dual mode and the second wire module will be in the release after 1.3.7, which will probably be 1.4.0, and a version will be checked into github for test as soon as that is released!


### swap() and pins() for Serial, SPI, and I2C
Like most recent parts, the Dx-series parts have multiple pin-mapping options for many of their peripherals, and the major serial interfaces of the Dx-series parts are not an exception. We provide the usual .swap() and .pins() methods whereby each instance of a UART, SPI interface, or I2C interface has a swap()


### PWM on LOTS of pins
The core provides hardware PWM (analogWrite) support. On all parts, 6 pins (by default, PD0-PD5 for 28 and 32-pin parts, PC0-PC6 for 48 and 64-pin parts) - see the part specific documentation pages for pin numbers) provide 8-bit PWM support from the Type A timer, `TCA0`. On 48-pin and 64-pin parts Dx-series parts, an additional 6 PWM pins are available on PB0-PB5 (by default) from `TCA1` (on EA-series parts with less than 48 pins, there is still a TCA1, but it can only do PWM on PA4, PA5, and PA6. Rempping support for `TCA1` kicks in on. Additionally, Type B timers not used for other purposes (TCB2 is used for millis unless another timer is selected, and other libraries mmay use a `TCB` as well) can each support 1 8-bit PWM pin. The pins available for this are shown on the pinout charts.

`TCA0` and `TCA1` will now detect the `PORTMUX.TCAROUTEA` register. As long as it is set to an option that allows 6 outputs in split mode (the two weird options for TCA1 aren't supported),  `analogWrite()` and `digitalWrite()` will work normally with it. See helper functions in DxCore.h library for some examples of this. We only offer support for this on`TCA1` on 64-pin parts - you can choose port B or G. This feature is not supported on the EA-series, since they never have 64 pins. It is also not supported on the 128DA64 until Microchip makes a new die rev that fixes it (it is broken in currently available silicon - though it works on the 64DA64, this will be fixed when they do a die revision). `TCA0` on the other hand can output on ANY PORT in this way, and is very flexible. Hence, . Since `TCA0 is far more useful`, we use `TCA1` to genrerate the prescaled clock for the `TCB`s if they are used for PWM.


If you want to take full control of one of the three pwm timers (maybe you want single mode), just call  `takeOverTCA0();` For the TCA's, it will also force hard reset, so they are returned to you in pristine condition. After this, analogWrite, `digitalWrite()` and `turnOffPWM()` will ignore anything these timers might be able to do on pins that those functions are called on. If taking over `TCD0`, and may the gods of silicon have mercy - it is one of the most fiendishly complicated contraptions on an AVR (not counting the older XMega line, where everything was like this). It's behavior is... often counterintuitive... but the featureset is incredible. I allowed into the core limited support for users making certain adjustments while still using `analogWrite()` because, well - because if I were to take my default position of "if you want to manually configure it, take it over"), nobody would do it, and that seems like a waste.. [Read more about DxCore and the TCD PWM functionality](https://github.com/SpenceKonde/DxCore/blob/master/megaavr/extras/Ref_TCD.md) But the basics are thus:


`TCD0` provides two PWM channels: `WOA` can output on PA4 or PA6, `WOB` on PA5, PA7. Those channels can each drive either - or both - of those pins, but only at one duty cycle. Users may prefer to configure this manually - `TCD0` is capable of, among other things, generating much higher frequency PWM, as it can be clocked from the PLL at 48MHz (or more, if you don't mind exceeding the specified operating ratings - I've gotten it up to 128 MHz, allowing 8-bit pwm at 500 kHz, or a 64 MHz squarewave). It is supposed to be remappable to other sets of pins. This doesn't work in current silicon, but future silicon revs will fix this - in all cases, the core uses channel A for the 2 even numbered pins, and channel B for the two odd numbered pins (if you take it over, the two higher nubmered ones can be set to either one, we only support one option through `analogWrite()`. If you try to turn a third PWM source, the other pin controlled by that channel will also change duty cycle - you can only get two proper PWM signals out. `digitalWrite()` the other one first, in the unlikely evenrt that you want to PWM different pins at different times in the program. (I've never wanted to do this, nor has anyone else I talked to), when I didn't also need them to output different durty signals at the same time, which is not possible.
```c++
analogWrite(PIN_PA4,128); // 50% PA4. - like usual
analogWrite(PIN_PA7,192); // 50% PA4, 75% PA7 - like usual
analogWrite(PIN_PA6, 64); // 25% PA5, 25% PA6, 75% PA7 - PA4 and PA6 are both channel A
//use digitalWrite to turn off the PWM on a pin
// or call turnOffPwm(pin);
```
**Note that TCA0, and TCA1 if present are configured by DxCore in Split Mode by default, which allows them to generate 8-bit PWM output on 6 pins each, instead of 16-bit pwm on three**; since the Arduino analogWrite() function we get takes values 0-255, this seems appropriate. See the [Taking over TCA0](https://github.com/SpenceKonde/DxCore/blob/master/megaavr/extras/TakingOverTCA0.md) guide for more information on reconfiguring this if you need 16-bit PWM.
**For general information on the available timers and how they are used PWM and other functions, consult the guide:**
#### [Timers and DxCore](https://github.com/SpenceKonde/DxCore/blob/master/megaavr/extras/Ref_Timers.md)

### EEPROM - Yes
A compatible `EEPROM.h` library is included; this implementation is derived from and fully compatible with the standard `EEPROM.h` api, even though the implementation differs internally.

### USERROW - No
Unlike the tinyAVR parts I support on megaTinyCore, there is not yet a `USERSIG` library to write to the "User Signature Space" (also called the `USERROW`, but the object that the library makes available can't be named that because the I/O headers already define that to point to the `USERROW` in memory in a more obtuse way), so it would conflict with any definition a library provided; hence why megaTinyCore has USERSIG.h). This is due largely to the need to make design decisions about how to expose it to the user. On megaTinyCore the USERROW can be erased with byte level granularity - like EEPROM - and an identical API to EEPROM is provided for USERROW. On the Dx-series, the only way to erase any one byte within USERROW is to erase the whole thing. While one can do read-modify-erase-write, the standard API would result in dramatic write amplification, as every single byte would result in a full erase and rewrite - even if those bytes were parts of a multi-byte datatype. Checking whether a cell was currently empty or held the same value being written would help greatly, but I would feel a lot better if there were an interface that "admitted" to the user that this was how it had to be, and put a simple wrapper around the "read to memory, modify buffer in RAM, then erase and rewrite EEPROM"

### Pin Interrupts
All pins can be used with `attachInterrupt()` and `detachInterrupt()`, on `RISING`, `FALLING`, `CHANGE`, or `LOW`. All pins can wake the chip from a sleep mode where the clock is turned off (power down and standby unless you have set something that needs it to run in standby) sleep on `CHANGE` or `LOW`. Pins marked as Async Interrupt pins on the pinout chart (this is marked by an arrow where they meet the chip on those charts - pins 2 and 6 on all ports have this feature) can be used to wake from sleep on `RISING` and `FALLING` edge as well. The async pins can also react to inputs shorter than one clock cycle (how *much* shorter was not specified) - this is both a blessing and a curse. If you need to detect super-short pulses, it's a blessing; but if you're not, those spikes are called "noise", and the async pins are more likely to be triggered by it.

For full information and example, but postentially dated information (`attachInterrupt()` should suck less now): [Pin Interrupts](https://github.com/SpenceKonde/DxCore/blob/master/megaavr/extras/Ref_Interrupts.md)

#### `attachInterrupt()` rework
There are three options, controlled by the Tools -> attachInterrupt Mode submenu: the new, enabled on all pins always (like the old one), manual (ports must be enabled before attaching to them), and old version (if the new implementation turns out to break something). Manual mode is required for the main benefit. In manual mode, you must call `attachPortAEnable()` (replace A with the letter of the port) before attaching the interrupt. The main point of this is that (in addition to saving an amount of flash that doesn't much matter on the Dx-series) `attachInterrupt()` on one pin (called by a library, say) will not glom onto every single port's pin interrupt vectors so you can't manually define any. The interrupts are still just as slow (it's inherrent to calling a function by pointer from an ISR - and low-numbered pins are faster to start executing than high numbered ones. The method to enable may change - I had hoped that I could detect which pins were used, but I couldn't get the function chose which ports to enable to not count as "referencing" those ports, and hence pull in the ISR. I am not happy with it, but "can't use any pin interrupts except through `attachInterrupt()` if using a library that uses `attachInterrupt()`" is significantly worse.

### On-chip Opamps
The DB-series parts have 2 (28 or 32 pin) or 3 (48/64 pin) on-chip opamps, with programmable resistor ladder, configurable for a variety of applications. They can be used as a voltage follower (you can follow the DAC and then use the output to drive VDDIO2, though the current is still only tens of mA, that's usually enough - driving heavy loads at the lower voltage is an unusual use case (I imagine powering low voltage sensors is not particularly rare - but those sort of modern sensors are also usually very low current).

We provide a basic wrapper in the form of the [Opamp Library](https://github.com/SpenceKonde/DxCore/blob/master/megaavr/libraries/Opamp) by MCUDude.

### Configurable Custom Logic
The CCL is exposed through the [Logic library](https://github.com/SpenceKonde/DxCore/blob/master/megaavr/libraries/Logic) by MCUDude. Number of logic blocks depends on series and pincount:
* 64/48 pin DA/DB have 6
* Everything else has 4

### Event System
The event system is exposed through the [Event library](https://github.com/SpenceKonde/DxCore/blob/master/megaavr/libraries/Event) by MCUDude. Number of channels depends on series and pincount:
* 8 channels on 28/32-pin DA/DB
* 10 on larger DA/DB
* 6 on everything else.

### Analog Comparators
The analog comparators are exposed through the [Comparator library](https://github.com/SpenceKonde/DxCore/blob/master/megaavr/libraries/Comparator) by MCUDude. Availability varies by pincount:
* 2 on 28 and 32 pin DA/DB
* 3 on 48/64 pin DA/DB
* 1 on all DD.
* 2 on all EA-series regardless of pincount.

### Zero-Crossing Detector
The ZCD(s) are exposed through the [ZCD library](https://github.com/SpenceKonde/DxCore/blob/master/megaavr/libraries/ZCD) by MCUDude. Availability depends on pincount:
* 2 on 28 and 32 pin DA/DB
* 3 on 48/64 pin DA/DB
* 1 on all DD
* The EA-series does not have any

### Timers
These parts for the most part are swimming in timers:
* TCA
  * 2 on 48/64 pin DA/DB and all EA-series
  * 1 elsewhere
* TCB
  * 5 on 64-pin parts
  * 4 on 48-pin parts and all EA-series
  * 3 on all 28/32-pin Dx-series
  * 2 on smaller parts
* TCD
  * 1 on all DA, DB, and DD parts
  * None on DU (presumably it's what they're using to generate the 48 MHz reference clock needed for standards compliant USB 2.0)
  * None on the EA-series


## Major core features

### Memory-mapped flash? It's complicated.
Unlike the tinyAVR 0/1-series and megaAVR 1-series parts, which are able to map their entire flash to memory, the DA-series parts can only map 32KB at a time. The FLMAP bits in NVMCTRL.CTRLB control this mapping. Unfortunately, because this can be changed at runtime, the linker can't automatically put constants into flash on 64k and 128k parts. However, on 32k parts, it can, and does!

As of 1.2.0, you can declare a variable `PROGMEM_MAPPED` (note: we had to change the name of this in 1.3.7 - we discovered "MAPPED_PROGMEM" is defined in some io headers and means something else there so our code broke things) ; this will put it in the final section of flash (section 1 or 3 - they're 0-indexed); in this case, the data is not copied to RAM, and *you can use the variable directly to access it through the mapped flash!*

See [PROGMEM and mapped flash reference](https://github.com/SpenceKonde/DxCore/blob/master/megaavr/extras/Ref_PROGMEM.md)

The `F()` macro works the same way as it does on normal boards as of 1.2.0, even on the 32k parts, where it is unnecessary to save RAM - this was done in order to maintain library compatibility; several very popular libraries rely on F() returning a `__FlashStringHelper *` and make use of `pgm_read_byte()` to read it.

### Writing to Flash from App
It is possible to write to the flash from the application code using the included Flash.h library. See the documentation for more information. Note that the API is completely different in every way from the Flash.h used on ATTinyCore and MegaCoreX. They were developed independently and reflect both the differences between the two NVM controllers and the ideals of the author of the libraries.
See the [Flash Library Documentation](https://github.com/SpenceKonde/DxCore/blob/master/megaavr/libraries/Flash/README.md)

### Servo Support
This core provides a version of the Servo library. This version of Servo always uses TCB0. If millis/micros is set to use TCB1 on those parts, servo will use TCB0 instead, making it incompatible with tone there as well). Servo output is better the higher the clock speed - when using servos, it is recommended to run at the highest frequency permitted by the operating voltage to minimize jitter.

**If you have also installed Servo** to your <sketchbook>/libraries folder (including via library manager), the IDE will use that version of the library (which is not compatible with these parts) instead of the one supplied with DxCore (which is). As a workaround, a duplicate of the Servo library is included with a different name - to use it, `#include <Servo_DxCore.h>` instead of `#include <Servo.h>` - all other code can remain unchanged.

### `printf()` support for "printable" class
Unlike the official board packages, but like many third party board packages, megaTinyCore includes the `printf()` method for the printable class (used for Serial and many other libraries that have `print()` methods); this works like `printf()`, except that it outputs to the device in question; for example:
```cpp
Serial.printf("Milliseconds since start: %ld\n", millis());
```
Note that using this method will pull in just as much bloat as `sprintf()`.
You can choose to have a full `printf()` implemenntation from a tools submenu if you want to print floating point numbers - floats by default are not enabled to save space (like normal arduino cores).

### Assembler Listing generation
Like my other cores, Sketch -> Export compiled binary will generate an assembly listing in the sketch folder. Starting in 1.3.3, a memory map is also created. These parts are nowhere near as flash constrained the other cores I maintain, but we all know what happens with programmers and memory - In the future I hope to provide a way make the output readable. The problem is that the first column it capped at 20 characters in width, and when symbols are longer, that row is misaligned. Sometimes they're much longer. A function that takes multiple arguments with long names can totally mess up the alignment. At some point I'd love to make an memory map cleaner...

### EESAVE configuration option
The EESAVE fuse can be controlled via the Tools -> Save EEPROM menu. If this is set to "EEPROM retained", when the board is erased during programming, the EEPROM will not be erased. If this is set to "EEPROM not retained", uploading a new sketch will clear out the EEPROM memory. Note that this only applies when programming via UPDI - programming through the bootloader never touches the EEPROM. You must do Burn Bootloader to apply this setting.

### Selectable printf implementation
A tools submenu lets you choose from full `printf()` with all features, the default one that drops float support to save 1k of flash, and the minimal one drops almost everything and for another 450 bytes (will be a big deal on the 16k and 8k parts. Less so on 128k ones.)

### BOD configuration options
These parts support multiple BOD trigger levels, with Disabled, Active, and Sampled operation options for when the chip is in Active and Sleep modes - Disabled uses no power, Active uses the most, and Sampled is in the middle. See the datasheet for details on power consumption and the meaning of these options. You must do Burn Bootloader to apply this setting, as this is not a "safe" setting: If it is set to a voltage higher than the voltage the board is running at, the chip cannot be reprogrammed until you apply a high enough voltage to exceed the BOD threshold.

### Link-time Optimization (LTO) support
This core *always* uses Link Time Optimization to reduce flash usage.


### NeoPixels (aka WS2812/SK6812 and many, many others)
Many of the usual NeoPixel (WS2812) libraries have problems on these parts. This core includes two libraries for controlling WS2812/SK6812/etc LEDs, both of which are tightly based on the Adafruit_NeoPixel library. See the [tinyNeoPixel documentation](https://github.com/SpenceKonde/DxCore/blob/master/megaavr/extras/tinyNeoPixel.md) and included examples for more information. Support is in the code for all clock speeds from 8 MHz up to 48 MHz. I suspect that it could be just barely made to work at 4 MHz by relaxing constraints - but I do not see much demand for such an undertaking. It just doesn't make sense to be driving WS2812's from low-clocked Dx-series. If you're driving a string of '2812s you're not worried about power consumption - a half dozen 2812's uses more power than the chip at full speed, and that's when the LEDs are off! the other reason to run at low frequencty is to operate at low voltage, but not only are the Dx parts rated for the full 24 MHz from 5.5V all the way down to 1.8V, at any voltage below 4V or so the blue LEDs don't work at full brightness anyway. So I decided that there was no reason to waste time porting the 'WS2812 driver to lower speeds.

### Tone Support
Support for `tone()` is provided on all parts using TCB0. This is like the standard `tone()` function on the Arduino megaavr board package. It does not support use of the hardware output compare to generate tones like tone on some parts does. (Note that if TCB0 is used for millis/micros timing - which is not a recommended configuration, we usually use the higher numbered TCB -  `tone()` will instead use TCB1).

**Recent Fixes** - Prior to the release of 1.3.3, there were a wide variety of bugs impacting the `tone()` function, particularly where the third argument, duration, was used; it could leave the pin `HIGH` after it finished, invalid pins and frequencies were handled with obvously unintended behavior, and integer math overflows could produce weird results with larger values of duration at high frequencies, and thee-argumwent `tone()` ddn't work at all above 32767Hz due to an integer overflow bug (even though maximum supported frequency is 65535 Hz).

**Long tones specifying duration** The three argument `tone()` counts the toggles of the pin for the duration. This count is kept in an unsigned long. To calculate it we multiply the arguments together and divide by 500 (s vs. ms supplies the factor of 1000, hz vs transitions the factor of 2). Division is painfully slow, so one generally tries to avoid or minimize it, and the base implementation simply multiplied them and divided by 500. If the product of the two overflows an unsigned long, the actual tone duration will overflow and come out much shorter. When "long" tones are supported, we now avoid having to calculate the product of frequency and duration by distributing the the division (at a slight cost to accuracy of the duration, and increase in flash use). As all of the DxCore parts have at least 16k of flash, "long" tones are supported on all parts, a future DxCore-supported part with less that 16k of flash would not support long tones, and these tones may not work if code is ported to other parts. A "long" tone is one where `(frequency * duration) > 4.294 billion` (if less than that it's a "short" tone) but less than`(frequency * duration) / 500 < 4.294 billion` (we rearrange to `(frequency/5) * (duration/100)` when duration > 65535).  Anything longer is a "very long" tone and is never supported via the three argument `tone()` - this means a high frequency tone output for a duration of hours, which is far outside the regime that tone is intended for use in. Instead, and for portable code that works without needing "long" tones, turn the tone on and off with the 2-argument `tone()` and use something else to time it.

### millis/micros Timekeeping Options
DxCore provides the option to us any available timer on a part for the `millis()`/micros timekeeping, controlled by a Tools submenu - (except, currently, TCD0 - implementations are available, but there are more options available here than on the tinyAVR 1-series, making it's use more complicated) - or it can be disabled entirely to save flash, eliminate the presence of frequent millis interrupts, and allow full use of the timers. By default, TCB2 will be used, except for DD-series parts without the third timer. Those will default instead to TCB1. TCA0, TCA1 (if present) and any of the TCB's present on the part may be used. TCD support will be added after DD-series relkease with limits - it will never be the default because TCD is far more powerful here since we get a PLL to drive it.

For more information, on the hardware timers of the supported parts, and how they are used by DxCore's built-in functionality, see the [Timers and DxCore](https://github.com/SpenceKonde/DxCore/blob/master/megaavr/extras/Ref_Timers.md)

### Improved digital I/O
This core adds a number of new features include fast digital I/O (1-14 clocks depending on what's known at compile time, and 2-28 bytes of flash, and for configuring all per-pin settings the hardware has with `pinConfigure()`

See the [Improved Digital I/O Reference](https://github.com/SpenceKonde/DxCore/blob/master/megaavr/extras/Ref_Digital.md)

## List of Tools sub-menus
* Tools -> Chip - sets the specific part within a selected family to compile for and upload to.
* Tools -> Clock Speed - sets the clock speed. You do not need to burn bootloader after changing this setting!
* Tools -> Retain EEPROM - determines whether to save EEPROM when uploading a new sketch. This option is not available on Optiboot board definitions - programming through the bootloader does not execute a chip erase function and never erases the bootloader.  ~You must burn bootloader after changing this to apply the changes~ As of 1.3.0, this setting is applied on all UPDI uploads without a "burn bootloader" cycle to AVR DA and AVR DB-series devices.
* Tools -> B.O.D. Voltage - If Brown Out Detection is enabled, when Vcc falls below this voltage, the chip will be held in reset. You must burn bootloader after changing this to apply the changes. Take care that these threshold voltages are not exact - they may vary by as much as +/- 0.3v! (depending on threshold level - see electrical characteristics section of datasheet). Be sure that you do not select a BOD threshold voltage that could be triggered during programming, as this can prevent successful programming via UPDI (reported in #86).
* Tools -> Reset/UPDI - This menu option can be set to Reset (default) or Input; the latter allows this pin to be used as a normal input. DD-series have extra options to configure the UPDI pin as well, and on these parts, This setting is applied to DA and DB series on all UPDI uploads without a "burn bootloader" cycle. It is not set on DD-series parts - the UPDI disable option makes this fuse "unsafe" to reconfigure.
* Tools -> B.O.D. Mode (active/sleeping) - Determines whether to enable Brown Out Detection when the chip is not sleeping, and while it is. Only combinations where active is at least as aggressive as sleep mode are shown, as those are the only sensible operating modees.=. You must burn bootloader after changing this to apply the changes.
* Tools -> `millis()`/`micros()` - If set to enable (default), `millis()`, `micros()` and `pulseInLong()` will be available. If set to disable, these will not be available, Serial methods which take a timeout as an argument will not have an accurate timeout (though the actual time will be proportional to the timeout supplied); delay will still work, though it's done using `delayMicroseconds()`, so interrupts are disabled for 1ms at a time during the delay, and any interrupts that happen during the delay will add to the length of the delay. Depending on the part, options to force millis/micros onto any type A or B timer on the chip are also available from this menu.
* Tools -> MVIO - MVIO option is back in 1.3.7. It is not a risk of hardware damage if it is turned off, and it saves 0.5 uA power consumption to disable it. Disabling it when you shouldn't doesn't keep the pins from being readable and writable, nor does it short the VDDIO pin to VDD.... it just no longer watches the voltage to ensure sane behavior if insufficient voltage is applied on VDDIO2. This is in effect an extra layer of monitoring like the BOD is,
* Tools -> Write flash from App - Either disabled (Flash.h library does not work), "Everywhere" (allow writes everywhere in the flash after first page), or allow writes only above a certain address. On Optiboot definirtions, it's always enabled for writes anywhere.
* Tools -> `printf()` imoplementation - The default option can be swapped for a lighter weight version that omits most functionality to save a tiny amount of flash, or for a full implementation (which allows printing floats with it) at the cost of about 1k extra. Note that if non-default options are selected, the implementation is always linked in, and will take space even if not called. Normal Arduino boards are set to default. They also don't have `Serial.printf()`
* Tools -> attachInterrupt Mode - Choose from 3 options - the new, enabled on all pins always (like the old one), Manual. or the old implementation in case of regressions in the new implementation. When in Manual mode, You must call `attachPortAEnable()` (replace A with the letter of the port) before attaching the interrupt. The main point of this is that (in addition to saving an amount of flash that doesn't much matter on the Dx-series) `attachInterrupt()` on one pin (called by a library, say) will not glom onto every single port's pin interrupt vectors so you can't manually define any. The interrupts are still just as slow (it's inherrent to calling a function by pointer from an ISR - and low-numbered pins are faster to start executing than high numbered ones. The method to enable may change - I had hoped that I could detect which pins were used, but I couldn't get the function chose which ports to enable to not count as "referencing" those ports, and hence pull in the ISR. I am not happy with it, but "can't use any pin interrupts except through `attachInterrupt()` if using a library that uses `attachInterrupt()`" is significantly worse.

## Additional DxCore documentation
### Reference
#### [Function Reference](https://github.com/SpenceKonde/DxCore/blob/master/megaavr/extras/Ref_Functions.md)
Covering top-level functions and macros that are non-standard, or are standard but poorly documented.
#### Constant Reference coming eventually.
#### [Analog Input (ADC) and output (DAC)](https://github.com/SpenceKonde/DxCore/blob/master/megaavr/extras/Ref_Analog.md)
The API reference for the analog-related functionality that is included in this core beyond the standard Arduino API.
#### [Digital I/O and enhanced options](https://github.com/SpenceKonde/DxCore/blob/master/megaavr/extras/Ref_Digital.md)
The API reference for the digital I/O-related functionality that is included in this core beyond the standard Arduino API, as well as a few digital I/O related features that exist in the hardware which we provide no wrapper around.
#### [Interrupts](https://github.com/SpenceKonde/DxCore/blob/master/megaavr/extras/Ref_Interrupts.md)
Includes a list of all interrupt vectors that can be used, how the flags are cleared (not a substitute for the datasheet - just a very quick reminder), which parts each vector exists on, and and what parts of the core, if any, make use of a vector. It also has general guidance and warnings relating to interrupts their handling, including estimates of real-world interrupt response times.
#### [Timers and PWM](https://github.com/SpenceKonde/DxCore/blob/master/megaavr/extras/Ref_Timers.md)
We configure the timers in specific ways upon startup, which determines the frequency of PWM output, and some parameters of `millis()` timekeeping.
#### [TCD0](https://github.com/SpenceKonde/DxCore/blob/master/megaavr/extras/Ref_TCD.md)
The type D timer is a powerful timer, but has quirks which one must be aware of if using it. This describes what you can do without having to take full control of the timer.
#### [Mapped flash and PROGMEM in DxCore](https://github.com/SpenceKonde/DxCore/blob/master/megaavr/extras/Ref_PROGMEM.md)
There are two ways to access constants stored in flash on DxCore. Which ones can read data stored where can be confusing; this document should make this clear.
#### [Optiboot Bootloader](https://github.com/SpenceKonde/DxCore/blob/master/megaavr/extras/Ref_Optiboot.md)
An Optiboot-derived bootloader is provided and may be optionally used. How that impacts operations is described here. This covers relevant considerations for deciding whether to use it as well.
#### [SerialUPDI](https://github.com/SpenceKonde/DxCore/blob/master/megaavr/extras/Ref_SerialUPDI.md)
Serial UPDI is our recommended tool for UPDI programming.
#### [Clock Information](https://github.com/SpenceKonde/DxCore/blob/master/megaavr/extras/Ref_Clocks.md)
Supported clock sources and considerations for the use thereof.
#### [Callbacks/weakly defined functions](https://github.com/SpenceKonde/DxCore/blob/master/megaavr/extras/Ref_CallBacks.md)
These are provided by the core and can be overridden with code to run in the event of certain conditions, or at certain times in the startup process.
#### [Identification of core features programmatically](https://github.com/SpenceKonde/DxCore/blob/master/megaavr/extras/Ref_Defines.md)
These are used by megaTinyCore and other cores I maintain as well.
#### [Reset control and the WDT](https://github.com/SpenceKonde/DxCore/blob/master/megaavr/extras/Ref_Reset.md)
The sources of reset, and how to handle reset cause flags to ensure clean resets and proper functioning in adcverse events. **Must read for production systems**
#### [Considerations for robust applications](https://github.com/SpenceKonde/DxCore/blob/master/megaavr/extras/Ref_Robust.md)
Covers a variety of design considerations for making something that will opperate reliably in the field, some specific to DxCore, others general. Lately I have been seeing a lot of projects get too far along without considering these. **Must read for production systems**
### Library documentation
See the [library index](https://github.com/SpenceKonde/DxCore/blob/master/megaavr/extras/Libraries.md) or readme files for each library (the former is mostly composed of links to the latter)
### Older guides inherited from megaTinyCore.
These guides may not account for all of differences between DxCore and megaTinyCore, and may not reflect recent changes.
#### [Power Saving techniques and Sleep](megaavr/extras/PowerSave.md)
There are plans for a better wrapper around this sort of functionality, which keep getting deferred as more pressing issues come up.
#### [Direct Port Manipulation](megaavr/extras/DirectPortManipulation.md)
It's similar to classic AVRs, but a bit more complicated. See also digital I/O reference
#### [Pin Interrupts](megaavr/extras/Ref_Interrupts.md)
Manually defining pin interrupts, because attachInterrupt results in interrupts which respond slowly. See also Interrupt Reference

## Support Continued Development
I sell breakout boards with regulator, UPDI header, and Serial header and other basic supporting parts in my Tindie shop, as well as the bare boards. Buying from my store helps support further development on the core, and is a great way to get started using these exciting new parts with Arduino. Note that we do not currently sell a 28-pin version - this did not seem like a compelling part with the availability of the 32-pin version; the main appeal of the 28-pin part is that it is available in a through-hole version. As we would not be able to make the 28-pin version significantly smaller, there did not seem to be a compelling reason to create a 28-pin version. We may revisit this decision in the future, including potentially a 28-pin bare board for the through-hole version, suitable for assembly by those not experienced with drag soldering.
### [Assembled Boards](https://www.tindie.com/products/21007/)
### Assembled AVR128DB64 Boards - Coming Soon
### [Bare Boards](https://www.tindie.com/products/21008/)
### "Azduino Nano DB" inspired by the shape of a Nano, but with a more coherent pinout - Coming Soon
### DIY 2" x 2" DA-series or DB/DD-series prortotyping boards - Coming Soon


## Warnings and Caveats

### Direct Register Manipulation
If you are manually manipulating registers controlling a peripheral, you should not count on the behavior of API functions that relate to the same peripheral, nor should you assume that calling said API functions will be harmless either. For example, if you reconfigure TCA0, you should not expect that using `analogWrite()` will work correctly on TCA0-based pins (for example, if it's no longer in split mode, analogWrite, might overwrite one half of the compare value. If you have reconfigured the ADC for free running mode, `analogRead()` will return errors, and so on. There are exceptions to this of course - Timer/counter prescalers just change the frequency of the PWM, and there are several other exceptions noted in the above reference documents.

### Timer PWM and exceptions
In the special case of TCA0, TCA1, and TCD0, a special function called `takeOverTCAn()` (or `takeOverTCD0()`) is provided - calling this will tell the core that you are assuming full responsibility for everything related to that timer. analogWrite on pins it is pointed at will not turn on PWM, nor will digitalWrite turn it off. This function will not be available for any timer used to control millis timekeeping (and manually reconfiguring such a timer should be expected to break timekeeping. Note that if you are using PWM on a pin provided by a type B timer (not recommended, they're lousy at it) they depend on the prescaler settings of a type A timer (one of the reasons they're lousy PWM timers), by default we use TCA0's prescaled clock for the TCBs, so taking over TCA1, if present, will not impact PWM generated by TCBs unless you have reconfigured them to use TCA1. We do not change that option after at any point after user code has started running, so you can move it to use TCA1 and take over TCA0 and keep using `analogWrite()` on the TCB-controlled pins. In order to help make manual timer configuration as easy as possible, we also tell type A timers to do a hard reset, which sets them back to power on reset configuration.

Becausue TCD0 does not have that option, you will start from our initial configuration. We'll stop changing it, but we can't reset it to power on state. That timer is so wacky and strange and complicated, realistically you're going to be dumping the values of all the registers to examine throughout the development process anyway. Outside of the XMega parts (where almost everything is a nightmarish maze) this is possibly the most complicated peripheral in the AVR line. Certainly within ones that people use with Arduino, it is. Read the part about synchronization in the datasheet several times.

### I2C **requires** external pullup resistors
Earlier versions of megaTinyCore, and possibly very early versions of DxCore enabled the internal pullup resistors on the I2C pins. This is no longer done automatically - they are not strong enough to meet the I2C specifications, and it is preferable for it to fail consistently without external ones than to work under simple conditions with the internal ones, yet fail under more demanding ones (more devices, longer wires, etc). However, as a testing aid, we supply Wire.`usePullups()` to turn on the weak internal pullups. If `usePullups()` ever fixes anything, you should install external pullups straight away. Our position is that whenever external pullups are not present, I2C is not expected to work. Remember that many modules include their own on-board pullups, frequently 10k.

## Differences in Behavior between DxCore and Official Cores
While we generally make an effort to emulate the official Arduino core, there are a few cases where the decision was made to have different behavior to avoid compromising the overall functionality; the official core is disappointing on many levels. The following is a (hopefully nearly complete) list of these cases.

### Serial does not manipulate interrupt priority
The official core for the (similar) megaAVR 0-series parts, which this was based on, fiddles with the interrupt priority (bet you didn't know that!) in ways that are of dubious safety towards other code. megaTinyCore does not do this (in the process saving several hundred bytes of flash). Writing to Serial when it's buffer is full, or calling `Serial.flush()` while  with interrupts disabled, or during another ISR (which you really shouldn't do anyway) will behave as it does on classic AVRs, and simply block until there is either space in the serial buffer, or the flush is completed. This will probably never be a problem, and I think removing that fixed several bugs.

### `digitalRead()` does not turn off PWM
On official cores, and most third party ones, the `digitalRead()` function turns off PWM when called on a pin. This behavior is not documented by the Arduino reference. This interferes with certain optimizations and moreover is logically inconsistent - a "read" operation should not change the thing it's called on. That's why it's called "read" and not "write". There does not seem to be a logical reason to do this, either. while it's rarely useful per se, it does let you demonstrate what PWM is just by reading the pin a bunch of times in succession.

### `digitalWrite()`/`pinMode()` and INPUT pins
Like the official "megaavr" core, calling `digitalWrite()` on a pin currently set INPUT will enable or disable the pullups as appropriate. Recent version of DxCore fix two gaps in this "classic emulation". On a classic core, `digitalWrite()` on an INPUT would also write to the port output register - thus, if one subsequently called `pinMode(pin, OUTPUT)`, the pin would immediately output that level. This behavior is not emulated in the official core, and there is a considerable amount of code in the wild which depends on it. `digitalWrite()` now replicates that behavior. `digitalWrite()` also supports `CHANGE` as an option; on the official core, this will turn the pullup on, regardless of which state the pin was previously in, instead of toggling the state of it. The state of the pullup is now set to match the value that the port output register was just set to.

Similarly, using `pinMode()` to set a pin to `INPUT` or `INPUT_PULLUP` will now also set the port output register. **Do not call pinmode on a pair of I2C pins that is being used for that purpose, ever** Wire.`begin()` and optionally `usePullups()` will do that for you, while `pinMode()` will break them.

### `analogWrite()` and TCD0 pins
Please see the above PWM feature description if using PWM on those pins and also doing sensitive/niche work on the impacted pins (PIN_PA6 and PIN_PA7).

### TCA(s) are configured in Split Mode to get 3 additional PWM pins
On official "megaavr" board package, TCA0 is configured for "Single mode" as a three-channel 16-bit timer (used to output 8-bit PWM anyway). DxCore always configures Type A timers for "split mode", which turns it into 6 independent 8-bit channels. See the datasheets for more information on the capabilities of these peripherals. See [Taking over TCA0](megaavr/extras/TakingOverTCA0.md) for information on reconfiguring it.

### TCA0/1 and all TCB's used for PWM have TOP at 254, not 255
0 is a count, so at 255, there are 256 steps, and 255 of those will generate PWM output - but since Arduino defines 0 as always off and 255 as always on, there are only 254 possible values that it will use. The result of this is that (I don't remember which) either `analogWrite(pin,254)` results in it being LOW 2/256's of the time, or `analogWrite(pin,1)` results in it being `HIGH` 2/256's of the time. On DxCore, with 255 steps, 254 of which generate PWM, the hardware is configured to match the API. If you make a graph of measured duty cycle vs the value passed to analogWrite, it is a straight line with no kink or discontinuity and an intercept at 0. In the event that TCA0 is used for millis, as it happens, 255 also (mathematically) works out such that integer math gets exact results for millis timing with both 16 MHz derived and 20 MHz derived clock speeds (relevant when TCA0 is used for `millis()` timing). I have not attempted this math for other system clock speeds. The same thing is done for TCD0 (though at higher clock speeds, we multiply the input and TOP by a power of 2 - `analogWrite()` accounts for this - so that we can get the same output frequency as an 8-bit timer would at the same unprescaled clock speed (higher frequencies) while keeping the fastest synchronization prescaler for fastest synchronization between TCD0 and system clock domains).

### digital I/O functions use old function signatures.
They return and expect uint8_t (byte) values, not enums like the official megaavr board package does. Like classic AVR cores, constants like `LOW`, `HIGH`, etc are simply #defined to appropriate values. The use of enums instead broke many common Arduino programming idioms and existing code, increased flash usage, lowered performance, and made optimization more challenging. While the enum implementation made language design purists comfortable, and provided error checking for newbies - because you couldn't pass anything that wasn't a PinState to a digital I/O function, and would get that error checking if - as a newbie - you accidentally got careless. Many common practices were tripped up by this, like `if(digitalRead(pin))`, and so a compatibility layer was added to the official core - but then that got rid of what was probably the most compelling benefit, the fact that it did generate an error for new users if they passed bogus values. When all was said and done, instead of helping to shine a light on errors, it turned out have the opposite effect. When I discovered this on megaTinyCore was when wrestling with a pinMode call that seemed to do nothing... I was astonished to find that `PinMode(pin,OUTPUT)`, an obvious typo of `pinMode(pin,OUTPUT)` was valid syntax (comma operator turns pin,OUTPUT into OUTPUT, and it returns a new PinMode of value OUTPUT...), instead of a syntax error! This turned a 30-second bug into a 2-3 hour session of frustration. Anyway - the enums are not present here, and they never will be; this is the case with [MegaCoreX](https://github.com/MCUdude/MegaCoreX) and [megaTinyCore](https://github.com/SpenceKonde/megaTinyCore) as well.

### `analogReadResolution()` is different
Official AVR boards do not have analogReadResolution. Official ARM-based boards do, but the implementation on those boards, awash in memory and flash, is very different - they allow specifying any number from 1 to 32, and will shift the reading as required (padding it with zeros if a resolution higher than the hardware is capable of is specified). I dislike conceptually the idea of the core presenting values as if it has more recision than it does, and in any event, on these resource constrained 8-bit microcontrollers, the code to create rounded or padded numbers with 1-32 bits is an extravagance we cannot afford, and the overhead of that should not be imposed on the vast majority of users who just want to read at the maximum resolution the hardware suopports, or 10 bits for code compatibility with classic AVRs. Since `analogReadResolution()` accepts a wide range of values on the official boards that have it, it does not need to report success or failure.

### As of 1.3.3, SerialEvent is removed
SerialEvent was an ill-conceived mess. I knew that when I added support for it, but I didn't know that the mess had already been deprecated; when I heard that it was, I wasted no time in fully removing it.

## Instruction Set (AVRe/AVRe+ vs AVRxt)
The classic AVR devices all use the venerable `AVRe` (ATtiny) or `AVRe+` (ATmega) instruction set (`AVRe+` differs from `AVRe` in that it has hardware multiplication and supports devices with more than 64k of flash). Modern AVR devices (with the exception of ones with minuscule flash and memory, such as the ATtiny10, which use reduced core `AVRrc`) use the latest iteration of the AVR instruction set: `AVRxt`. This adds no new instructions (unlike `AVRxm`, the version used by the XMega devices, which adds 4 combined read-write instructions for accessing SRAM only (which I think doesn't include peripheral registers, which takes out all the use cases for it that I can theink of...), but a small number of instructions have improved execution time, and has slower execution time. This distinction is unimportant for 99.9% of users - but if you happen to be working with hand-tuned assembly (or are using a library that does so, and are wondering why the timing is messed up), the changes are:
* PUSH is 1 cycle vs 2 on classic AVR (POP is still 2) (this is the biggest change in terms of overall speed - It is not unusual for 25% or more of a program to be push and pop instructions!)
* CBI and SBI are 1 cycle vs 2 on classic AVR (Sweeet! very noticeable when we use it, even though it's a relatively rare instruction.)
* LDS is 3 cycles vs 2 on classic AVR :disappointed: LD and LDD are still two cycle instructions.
* RCALL and ICALL are 2 cycles vs 3 on classic AVR
* CALL is 3 cycles instead of 4 on classic AVR
* Best one for last: ST and STD is 1 cycle vs 2 on classic AVR! (STS is still 2). This includes std as well, which makes this nearly as large as the improvement to push.
* When ld, ldd, or lds is used to load data from flash using the flash mapping... it takes an extra clock for the NVM access (as far as I can tell; the location in the datasheet where theinstruction set manual says more information can be found has no such information.)

PUSH and POP are basically like ST X+ and LD -X with a 4th pointer register pair, except the stack pointer isn't made up of working registers and there aren't any other options other than postincrement and predecrement - so these changes are largely the same, only the instructions are called something else (the opcodes even look almost the same. The improvement to PUSH can make interrupts respond significantly faster (since they have to push the contents of registers onto the stack at the beginning of the ISR to make room for any that the ISR might need.), though the corresponding POP's at the end aren't any faster, and if your interrupt needs so many registers that it has to push half of the register file onto the stack, it's still gonna be slow (ex: Servo).  You may notice that there is a pattern here: writing and PUSHing lost a clock cycle. It is likely that the improvements to the push-ing mechanism are responsible for the improvements to the call instructions (which push the PC value onto the stack so ret knows where execution should resume from.) The change with ST impacted tinyNeoPixel. Prior to my realizing this, the library worked on SK6812 LEDs (which happened to be what I tested with) at 16/20 MHz, but not real WS2812's. However, once I discovered this, I was able to leverage it to use a single tinyNeoPixel library instead of a different one for each port like was needed with ATTinyCore (for 8 MHz, they need to use the single cycle OUT on classic AVRs to meet timing requirements, the two cycle ST was just too slow; hence the port had to be known at compile time, or there must be one copy of the routine for each port, an extravagance that the ATtiny parts cannot afford. But with single cycle ST, that issue vanished)

## License
DxCore itself is released under the [LGPL 2.1](LICENSE.md). It may be used, modified, and distributed, and it may be used as part of an application which, itself, is not open source (though any modifications to these libraries must be released under the LGPL as well). Unlike LGPLv3, if this is used in a commercial product, you are not required to provide means for user to update it.

The DxCore hardware package (and by extension this repository) contains DxCore as well as libraries, bootloaders, and tools. These are released under the same license, *unless specified otherwise*. For example, tinyNeoPixel and tinyNeoPixel_Static, being based on Adafruit's library, are released under GPLv3, as described in the LICENSE.md in those subfolders and within the body of the library files themselves.

The pyupdi-style serial uploader in megaavr/tools is a substantially renovated version of pymcuprog from Microchip, which ~is not open source~ *has now been released under the open source MIT license!*.

Any third party tools or libraries installed on behalf of DxCore when installed via board manager (including but not limited to, for example, avr-gcc and avrdude) are covered by different licenses as described in their respective license files.
