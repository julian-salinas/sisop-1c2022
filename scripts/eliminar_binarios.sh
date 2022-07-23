echo Arranca eliminar_binarios.sh
sleep 1

echo Eliminando binario: memoria
cd ../memoria/
rm memoria
cd ..
sleep 1

echo Eliminando binario: cpu
cd cpu/
rm cpu
cd ..
sleep 1

echo Eliminando binario: kernel
cd kernel/
rm kernel
cd ..
sleep 1

echo Eliminando binario: consola
cd consola/
rm consola
cd ..
sleep 1

echo Terminó la eliminación de los binarios
