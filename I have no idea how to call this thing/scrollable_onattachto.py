import AZflib
import GUI 
import GAPI
from GTYPES import *
strid = GUI.GetSTRID()
wndptr = GUI.GetArg(strid,0,"VOIDPTR")
wnd = GUI.GetWndID(wndptr)
GUI.Memory_Set(wnd,"BOOL", "isattaching",True)
GUI.Memory_Set(wnd,"BOOL", "isattaching",False)