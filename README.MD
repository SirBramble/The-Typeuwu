# The Typeuwu

The Typeuwu is a modular keyboard design that can be extended with magnetic modules. It also provides a connector for a optional Bluetooth and Battery module.\
The firmware is written in C++ and provides a recompile free key remapping with the AniMacro scripting language.

![Keyboard](04%20Pictures/5.jpeg)
![Keyboard](04%20Pictures/The%20Typeuwu%20PCB%20back.png)

## Hardware

The Typeuwu uses a RP2040 as the core processor, that polls the keyboard matrix, controls the LEDs and handles the communication between The keyboard and PC.

It also connects to the modules via magnetic pogo pin connectors located in the centre of each side. These provide the modules with power and expose the 5V level-shifted I2C lines.

Schematics can be found in ```/01 PCB/01 The Typeuwu PCB```.\
A PDF is available [```here```](01%20PCB/01%20The%20Typeuwu%20PCB/schematics/The%20Typeuwu.pdf).

## Firmware

The keyboard uses the custom written UWU (Universal Wordtyping Unit) Firmware that provides all needed interfaces for communicating with the Host and other modules.\
It also provides a parser for the 'AniMacro' scripting language, allowing the user to customise the key functionality from a config file located on the keyboards flash drive, without having to recompile the firmware.\

> [!IMPORTANT]  
> The firmware version included in this repository is deprecated. Though functional, further updates to the firmware will be posted to the [```UWU-Firmware```](https://github.com/SirBramble/UWU-Firmware) repository. Precompiled binary files are also available there.

### Uploading firmware

#### Precompiled firmware

To upload the precompiled binary, set the RP2040 into bootloader mode. This is done by first pressing the ```boot``` button, followed by either pressing the ```reset```button or performing a power cycle. The RP2040 should then mount as a flash drive named ```UF2```.\
Then either upload the Code using PIO or the Arduino IDE or copy the ```firmware.uf2``` file to the flash drive.

#### Compiling PIO

1. Make sure you have VSCode with the PlatformIO extension installed.
2. Clone the [```UWU-Firmware```](https://github.com/SirBramble/UWU-Firmware) repository (here: [github.com/SirBramble/UWU-Firmware](github.com/SirBramble/UWU-Firmware)).
3. Open the folder with PlatformIO.
   - PlatformIO should recognise it as a project and allow you to compile using either the VSCode interface or with the command ```platformio run```.
   - See [PlatformIO run documentation](https://docs.platformio.org/en/latest/core/userguide/cmd_run.html) for more information and possible arguments.
   - Any required library can either be added in the ```platformio.ini``` or copied to the ```lib``` (lib.h) and ```src``` (lib.c or lib.cpp) directory.

#### Compiling Arduino (deprecated)

1. Download the latest version of the Arduino IDE.
2. Install the [```arduino-pico```](https://github.com/earlephilhower/arduino-pico) board library.
    - You will also have to install the following librarys:
      - Adafruit NeoPixel (version: 1.12.0)
      - Adafruit SPIFlash (version: 4.3.1)
      - MIDI Library (version: 5.0.2)
3. Download the ```Typeuwu_Firmware``` folder.
4. Open the ```Typeuwu_Firmware.ino``` file with the Arduino IDE and either compile or upload directly.

## AniMacro scripting language

The 'AniMacro' scripting language allows the user to configure any Hardware Interface running the 'UWU firmware'.\
Configurations written in AniMacro are automatically loaded upon start-up and subsequently parsed using regular expressions. Additionally, the firmware can detect changes in the configuration file, facilitating its reload as needed, thereby enabling dynamic configuration adjustments at runtime.

A single configuration file accommodates configurations for various hardware UWU modules. The firmware selectively parses configurations pertinent to the currently connected hardware modules. This design choice allows for the seamless connection of multiple modules during runtime.

### Syntax

The 'AniMacro' syntax is based loosely on LaTeX syntax. A configuration for a specific module is wrapped in a ```begin``` and ```end``` qualifier:

```c
begin{module}
...
end{module}
```

Within a module configuration, multiple layers can be incorporated. Layers are a mechanism to implement diverse button configurations, facilitating dynamic switching between them at runtime. Layers can be added by typing ```Layer``` followed by the number of the layer:

```c
begin{module}
Layer x
...
end{module}
```

> [!IMPORTANT]
>By default the Firmware starts on layer 1. In the absence of a configured layer 1, the firmware remains inactive. It is advisable to consistently include a layer 1 configuration to ensure proper functionality.

All devices responsible for executing actions within a layer are designated using the ```Button``` keyword. Whether these devices are faders, potentiometers, or conventional buttons, they are uniformly referenced with the ```Button``` keyword.\
Buttons are implemented using the 'Button' keyword, followed by the button number, a colon, and a space. Subsequently, functionality can be added to the button by incorporating commands within the same line as the 'Button' keyword:

```c
begin{module}
Layer x
    Button a: commands
    Button b: commands
...
end{module}
```

Commands are categorized into several groups, which can be combined within a button configuration:

- Plain text:\
Entering plain text (e.g., "this is text123") utilizes the keyboard HID interface to type out the provided text.
    > [!NOTE]
    > Only ASCII characters are accepted in plain text. Non-ASCII characters (e.g. ```ä```) are managed with separate commands.
- ```\COMMAND``` with no arguments:\
    These commands serve as non-configurable placeholders for specific actions. For instance, pressing the 'Enter' key on the keyboard (```\ENTER_KEY```) or adding umlauts (```\ae``` => ```ä```).
- ```\COMMAND{}``` with arguments:\
    These commands are commonly employed to configure actions and callbacks within the firmware.\
    For example: ```\COLOR{const,green}``` would set the button to a constant green colour.\
    ```\MIDI_CC_KEY{16,1}\COLOR{midi,white}``` would configure a MIDI callback, binding the button to MIDI CC number 16 on MIDI channel 1. When the button is triggered, a MIDI message is dispatched to the host (PC), initiating an action. Subsequently, the host can relay back the state of the triggered action. The response from the host is then linked to the button colour, with a response greater than 126 denoting a triggered colour override, and a response of less than 126 signifying a reversion back to the layer default.

### List of commands

#### AniMacro commands

| Command | Description | Note |
| --- | --- | --- |
| Plain text | Types out the given text via the HID Interface | Only works with ASCII characters|
| ```\ae``` ```\oe``` ```\ue``` | Types out ```Umlauts``` (ä,ö,ü) | |
| ```\SZ_KEY``` | Sends keycode of ```ß``` key | |
| ```\GRAVE_KEY``` | Sends keycode of ```backtick``` ('´') key | |
| ```\CAPS_LOCK_KEY``` | Sends keycode of ```caps lock``` key | |
| ```\LSHIFT_KEY``` | Sends keycode of ```left shift``` key | |
| ```\RSHIFT_KEY``` | Sends keycode of ```right shift``` key | |
| ```\LCTRL_KEY``` | Sends keycode of ```left control``` key | For control combinations (e.g. ```CTRL``` + ```C```) refer to ```\STRG{char}```|
| ```\RCTRL_KEY``` | Sends keycode of ```right control``` key | For control combinations (e.g. ```CTRL``` + ```C```) refer to ```\STRG{char}```|
| ```\LATL_KEY``` | Sends keycode of ```left alt``` key | |
| ```\RATL_KEY```| Sends keycode of ```right alt``` key | |
| ```\WIN_KEY``` | Sends keycode of ```windows``` key | |
| ```\APP_KEY``` | Sends keycode of ```app``` key | |
| ```\RMETA_KEY``` | Sends keycode of ```right meta``` key | |
| ```\SPACE_KEY``` | Sends keycode of ```space bar``` key | Equivalent to typing space character in plain text |
| ```\BACKSPACE_KEY``` | Sends keycode of ```backspace``` key | |
| ```\DEL_KEY``` | Sends keycode of ```delete``` key | |
| ```ENTER_KEY``` | Sends keycode of ```enter``` key | |
| ```ESC_KEY``` | Sends keycode of ```escape``` key | |
| ```Tab_KEY``` | Sends keycode of ```tab``` key | |
| ```F[x]_KEY``` | Sends keycode of ```F[x]``` key | ```[x]``` to be replaced with number from ```1-12```|
| ```[arrow]_KEY``` | Sends keycode of ```[arrow]``` key | [arrow] to be replaced with one of: ```UP```, ```DOWN```, ```LEFT```, ```RIGHT```|
| ```[arrow]_KEY{[x]}```| Sends keycode of ```[arrow]``` key ```[x]``` times | ```[x]``` to be replaced with number and ```[arrow]``` to be replaced with one of: ```LEFT, RIGHT``` |
| ```\STRG{char}``` | Sends control combinations | Example: ```\STRG{c}``` => ```CTRL``` + ```C```|
| ```\MIDI_CC{[CT],[CH]}``` | Sends MIDI CC with number ```[CT]``` on MIDI channel ```[CH]``` with value from button | Replace ```[CT]``` with number from 0 to 127 and ```[CH]``` with number from 1-15. The button has to have a value or be a ```fader```/```potentiometer```/```encoder```.|
| ```\MIDI_CC_KEY{[CT],[CH]}``` | Sends MIDI CC with number ```[CT]``` on MIDI channel ```[CH]``` with value 127 | Replace ```[CT]``` with number from ```0``` to ```127``` and ```[CH]``` with number from ```1-15```. Used to trigger button events in software|
| ```\COLOR{[M],[C]}``` | Sets colour mode of button to ```[M]``` with colour ```[C]``` | Refer to ```colour mode list``` for valid mode ```[M]``` and ```colour list``` for valid colour ```[C]```|
| ```\COLOR{[M],[R],[G],[B]}``` | Sets colour mode of button to ```[M]``` with colour comprised of ```[R]```,```[G]``` and ```[B]``` | Refer to ```colour mode list``` for valid mode ```[M]```|
| ```\LAYER{[L]}``` | Sets active Layer to ```[L]``` | Layer ```[L]``` should be defined. Example: For ```\Layer{2}```, ```Layer 2``` should be defined.|

#### Colour mode list

|<div style="width:90px">Colour Mode</div> | Description | Note|
|:---|:---|:---|
| ```const``` | Sets a constant colour | |
| ```pressed``` | Sets the given colour if the button is triggered | |
| ```not pressed``` | Sets the given colour if the button is not triggered | |
| ```toggle``` | Toggles the colour on button trigger | |
| ```MIDI``` | Sets the colour based on message received over MIDI | Button has to be set as MIDI button with ```\MIDI_CC_KEY```. The channel and number set in ```\MIDI_CC_KEY``` will be used to trigger the colour|
| ```disabled``` | Disables the colour on the key | |

#### Colour list

|Colour|Note|
|:---|:---|
| ```red``` | |
| ```yellow``` | |
| ```green``` | |
|```cyan``` | |
| ```blue``` | |
| ```magenta``` | |
| ```white``` | |
| ```none/off``` | Used for modes, where colour is not a relevant parameter|

## Case

The Case for the Typeuwu is 3D-printed. Solidworks project files, STEP and STL files are included.

## Pictures

![Keyboard with module](04%20Pictures/20.jpeg)
![Keyboard](04%20Pictures/5.jpeg)
![Keyboard](04%20Pictures/The%20Typeuwu%20PCB%20back.png)
![Keyboard](04%20Pictures/The%20Typeuwu%20PCB%20front.png)
