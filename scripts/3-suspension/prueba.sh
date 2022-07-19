echo Ejecutando modulo Memoria en otra terminal
cd ../../memoria
gnome-terminal -e ./memoria

echo Ejecutando modulo CPU en otra terminal
cd ../cpu
gnome-terminal -e ./cpu

echo Ejecutando modulo Kernel en otra terminal
cd ../kernel
gnome-terminal -e ./kernel

echo Enviando consolas...
cd ../consola
gnome-terminal -e "./consola 20 ../scripts/3-suspension/SUSPE_1"
gnome-terminal -e "./consola 20 ../scripts/3-suspension/SUSPE_2"
gnome-terminal -e "./consola 20 ../scripts/3-suspension/SUSPE_3"