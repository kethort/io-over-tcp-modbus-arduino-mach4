panel = mcLuaPanelParent
inst = mc.mcGetInstance()

local wxForms = {}

function wxForms.CreateCombobox(comboboxID, choices)
	local sizer = wx.wxBoxSizer(wx.wxHORIZONTAL)
	local cboSelector = wx.wxComboBox(panel, comboboxID, "", wx.wxDefaultPosition, wx.wxSize(285, 40), choices, wx.wxCB_READONLY) 
    cboSelector:SetFont(wx.wxFont(16, wx.wxMODERN, wx.wxNORMAL, wx.wxNORMAL, 0, "Arial")) 

    sizer:Add(cboSelector, 1, wx.wxALL + wx.wxGROW, 5)
    panel:SetSizer(sizer)
    sizer:SetSizeHints(panel)

    cboSelector:SetSelection(0)

    cboSelector:Connect(wx.wxEVT_COMMAND_COMBOBOX_SELECTED,     
    function (event)
    	local currSel = cboSelector:GetSelection()
    	local comboBoxSelReg = mc.mcRegGetHandle(inst, "gRegs0/cboSelection")
    	mc.mcRegSetValueLong(comboBoxSelReg, currSel)
    end)

	panel:Connect(wx.wxEVT_UPDATE_UI, function (event)
		local comboBoxSelReg = mc.mcRegGetHandle(inst, "gRegs0/cboSelection")
		local comboBoxSel = mc.mcRegGetValueLong(comboBoxSelReg)

		if comboBoxSel == 7777 then
			cboSelector:SetSelection(0)
			mc.mcRegSetValueLong(comboBoxSelReg, 0)
		end
	end)
end

function wxForms.CreateListBox()
	panel:SetSizeHints(wx.wxDefaultSize, wx.wxDefaultSize)

	gbSizer1 = wx.wxGridBagSizer(0, 0)
	gbSizer1:SetFlexibleDirection(wx.wxBOTH)
	gbSizer1:SetNonFlexibleGrowMode(wx.wxFLEX_GROWMODE_SPECIFIED)

	bSizer1 = wx.wxBoxSizer(wx.wxHORIZONTAL)

	listboxItems = {}
	listbox = wx.wxListBox(panel, wx.wxID_ANY, wx.wxDefaultPosition, wx.wxSize(250, 250), listboxItems, 0)
	listbox:SetFont(wx.wxFont(14, wx.wxMODERN, wx.wxNORMAL, wx.wxNORMAL, 0, "Arial"))
	bSizer1:Add(listbox, 0, wx.wxALL + wx.wxEXPAND, 5)

	gbSizer1:Add(bSizer1, wx.wxGBPosition(0, 0), wx.wxGBSpan(1, 1), wx.wxALL, 5)

	bSizer9 = wx.wxBoxSizer(wx.wxVERTICAL)

	bSizer2 = wx.wxBoxSizer(wx.wxVERTICAL)

	btnMoveUp = wx.wxButton(panel, wx.wxID_ANY, "▲", wx.wxDefaultPosition, wx.wxDefaultSize, 0)
	bSizer2:Add(btnMoveUp, 0, wx.wxALL, 5)

	bSizer2:Add(0, 50, 1, wx.wxEXPAND, 5)

	bSizer9:Add(bSizer2, 0, wx.wxEXPAND, 5)

	bSizer3 = wx.wxBoxSizer(wx.wxVERTICAL)

	btnAdd = wx.wxButton(panel, wx.wxID_ANY, "Add", wx.wxDefaultPosition, wx.wxDefaultSize, 0)
	bSizer3:Add(btnAdd, 0, wx.wxALL, 5)

	bSizer9:Add(bSizer3, 0, wx.wxEXPAND, 5)

	bSizer5 = wx.wxBoxSizer(wx.wxVERTICAL)

	btnRemove = wx.wxButton(panel, wx.wxID_ANY, "Remove", wx.wxDefaultPosition, wx.wxDefaultSize, 0)
	bSizer5:Add(btnRemove, 0, wx.wxALL, 5)

	bSizer9:Add(bSizer5, 0, wx.wxEXPAND, 5)

	bSizer7 = wx.wxBoxSizer(wx.wxVERTICAL)

	bSizer7:Add(0, 50, 1, wx.wxEXPAND, 5)

	btnMoveDown = wx.wxButton(panel, wx.wxID_ANY, "▼", wx.wxDefaultPosition, wx.wxDefaultSize, 0)
	bSizer7:Add(btnMoveDown, 0, wx.wxALL, 5)

	bSizer9:Add(bSizer7, 0, wx.wxEXPAND, 5)

	gbSizer1:Add(bSizer9, wx.wxGBPosition(0, 1), wx.wxGBSpan(1, 1), wx.wxALIGN_CENTER + wx.wxALL, 5)

	panel:SetSizer(gbSizer1)
	panel:Layout()

	panel:Centre(wx.wxBOTH)

	btnAdd:Connect(wx.wxEVT_COMMAND_BUTTON_CLICKED, function(event)
		local comboBoxSelReg = mc.mcRegGetHandle(inst, "gRegs0/cboSelection")
		local comboBoxSel = mc.mcRegGetValueLong(comboBoxSelReg)
		listbox:Append(string.format("%.0f", comboBoxSel))
		table.insert(listboxItems, comboBoxSel)
	end)

	btnRemove:Connect(wx.wxEVT_COMMAND_BUTTON_CLICKED, function(event)
		local currSel = listbox:GetSelection()
		if currSel >= 0 then
			listbox:Delete(currSel)
		else
			wx.wxMessageBox("No item selected from list", "Confirm", wx.wxOK)
		end
	end)

	btnMoveUp:Connect(wx.wxEVT_COMMAND_BUTTON_CLICKED, function(event)
		local currSel = listbox:GetSelection()
		local currStr = listbox:GetString(currSel)
		if currSel > 0 then
			local item = table.remove(listboxItems, currSel + 1)
			
			table.insert(listboxItems, currSel, item)
			listbox:Delete(currSel)
			listbox:InsertItems({currStr}, currSel - 1)
			listbox:SetSelection(currSel - 1)
		end
	end)

	btnMoveDown:Connect(wx.wxEVT_COMMAND_BUTTON_CLICKED, function(event)
		local currSel = listbox:GetSelection()
		local currStr = listbox:GetString(currSel)
		if currSel < listbox:GetCount() - 1 then
			local item = table.remove(listboxItems, currSel + 1)
			
			table.insert(listboxItems, currSel + 2, item)
			listbox:Delete(currSel)
			listbox:InsertItems({currStr}, currSel + 1)
			listbox:SetSelection(currSel + 1)
		end
	end)

	panel:Connect(wx.wxEVT_UPDATE_UI, function (event)
		local comboBoxSelReg = mc.mcRegGetHandle(inst, "gRegs0/cboSelection")
		local comboBoxSel = mc.mcRegGetValueLong(comboBoxSelReg)

		if comboBoxSel == 9999 then
			listbox:Set({})
			mc.mcRegSetValueLong(comboBoxSelReg, 7777)
		end
	end)
end

return wxForms