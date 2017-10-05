Satan GH60 (Rev. CHN)
======

This is a firmware for Satan GH60. It is coded based upon the onekey example.

### LED
This firmware supports breathing and responsive in-switch LED. The backlight level increases every time a key is pressed and dims back down gently.

### Keymap
The keymap is tailored to ANSI layout with split Backspace, Left Shift, and Right Shift, but you can modify it easily based on your preference. A complete list of keycode can be found in [`../../tmk_core/doc/keycode.txt`](../../tmk_core/doc/keycode.txt). For more information on keymaps, see [`../../tmk_core/doc/keymap.md`](../../tmk_core/doc/keymap.md).

 Default QWERTY layer
```
 ,-----------------------------------------------------------.
 |  `|  1|  2|  3|  4|  5|  6|  7|  8|  9|  0|  -|  =|  \|Del|
 |-----------------------------------------------------------|
 |Tab  |  Q|  W|  E|  R|  T|  Y|  U|  I|  O|  P|  [|  ]| BS  |
 |-----------------------------------------------------------|
 |Esc   |  A|  S|  D|  F|  G|  H|  J|  K|  L|  ;|  '|   Enter|
 |-----------------------------------------------------------|
 |Shift|Num|  Z|  X|  C|  V|  B|  N|  M|  ,|  .|  /|Shift|Fn2|
 |-----------------------------------------------------------|
 |Ctl | Sys| Alt|         Space          | Fn0| Alt| App| Ctl|
 `-----------------------------------------------------------'
```

Features:
- QWERTY and Dvorak base layer selectable.
- CapsLock key as ESC.
- Backslash key as Backspace.
- CapsLock can be toggled by Num(see the keymap above) + Left Shift
- [Oneshot](../../tmk_core/doc/keymap.md#43-oneshot-modifier) Shift keys.
- Function layer activated by holding the key to the right of the spacebar. The function layer features Vim arrow keys. Home, PgDn, PgUp, End keys intuitively arranged right above Vim arrow keys. Mouse Wheel Left, Mouse Wheel Down, Mouse Wheel Up, and Mouse Wheel Right are located right below the Vim arrow keys.

 Function layer:
```
 ,-----------------------------------------------------------.
 |   | F1| F2| F3| F4| F5| F6| F7| F8| F9|F10|F11|F12|Slp|INS|
 |-----------------------------------------------------------|
 |     |LMB|MUp|RMB|   |   |Hme|PgD|PgU|End|PSc| L0| L1| DEL |
 |-----------------------------------------------------------|
 |      |MLt|MDn|MRt|   |   |Lft|Dwn| Up|Rgt|SLk|   |        |
 |-----------------------------------------------------------|
 |Caps|MB4|MMB|MB5|   |   |   |WhL|WhD|WhU|WhR|   |      |   |
 |-----------------------------------------------------------|
 |    |    |    |                        |    |    |    |    |
 `-----------------------------------------------------------'
 L0 is QWERTY and L1 is Dvorak
```

- Number pad layer activated by holding right half of the split Left Shift (ISO `\` equivalent), togglable with double tap.
```
 ,-----------------------------------------------------------.
 |   | F1| F2| F3| F4| F5| F6|  7|  8|  9|  /|  *|   |   |   |
 |-----------------------------------------------------------|
 |     |   |   |   |   |   |   |  4|  5|  6|  -|   |   |     |
 |-----------------------------------------------------------|
 |      |   |   |   |   |   |   |  1|  2|  3|  +|   |        |
 |-----------------------------------------------------------|
 |     |  |   |   |   |   |   |   |  0|  .|  *|ENT|      |   |
 |-----------------------------------------------------------|
 |    |    |    |                        |    |    |    |    |
 `-----------------------------------------------------------'
```

This project is a work in progress, right now, FN2(right half of the split Right Shift) is the same as FN0 except is can also toggle the function layer with double tap. But I have not decided what to use it for yet.

Comments, suggenstions are welcome!