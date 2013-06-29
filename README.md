Slice & print

web application for Raspberry to upload stl -->check-->create gcode ???--->print.
Currently no print

what i use: <br>
  - Admesh https://sites.google.com/a/varlog.com/www/admesh-htm (this git contain source code of this because i make little modified code)<br>
  - Cura_engine (contain Bin but for new bin compile lastes from https://github.com/Ultimaker/CuraEngine)<br>
  - Python cgi<br>


Is very early version...need work too much:<br>

  - On webpage (take nice graph and add value check)<br>
  - Cgi script (real time log from admesh and cura...)<br>
  - Send directly gcode for print software.....i want use 5dprint because i hope earlier have a Makibox at home :) <br>

Preset work but need configure fine file (no have a 3d printer and no have possible to set it :D )<br>

Thanks too much at:<br>
<br>
Ulimaker and David from great cura... and thanks for cura-engine it work very fine on Raspberry pi<br>
amartin@varlog.com  for ADMesh<br>
<br>
<br>
Thanks at all and any suggesition or help are welcome!<br>
Mauro<br>
exilaus@hotmail.com


**how stat it**<br>
Use the console to go in Slice_n_print directory and run the following commands:

    sudo cp Shape_service /etc/init.d/service_slicee
    sudo chmod 755 /etc/init.d/service_slice
    sudo update-rc.d service_slice defaults

Now every time the Rarpberry pi is started it also runs a simple python cgi webserver on port 7001

If you do not want reboot the Raspberry Pi you can run the server with this command:

    ./etc/init.d/Sservice_slice start

or likewise you can stop it with this one:
 
    ./etc/init.d/service_slice stop

Now open your browser and play <br>
