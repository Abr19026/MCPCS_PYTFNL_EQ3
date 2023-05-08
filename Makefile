SERIAL_PORT ?= com3
SKETCH ?= CodigoProyecto
DIR_SKETCH ?= #Pruebas_Modulos/

ARDUINO_CLI = arduino-cli
BOARD_TYPE = arduino:avr:uno
INCLUDE = ./Include

PHONY: build upload clear

build: build/$(SKETCH)

build/$(SKETCH): $(DIR_SKETCH)$(SKETCH)/$(SKETCH).ino
	$(ARDUINO_CLI) compile "$<" -b $(BOARD_TYPE) --build-path="$@" --build-cache-path="$@" --libraries "$(INCLUDE)"

clear:
	rm -r build/$(SKETCH)

upload: build/$(SKETCH)
	"$(ARDUINO_CLI)" upload --input-dir "$<" --fqbn $(BOARD_TYPE) -p $(SERIAL_PORT)