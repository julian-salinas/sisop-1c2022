echo Inicia configuracion.sh
cd ~
echo 1- Clonar las commons
sleep 2
git clone https://github.com/sisoputnfrba/so-commons-library/

echo 2- Instalación de commons...
sleep 2
cd so-commons-library
sudo make install

echo 3- Volviendo a la carpeta principal...
sleep 1
cd /home/utnso/tp-2022-1c-Grupo-Final-Final

echo 4- Crear carpetas obj
sleep 1
cd consola/
mkdir obj

cd ..
cd cpu/
mkdir obj

cd ..
cd kernel/
mkdir obj

cd ..
cd memoria/
mkdir obj

cd ..
cd shared/
mkdir obj

echo 5- Crear carpeta swap
sleep 1
cd ../memoria/
mkdir swapfiles

echo Finaliza configuracion.sh