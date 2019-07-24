## QMK Flashing Steps

1. Install the qmk firmware. My preference is linux.  
```bash
git clone --recurse-submodules https://github.com/qmk/qmk_firmware.git
cd qmk_firmware
util/qmk_install.sh
```

2. Test install by building the default keymap.  
```bash
make crkbd:default
```

3. Customize the keymap.
```bash
cd qmk_firmware/keyboards/crkbd/keymaps
```
Copy custom keymap folder to the qmk firmware directory.  
```bash
cp ~/keyboards/crkbd/qmk/* ./<my_keymap__name>/
```
The following files allow for customization:
 - config.h (includes hardware features and definitions)
 - keymap.c (text file for key mapping)
 - rules.mk (includes major features like bluetooth, midi, led, sounds, nkro)

4. Build custom firmware.  
```bash
make crkbd:<my_keymap_name>
```

5. Install custom firmware. Start by entering the keyboard into bootloader mode. Press the reset keycode or short reset to ground with a wire.
```bash
make crkbd:<my_keymap_name>:avrdude
```

6. Done! Test any changes to confirm it's working as intended.
