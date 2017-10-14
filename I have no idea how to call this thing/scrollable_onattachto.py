import AZflib
import GUI 
import GAPI
from GTYPES import *
strid = GUI.GetSTRID()
loc = GUI.GetFLoc(strid)
wnd = GUI.GetArg(strid,0)
floc = GUI.GetName(wnd)
GUI.Memory_Set(wnd,"BOOL", "isattaching",True)
GUI.Memory_Set(wnd,"BOOL", "isattaching",False)