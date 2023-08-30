@echo off
::This file was created automatically by CrossIDE to compile with C51.
C:
cd "\Users\alexj\Documents\ELEC2\ELEC291\Lab5\AT89LP51Rx2_make_projects\LCD\"
"C:\Users\alexj\Documents\ELEC2\ELEC291\CrossIDE\Call51\Bin\c51.exe" --use-stdout  "C:\Users\alexj\Documents\ELEC2\ELEC291\Lab5\AT89LP51Rx2_make_projects\LCD\adc_spi.c"
if not exist hex2mif.exe goto done
if exist adc_spi.ihx hex2mif adc_spi.ihx
if exist adc_spi.hex hex2mif adc_spi.hex
:done
echo done
echo Crosside_Action Set_Hex_File C:\Users\alexj\Documents\ELEC2\ELEC291\Lab5\AT89LP51Rx2_make_projects\LCD\adc_spi.hex
