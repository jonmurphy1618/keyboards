## QMK Flashing Steps

1. Install the qmk firmware. My preference is linux.  
```
git clone --recurse-submodules https://github.com/qmk/qmk_firmware.git
cd qmk_firmware
util/qmk_install.sh
```

2. Test install by building the default keymap.  
```
make planck/rev5:default
```

3. Customize the keymap.
```  
cd qmk_firmware/keyboards/planck/keymaps
```

Copy custom keymap folder to the qmk firmware directory.  
```
cp ~/keyboards/planck/qmk/* ./<my_keymap__name>/
```

The following files allow for customization:
- config.h
- keymap.c
- rules.mk

4. Build custom firmware.  
```
make planck/rev5:<my_keymap_name>
```

5. Install custom firmware. Start by entering the keyboard into bootloader mode. Press the reset keycode or short reset to ground with a wire.
```
make planck/rev5:<my_keymap_name>:dfu
```

6. Done! Test any changes to confirm it's working as intended.
