#################################################################################
# A makefile for programing and debugging an AVR butterfly (atmega169p).        #
# Origanylly writen for old lab equipment by vikson-6 and uppdated to the new   #
# equipment and debbuging by eriser-8.                                          #
#################################################################################

CC         = avr-gcc
OBJCOPY    = avr-objcopy
OBJDUMP    = avr-objdump
SIZE       = avr-size
TARGET     = main
CONTROLLER = atmega169p
SRC        = $(wildcard *.c)
OBJ        = ${SRC:.c=.o}
D_OBJ      = $(addprefix ./debug/, $(OBJ))
R_OBJ      = $(addprefix ./run/, $(OBJ))
INCLUDE    = ./include

# Flags copied from Atmel Studio's build output.
CFLAGS += -DBOARD=USER_BOARD -DDEBUG -fdata-sections -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -mmcu=$(CONTROLLER) -std=gnu99 -fno-strict-aliasing -Wstrict-prototypes -Wmissing-prototypes -Werror-implicit-function-declaration -Wpointer-arith -mrelax

# Flags only on run
run: CFLAGS += -O2
run: BUILD_PATH = ./run/

# Flags only on debug
debug: CFLAGS += -O0 -ggdb3
debug: BUILD_PATH = ./debug/

# The rest of the flags for all builds.
CFLAGS += -Wl,--start-group -Wl,-lm -Wl,--end-group -Wl,--gc-sections -Wl,--relax
CFLAGS += $(foreach includedir,$(INCLUDE),-I$(includedir)) # Add each dir in ${INCLUDE} as an include directory.

# Flags not found in Atmel Studio's build output (but should be there).
EXTRAFLAGS = -Wextra -pedantic -Wno-expansion-to-defined -Wno-int-to-pointer-cast

# Quirk; specify non-file targets.
# Explaination at <https://stackoverflow.com/questions/2145590/what-is-the-purpose-of-phony-in-a-makefile#2145605>
.PHONY: all clean

all: run

run: mkpath ./run/$(TARGET).elf hex upload

debug: mkpath ./debug/$(TARGET).elf d_hex upload
	avarice -4 -P atmega169 :4242 &
	avr-gdb debug/$(TARGET).elf -ex "target remote localhost:4242"

mkpath:
	mkdir -p $(BUILD_PATH)

./run/$(TARGET).elf : $(R_OBJ)
	$(CC) $(CFLAGS) $(EXTRAFLAGS) -o $@ $(R_OBJ) $(ASFLAGS)

./debug/$(TARGET).elf : $(D_OBJ)
	$(CC) $(CFLAGS) $(EXTRAFLAGS) -o $@ $(D_OBJ) $(ASFLAGS)

./run/%.o : %.c
	$(CC) $(CFLAGS) $(EXTRAFLAGS) -o $@ -c $<

./debug/%.o : %.c
	$(CC) $(CFLAGS) $(EXTRAFLAGS) -o $@ -c $<

hex: ./run/$(TARGET).elf
	$(OBJCOPY) -O ihex -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures ./run/$(TARGET).elf ./run/$(TARGET).hex
	$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0 --no-change-warnings -O ihex ./run/$(TARGET).elf ./run/$(TARGET).eep || exit 0
	$(OBJDUMP) -h -S ./run/$(TARGET).elf > ./run/$(TARGET).lss
	$(OBJCOPY) -O srec -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures ./run/$(TARGET).elf ./run/$(TARGET).srec

d_hex: ./debug/$(TARGET).elf
	$(OBJCOPY) -O ihex -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures ./debug/$(TARGET).elf ./debug/$(TARGET).hex
	$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0 --no-change-warnings -O ihex ./debug/$(TARGET).elf ./debug/$(TARGET).eep || exit 0
	$(OBJDUMP) -h -S ./debug/$(TARGET).elf > ./debug/$(TARGET).lss
	$(OBJCOPY) -O srec -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures ./debug/$(TARGET).elf ./debug/$(TARGET).srec

upload:
	avrdude -p m169 -c atmelice -U flash:w:$(BUILD_PATH)/main.hex

clean:
	@- rm -f *.o *.elf *.hex *.eep *.lss *.srec
