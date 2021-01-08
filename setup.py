from distutils.core import setup
import py2exe
import os

installer_script = os.path.join(os.getcwd(), 'src\\installer\\ArduinoModbusInstaller.py')
modbus_ini = os.path.join(os.getcwd(), 'src\\ini_settings\\modbus_ini_settings')
modbus_ini_adc = os.path.join(os.getcwd(), 'src\\ini_settings\\modbus_ini_settings_adc')
modbus_ini_adc_dac = os.path.join(os.getcwd(), 'src\\ini_settings\\modbus_ini_settings_adc_dac')
modbus_ini_dac = os.path.join(os.getcwd(), 'src\\ini_settings\\modbus_ini_settings_dac')

data_files = [('', [modbus_ini, modbus_ini_adc, modbus_ini_adc_dac, modbus_ini_dac])]
setup(windows=[installer_script], data_files = data_files)