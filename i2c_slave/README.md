# i2c_slave
> Sur "arduino pro mini" il y a 5 entrées analogiques A0..A4
> Le protocole I2C utilise A3 et A4
> On ne peut donc utiliser, au travers de I2C, que 3 entrées analogiques A0..A2
> Cet exemple montre que la tentative d'utilisation d'une 4eme entrée A4 est un échec.
> La solution est d'utiliser l'autre bus disponible SPI.