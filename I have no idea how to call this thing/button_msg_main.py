import sys
import GUI
def main ():
    strid = sys.argv[0]
    wnd = GUI.GetArg(strid,0)
    boxn = GUI.GetArg(strid,1)
    umsg = GUI.GetArg(strid,2)
    msg = GUI.GetArg(strid, 3)
    btptr = GUI.GetArg(strid,5)
    class int2:
        x = 0
        y = 0
    class int4:
        x = 0
        y = 0
        z = 0
        w = 0
    rect = int4()
    point = int2()
    true = True
    false = False
    rect.x = GUI.GetRealPosX(wnd,boxn)
    rect.y = GUI.GetRealPosY(wnd,boxn)
    rect.z = GUI.GetRealSizeX(wnd,boxn)
    rect.w = GUI.GetRealSizeY(wnd,boxn)
    point.x = GUI.GetArg(strid,6)
    point.y = GUI.GetArg(strid,7)
    GUI.Exit(strid)
    hasmover = False
    hasmover = GUI.Memory_Find(wnd,"hasmover")
    if hasmover:
        hasmover = GUI.Memory_Get(wnd,"hasmover")
    isinside = False
    isinside = GUI.IsInside(point.x,point.y,rect.x,rect.y,rect.z,rect.w)
    if msg == "MOUSEMOVE":
        if  (not isinside) and hasmover:
            GUI.Memory_Set(wnd,"hasmover","BOOL",False)
            GUI.MouseMoveDetB(btptr,False)
            GUI.SetCol(wnd,"IN","OUT",true)
            return
    if not isinside:
        return
    if msg == "MOUSEMOVE":
        GUI.Memory_Set(wnd,"hasmover","BOOL",true)
        GUI.MouseMoveDetB(btptr,True)
        GUI.SetCol(wnd,"IN","OUT",true)
    elif msg == "LBUTTONDOWN":
        pass
    elif msg == "RBUTTONDOWN":
        pass
    elif msg == "MBUTTONDOWN":
        pass
    elif msg == "LBUTTONUP":
        pass
    elif msg == "RBUTTONUP":
        pass
    elif msg == "MBUTTONUP":
        pass
    elif msg == "DOUBLECLICK":
        pass
main() 