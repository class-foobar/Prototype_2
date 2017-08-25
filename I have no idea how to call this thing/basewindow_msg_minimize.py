import sys
import GUI
strid = sys.argv[0]
wnd = GUI.GetArg(strid, 0)
GUI.HideWindow(wnd)
GUI.Exit(strid)