import sys
import GUI
strid = GUI.GetSTRID()
wnd = GUI.GetArg(strid, 0)
msg = GUI.GetArg(strid, 3)
if msg == "LBUTTONUP":
    GUI.HideWindow(wnd)
GUI.Exit(strid)