# KEYLOGGER
Advanced Keylogger written in C

## Keyboard supported
> US keyboard ![keyboard](/img/USkeyboard.jpeg)
> FR keyboard ![keyboard](/img/FRkeyboard.png)

## Specificities
- This keylogger detect and handle:
  - [x] All normal keys (a, z, e, r, etc.)
  - [x] All modifiers keys (Shift, Control, Alt, AltGr, etc.)
  - [x] The CapsLock key
  - [x] All Keypad keys

- This keylogger detect but do not handle:
  - [ ] Scroll Lock
  - [ ] Num Lock (it gets the keypad buttons like the num lock is always ON)

- The keylogger will print on STDOUT all the keys pressed or the result of the modifiers:
  - Pressing "a"              => a,
  - Pressing "Shift" then "a" => A  ( not \<Shift\>+a )
  - Pressing "Control" then "c" => Control_c

## Dependencies
> The X11 library
```
apt install libx11-dev
```
## Author
- Gilles Bourgeois
