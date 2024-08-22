import time
import grovepi

# Connecter le capteur d'humidité du sol au port A0
sensor = 2

while True:
    try:
        # Lire la valeur analogique du capteur
        moisture_value = grovepi.analogRead(sensor)
        print("Moisture Level: ", moisture_value)
        time.sleep(0.5)

    except KeyboardInterrupt:
        break
    except IOError:
        print("Error")