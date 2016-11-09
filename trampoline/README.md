export TRAMPOLINE_ROOT=<PATH_TO_TRAMPOLINE>
alias goil=$TRAMPOLINE_ROOT/goil/makefile-unix/goil-debug

Dans le répertoire du casse brique (examples/avr/arduinoUno/casse-brique)
goil --target=avr/arduino/uno --templates=$TRAMPOLINE_ROOT/goil/templates casseBrique.oil
./make.py
sudo avrdude -c arduino -p m328p -P /dev/ttyACM0 -U flash:w:Casse-brique.hex


[Dans le .oil] S'assurer que le chemin vers trampoline est le bon (identique à $TRAMPOLINE_ROOT)
