echo Inicia configuracion.sh
cd ~
echo 1) Clonar las commons
sleep 2
git clone https://github.com/sisoputnfrba/so-commons-library/

echo 2) Instalación de commons...
sleep 2
cd so-commons-library
sudo make install

echo 3) Volviendo a la carpeta principal...
cd /home/utnso/tp-2022-1c-Grupo-Final-Final

echo Finaliza configuracion.sh