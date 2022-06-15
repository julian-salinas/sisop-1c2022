echo Arranca eliminar_logs.sh
sleep 1
echo Eliminando: memoria.log
cd ../memoria/cfg
rm memoria.log
cd ../..
sleep 1
echo Eliminando: cpu.log
cd cpu/cfg
rm cpu.log
cd ../..
sleep 1
echo Eliminando: kernel.log
cd kernel/cfg
rm kernel.log
cd ../..
sleep 1
echo Eliminando: consola.log
cd consola/cfg
rm consola.log
cd ../..
sleep 1
echo Terminó la eliminación de los logs
