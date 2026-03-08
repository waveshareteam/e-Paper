import gpiod

# ----- Public constants (RPi.GPIO compatible) -----
BOARD = 10

IN = 0
OUT = 1

LOW = 0
HIGH = 1

# ----- Internal state -----
_mode = None
_lines = {}

# ----- Voltage domains -----
PIN_VOLTAGE = {
    # 3.3V safe GPIO
    3:  "3.3V", 5:  "3.3V",
    7:  "3.3V", 8:  "3.3V",
    10: "3.3V", 11: "3.3V",
    19: "3.3V", 21: "3.3V",
    22: "3.3V", 23: "3.3V",
    24: "3.3V", 32: "3.3V",
    34: "3.3V", 36: "5V_OD",
    37: "3.3V", 38: "3.3V",
    40: "3.3V",

    # 1.8V logic
    12: "1.8V",
    13: "1.8V",
    16: "1.8V",
    18: "1.8V",
}


# ----- Radxa Zero physical pin map -----
PINMAP = {
    3:  ("gpiochip0", 63),
    5:  ("gpiochip1", 5),    # ⚠ USB related
    7:  ("gpiochip1", 3),
    8:  ("gpiochip1", 0),
    10: ("gpiochip1", 1),
    11: ("gpiochip1", 2),
    12: ("gpiochip0", 74),
    13: ("gpiochip0", 76),
    16: ("gpiochip0", 75),
    18: ("gpiochip0", 73),
    19: ("gpiochip0", 20),
    21: ("gpiochip0", 21),
    22: ("gpiochip0", 48),   # open-drain
    23: ("gpiochip0", 23),
    24: ("gpiochip0", 22),
    27: ("gpiochip1", 3),
    28: ("gpiochip1", 2),
    32: ("gpiochip1", 4),
    34: ("gpiochip1", 8),
    36: ("gpiochip0", 24),   # ⚠ 5V open-drain
    37: ("gpiochip1", 9),
    38: ("gpiochip1", 10),
    40: ("gpiochip1", 11),
}

# ----- API -----

def setmode(mode):
    global _mode
    if mode != BOARD:
        raise ValueError("Only GPIO.BOARD mode is supported")
    _mode = mode

def setup(pin, direction, initial=None):
    if _mode != BOARD:
        raise RuntimeError("Call GPIO.setmode(GPIO.BOARD) first")

    if pin not in PINMAP:
        raise ValueError(f"Invalid pin: {pin}")

    voltage = PIN_VOLTAGE.get(pin, "unknown")
    if voltage == "1.8V":
        print(
            f"⚠ WARNING: Pin {pin} is 1.8V logic. "
            "Do NOT connect directly to 3.3V devices."
        )
    elif voltage == "5V_OD":
        print(
            f"⚠ WARNING: Pin {pin} is open-drain / 5V tolerant. "
            "External pull-up required."
        )

    chip_name, line_offset = PINMAP[pin]
    chip = gpiod.Chip(chip_name)
    line = chip.get_line(line_offset)

    if direction == OUT:
        line.request(
            consumer="RADXAZero.GPIO",
            type=gpiod.LINE_REQ_DIR_OUT,
            default_vals=[initial if initial is not None else 0],
        )
    elif direction == IN:
        line.request(
            consumer="RADXAZero.GPIO",
            type=gpiod.LINE_REQ_DIR_IN,
        )
    else:
        raise ValueError("Invalid direction")

    _lines[pin] = (chip, line)


def output(pin, value):
    if pin not in _lines:
        raise RuntimeError(f"Pin {pin} not set up")

    _, line = _lines[pin]
    line.set_value(1 if value else 0)


def input(pin):
    if pin not in _lines:
        raise RuntimeError(f"Pin {pin} not set up")

    _, line = _lines[pin]
    return line.get_value()


def cleanup(pin=None):
    if pin is None:
        pins = list(_lines.keys())
    else:
        pins = [pin]

    for p in pins:
        if p in _lines:
            chip, line = _lines.pop(p)
            line.release()
            chip.close()
