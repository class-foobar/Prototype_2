true  = True
false = False
class int2 :
    x = int(0)
    y = int(0)
    def __init__(self,nx = 0,ny = 0):
        self.x = nx
        self.y = ny
    def __add__(self,another):
        return int2(self.x+another.x,self.y+another.y)
    def __sub__(self,another):
        return int2(self.x-another.x,self.y-another.y)
    def __mul__(self,another):
        return int2(self.x*another.x,self.y*another.y)
    def __div__(self,another):
        return int2(self.x/another.x,self.y/another.y)
    def str():
        return "{" + str(x) + ", " + str(y) + "}"
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
    def __add__(self,another):
        return int4(self.x+another.x,self.y+another.y,self.z+another.z,self.w+another.w)
    def __sub__(self,another):
        return int4(self.x-another.x,self.y-another.y,self.z-another.z,self.w-another.w)
    def __mul__(self,another):
        return int4(self.x*another.x,self.y*another.y,self.z*another.z,self.w*another.w)
    def __div__(self,another):
        return int4(self.x/another.x,self.y/another.y,self.z/another.z,self.w/another.w)
    def str():
        return "{" + str(x) + ", " + str(y) + ", " + str(z) + ", " + str(w)+ "}"
class float2 :
    x = float(0.0)
    y = float(0.0)
    def __init__(self,nx = 0.0,ny = 0.0):
        self.x = nx
        self.y = ny
    def __add__(self,another):
        return float2(self.x+another.x,self.y+another.y)
    def __sub__(self,another):
        return float2(self.x-another.x,self.y-another.y)
    def __mul__(self,another):
        return float2(self.x*another.x,self.y*another.y)
    def __div__(self,another):
        return float2(self.x/another.x,self.y/another.y)
    def str():
        return "{" + str(float(x)) + ", " + str(float(y)) + "}"
class RGBA:
    r = 0.0
    g = 0.0
    b = 0.0
    a = 1.0
    def __init__(self, r = 0.0, g = 0.0, b = 0.0, a = 1.0):
        self.r = float(r)
        self.b = float(b)
        self.g = float(g)
        self.a = float(a)
    def get255base():
        return [r*255,b*255,g*255,a*255]
class colour:
    darkblue    =RGBA (0,0,1)
    blue        =RGBA (0.1,0.1,0.9)
    lightblue   =RGBA (0.3,0.3,0.7)
    red         =RGBA (0.9,0.1,0.1)
    green       =RGBA (0.1,0.9,0.1)
    white       =RGBA (1,1,1)
    lightgrey   =RGBA (0.85,0.85,0.85)
    grey        =RGBA (0.5,0.5,0.5)
    darkgrey    =RGBA (0.25,0.25,0.25)
    black       =RGBA (0,0,0)
    yellow      =RGBA (1,1,0)
    pink        =RGBA (1,0,1)
    cyan        =RGBA (0,1,1)
class WF:
    NULL = (0<<0)
    SHOW = (1<<0)
    HIDE = (1<<1)
    class SCALETO:
        VH = (1<<2)
        V = (1<<3)
        H = (1<<4)
    PROPORTIONALPOS = (1<<5)
    SAMEPOS = (1<<6)
    NOVINIT = (1<<7)
    DRAGHAND = (2<<0)
    PROJECTION = (2<<1)
    POSBASELEFT = (2<<2)
    POSBASERIGHT = (2<<3)
    SUBFRAME = (2<<4)
class UI:
    NULL = 0
    OK = 0
    EXCEPTIONTHROWN = 1
    WRONGARG = 2
    UNKNOWN = 3
    UNKNOWNERROR = 4
    NULLPTR = 5
class AT:
    NULL = (0<<0)
    VINIT = (1<<0)
    STYLEPROPPOS = (1<<1)
    PROPPOS = (1<<2)
    SAMEPOS = (1<<3)
    FORCE = (1<<5)
class winapi:
    class msg:
        class WM:
            MOUSEMOVE = 0x0200
            LBUTTONDOWN = 0x0201
            LBUTTONUP = 0x0202
            RBUTTONDOWN = 0x0203
            RBUTTONUP = 0x0204
            MBUTTONDOWN = 0x0207
            MBUTTONUP = 0x0208
            MOUSEWHEEL = 0x020A
    def MakeLParam(a,b):
        return (a & 0xffff | ((b & 0xffff) << 16))
class pair:
    first = None
    second = None
    def __init__(self, first=None, second=None):
        self.first = first
        self.second = second
def splitvariable(v, totalbits=64, signed=True):
    pir = pair()
    v = int(v)
    first = (v & (pow(2,totalbits)-pow(2,totalbits/2)) >> totalbits/2)
    second = (v & pow(2,totalbits))
    if signed:
        pir.first -= pow(2,totalbits/2)
        pir.second -= pow(2,totalbits/2)
    pir.first = int(first)
    pir.second = int(second)
    return pir
