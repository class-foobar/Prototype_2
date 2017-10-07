import AZfile
import GUI
import GAPI
selectedent = GAPI.GetSelectedEntity()
oldfind = GUI.Memory_Find("selected")
if oldfind:
    oldselection = GUI.Memory_Get(1,"selected")
if selectedent != oldselection:
    sprojwnd = GUI.GetWndByName("selectprojection")
    selinfownd = GUI.GetWndByName("selectinfo")
    schoicewnd = GUI.GetWndByName("schoicewnd")
    if selectedent == 0:
        if sprojwnd != 0:
            if GUI.Memory_Get(sprojwnd,"isprojecting"):
                GUI.ChangeBoolPtr(GUI.Memory_Get(sprojwnd,"projection"),False)
        if selinfownd != 0:
           if GUI.Memory_Get(selinfownd,"hastarget"):
                GAPI.ResetInfoWnd(selinfownd)
        if schoicewnd != 0:
           if GUI.Memory_Get(selinfownd,"hastarget"):
                GAPI.ResetChoiceWnd(schoicewnd)
    else:
        if sprojwnd != 0:
            if GUI.Memory_Get(sprojwnd,"isprojecting"):
                GUI.ChangeBoolPtr(GUI.Memory_Get(sprojwnd,"projection"),False)
            texture = GAPI.CreateTextureFromEntity(selectedent)
            GUI.ProjectTexture(sprojwnd,texture,-1)
        if selinfownd != 0:
           if GUI.Memory_Get(selinfownd,"hastarget"):
                GAPI.ResetInfoWnd(selinfownd)     
           GAPI.SetInfoWnd(selectedent, selinfownd)
        if schoicewnd != 0:
           if GUI.Memory_Get(schoicewnd,"hastarget"):
                GAPI.ResetChoiceWnd(schoicewnd)
           GAPI.SetChoiceWnd(schoicewnd,schoicewnd)
GUI.Memory_Set(1,"selected","PYOBJ",selectedent)
if oldfind:
    GAPI.ReleaseEntity(oldselection)