import RPi.GPIO as GPIO


class Sensor:
    def __init__(self, sensor_id, name, state, pin):
        self.sensor_id = sensor_id
        self.name = name
        self.state = state
        self.pin = pin

        # Setup GPIO pin as an input with a pull-up resistor
        # Sensors are wired to ground: 0 = closed, 1 = open
        GPIO.setmode(GPIO.BCM)
        GPIO.setup(pin, GPIO.IN, pull_up_down=GPIO.PUD_UP)

    def update_state(self):
        # Get state from GPIO pin
        self.state = GPIO.input(self.pin)
#