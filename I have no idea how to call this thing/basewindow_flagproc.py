import sys
import GUI
import AZflib
import importlib
strid = GUI.GetSTRID()
wnd = GUI.GetArg(strid,0)
GUI.Memory_Set(wnd,"istopmoving","BOOL",False)
