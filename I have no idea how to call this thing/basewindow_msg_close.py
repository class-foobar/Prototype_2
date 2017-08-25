import sys
import GUI
strid = sys.argv[0]
wnd = GUI.GetArg(strid, 0)
GUI.DestroyWndAndExit(strid,wnd)