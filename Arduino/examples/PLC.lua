function sendDROFloatThroughModbus()
	-- get the DRO value from Mach4 and format it to 4 decimal places
	local axisPos = mc.mcAxisGetPos(inst, mc.X_AXIS)
	axisPos = tonumber(string.format('%0.4f', tostring(axisPos)))

	-- get the non-decimal part of the x axis DRO value
	local axisPosPrefix = 0
	if axisPos < 0 then
		axisPosPrefix = math.ceil(axisPos)
	else 
		axisPosPrefix = math.floor(axisPos)
	end

	-- get the decimal part of the DRO value and convert it into a whole number
	local axisPosDecimal = (axisPos - axisPosPrefix) * 10000

	-- get the modbus register handles 
	local xAxisDROReg1 = mc.mcRegGetHandle(inst, 'modbus1/writeXDRO0')
	local xAxisDROReg2 = mc.mcRegGetHandle(inst, 'modbus1/writeXDRO1')

	-- set the first register value to the non-decimal part of the x-axis DRO value
	mc.mcRegSetValueLong(xAxisDROReg1, axisPosPrefix)
	droRegsPos = droRegsPos + 1

	-- set the second register value to the (converted) decimal part of the x-axis DRO value
	mc.mcRegSetValueLong(xAxisDROReg2, axisPosDecimal)
end

sendDROFloatThroughModbus()