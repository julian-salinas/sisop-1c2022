echo Arranca inicio.sh
sleep 2
echo Arranca: Memoria
cd ../memoria
make clean
make
cd ..
sleep 2
echo Arranca: CPU
cd cpu
make clean
make
cd ..
sleep 2
echo Arranca: Kernel
cd kernel
make clean
make
cd ..
sleep 2
echo Arranca: Consola
cd consola
make clean
make
cd ..
sleep 2
echo Se terminó la carga de los módulos
