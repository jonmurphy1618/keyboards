## QMK Flashing Steps

1. Install the qmk firmware. My preference is linux.  
```bash
git clone --recurse-submodules https://github.com/qmk/qmk_firmware.git
cd qmk_firmware
util/qmk_install.sh
```

2. Test install by building the default keymap.  
```bash
make planck/rev5:default
```

3. Customize the keymap.
```bash
cd qmk_firmware/keyboards/planck/keymaps
```
Copy custom keymap folder to the qmk firmware directory.  
```bash
cp ~/keyboards/planck/qmk/* ./<my_keymap__name>/
```
The following files allow for customization:
 - config.h (includes hardware features and definitions)
 - keymap.c (text file for key mapping)
 - rules.mk (includes major features like bluetooth, midi, led, sounds, nkro)

4. Build custom firmware.  
```bash
make planck/rev5:<my_keymap_name>
```

5. Install custom firmware. Start by entering the keyboard into bootloader mode. Press the reset keycode or short reset to ground with a wire.
```bash
make planck/rev5:<my_keymap_name>:dfu
```

6. Done! Test any changes to confirm it's working as intended. Please considering sharing your keymap on qmk's [github](https://github.com/qmk/qmk_firmware/tree/master/keyboards/planck/keymaps). 
It's also a great place to browse keymaps and find new tricks.
