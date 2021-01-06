from distutils.core import setup
import py2exe
import os

installer_script = os.path.join(os.getcwd(), 'src\\installer\\ArduinoModbusInstaller.py')
modbus_ini = os.path.join(os.getcwd(), 'src\\ini_settings\\modbus_ini_settings')

data_files = [('', [modbus_ini])]
setup(windows=[installer_script], data_files = data_files)