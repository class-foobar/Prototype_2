import sys
import GUI
import AZflib
import importlib
args = sys.argv
strid = sys.argv[0]
loc = sys.argv[1]
wnd = GUI.GetArg(strid,0)
GUI.Memory_Set(wnd,"istopmoving","BOOL",False)
GUI.Exit(strid)