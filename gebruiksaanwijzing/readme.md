# Gebruiksaanwijzing

### opladen / vervangen batterijen
Batterijen worden opgeladen met een lader die specifiek ontworpen is voor 18650 (Lithium) batterijen. Voor de batterijen kunnen worden ingestoken of verwijderd moet er gecontroleerd worden of de schakelaar uitstaat voor de zekerheid

### draadloze communicatie
#### verbinding maken
We gebruiken een HM-10 Bluetooth-module. Deze module werkt met een voedingsspanning van 3,3 V (sommige breakout-boards ondersteunen 3,6–6 V dankzij een ingebouwde spanningsregelaar).
De TX- en RX-pinnen werken op 3,3 V logic level en zijn niet 5 V-tolerant.

De HM-10 werkt op BLE (Bluetooth Low Energy) en is daardoor compatibel met zowel Android- als iOS-apparaten.

De applicatie die gebruikt wordt om de commando’s te versturen is “Serial Bluetooth Terminal”, beschikbaar in de Google Play Store.
Voor iPhone kan een BLE-compatibele terminal-app gebruikt worden, aangezien iOS enkel BLE-apps ondersteunt.

#### commando's
debug [on/off]  
run  
stop  
set cycle [µs]  
set power [0..255]  
set diff [0..1]  
set kp [0..]  
set ki [0..]  
set kd [0..]  
calibrate zwart  
calibrate wit  

### kalibratie
uitleg kalibratie  => zie video leerpad
eerst op wit zetten => calibrate wit invoeren op gsm 
dan op zwart zetten => calibrate zwart invoeren op gsm 
zwart word ongeveer naar 0 omgezet en wit ongeveer naar 1000
je moet dit iedere keer per locatie opnieuw doen door andere belichting

### settings
De robot rijdt stabiel met volgende parameters:  
cycle: 10000
power: 140
diff: 0.30
kp: 11.5
### start/stop button
uitleg locatie + werking start/stop button
