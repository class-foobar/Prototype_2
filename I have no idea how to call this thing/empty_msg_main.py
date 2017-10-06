import sys
import GUI
import AZflib
import importlib
class int2 :
    x = 0
    y = 0
    def __init__(self,nx = 0,ny = 0):
        self.x = nx
        self.y = ny
class int4 :
    x = 0
    y = 0
    z = 0
    w = 0
    def __init__(self,nx = 0,ny = 0,nz = 0,nw = 0):
        self.x = nx
        self.y = ny
        self.z = nz
        self.w = nw
strid = GUI.GetSTRID()
loc = GUI.GetFLoc(strid)
wnd = GUI.GetArg(strid,0)
floc = GUI.GetName(wnd)
isdragable = GUI.GetFlags(wnd) & (2 << 0)
boxn = GUI.GetArg(strid, 1)
umsg = GUI.GetArg(strid, 2)
msg = GUI.GetArg(strid, 3)
btptr = GUI.GetArg(strid, 5)
if msg == "MOUSEMOVE" and isdragable:
    if GUI.Memory_Find(0,"isdraggingwnd"):
        if GUI.Memory_Get(0,"isdraggingwnd")and (not GUI.Memory_Get(wnd,"isprojecting")):
            dwnd = GUI.Memory_Get(0,"dragged")
            projwnd = GUI.ProjectWindow(wnd,dwnd,1,1,1,1)
            GUI.Memory_Set(wnd,"BOOL","isprojecting",True)
            GUI.Memory_Set(wnd,"BOOL","projwnd",projwnd)
            GUI.MouseMoveDetB(btptr,True)
    if GUI.Memory_Get(wnd,"isprojecting"):
        rect = int4()
        point = int2()
        rect.x = GUI.GetRealPosX(wnd,boxn)
        rect.y = GUI.GetRealPosY(wnd,boxn)
        rect.z = GUI.GetRealSizeX(wnd,boxn)+rect.x
        rect.w = GUI.GetRealSizeY(wnd,boxn)+rect.y
        point.x = GUI.GetArg(strid,6)
        point.y = GUI.GetArg(strid,7)
        if not GUI.IsInside(point.x,point.y,rect.x,rect.y,rect.z,rect.w):
            GUI.MouseMoveDetB(btptr,False)
            projwnd = GUI.Memory_Get(wnd,"projwnd")
            GUI.ChangeBoolPtr(projwnd,False)
if msg == "LBUTTONUP" and isdragable:
    if GUI.Memory_Find(0,"isdraggingwnd"):
        rect = int4()
        point = int2()
        rect.x = GUI.GetRealPosX(wnd,boxn)
        rect.y = GUI.GetRealPosY(wnd,boxn)
        rect.z = GUI.GetRealSizeX(wnd,boxn)+rect.x
        rect.w = GUI.GetRealSizeY(wnd,boxn)+rect.y
        point.x = GUI.GetArg(strid,6)
        point.y = GUI.GetArg(strid,7)
        dwnd = GUI.Memory_Get(0,"dragged")
        if GUI.IsInside(point.x,point.y,rect.x,rect.y,rect.z,rect.w):
            GUI.AttachTo(dwnd, wnd, (1 << 2))
            projwnd = GUI.Memory_Get(wnd,"projwnd")
            GUI.ChangeBoolPtr(projwnd,False)
            GUI.HideWindow(wnd)
