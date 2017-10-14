import AZflib
import GUI 
import GAPI
from GTYPES import *
strid = GUI.GetSTRID()
wnd = GUI.GetArg(strid, 0)
GUI.Once(wnd,strid)
boxn = GUI.GetArg(strid, 1)
umsg = GUI.GetArg(strid, 2)
msg = GUI.GetArg(strid, 3)
btptr = GUI.GetArg(strid, 5)
swnd = GUI.Memory_Get(wnd,"subwnd");
size = GUI.Memory_Get(wnd,"scrollablesize")
llpos = GUI.Memory_Get(wnd,"scrollablepos")
llrealsize = GUI.GetRealSizeY(wnd,1)
scrollsize = GUI.GetRealSizeY(wnd,2)
llwidth = GUI.GetRealSizeX(wnd,1)
relposX = GUI.GetX(wnd,1)-GUI.GetX(wnd,0)
curpos = int2(GUI.CursporX(),GUI.CursporY())
pos = int2(GUI.GetRealPosX(wnd,1), GUI.GetRealPosY(wnd,1))
relcpos = curpos-pos
def mov(wnd,strid):
    ispressed = True
    GUI.Memory_Set(1,"BOOL","isdragging",True)
    GUI.Memory_Set(1,"UINT","dragged",wnd)
    while ispressed:
        nmsg = GUI.WaitFor(wnd,"MOUSEMOVE LBUTTONUP",strid)
        oldpos = int2 (GUI.Memory_Get(wnd,"mouseXtop"), GUI.Memory_Get(wnd,"mouseYtop"))
        if nmsg == "MOUSEMOVE":
            point = int2()
            point.x = GUI.CursorX()
            point.y = GUI.CursorY()
            nposX = GUI.GetX(wnd) + point.x - oldpos.x;
            nposY = GUI.GetY(wnd) +  point.y - oldpos.y;
            if nposY > llpos.y + scrollsize:
                GUI.SetShapePos(wnd,relposX,llpos.y + llrealsize.y-scrollsize.y,2)
            elif nposY < llpos.y:            
                GUI.SetShapePos(wnd,relposX,llpos.y,2)
            else:
                GUI.SetShapePos(wnd,relposX,nposY,2)
            GUI.UpdatePos(wnd)
            GUI.Memory_Set(wnd,"mouseXtop","INT",point.x)
            GUI.Memory_Set(wnd,"mouseYtop","INT",point.y)
            if(GUI.KeyState("LBUTTON") == False):
                ispressed = False
        if nmsg == "LBUTTONUP":
            ispressed = False
            GUI.Memory_Set(wnd,"istopmoving","BOOL",False)
            GUI.MouseMoveDetB(btptr,False)
            GUI.Memory_Set(0,"BOOL","isdragging",False)
point = int2()
point.x = GUI.GetArg(strid, 6)
point.y = GUI.GetArg(strid, 7)
if msg == "LBUTTONDOWN":
    GUI.Memory_Set(wnd,"istopmoving","BOOL",True)
    GUI.Memory_Set(wnd,"mouseXtop","INT",point.x)
    GUI.Memory_Set(wnd,"mouseYtop","INT",point.y)
    GUI.MouseMoveDetB(btptr,True)
if msg == "LBUTTONUP":
    GUI.Memory_Set(wnd,"istopmoving","BOOL",False)
    GUI.MouseMoveDetB(btptr,False)
if msg == "MOUSEMOVE" and GUI.Memory_Get(wnd,"istopmoving"):
    mov(swnd,strid)