#!/usr/bin/env python
import os
import sys
import re
import numpy as np
from subprocess import call
from scipy.interpolate import splrep,splev
try:
    from io import StringIO
except ImportError:
    from StringIO import StringIO

#NTH = 16
NTH = 4
TH_SUN = 20.0 # Solar zenith angle in deg
PH_SUN = 0.0 # Solar azimuth angle in deg
TH_PNL = 0.0 # Zenith angle of solar panel's normal direction in deg
PH_PNL = 0.0 # Azimuth angle of solar panel's normal direction in deg
IAER = 1 # Rural aerosol model
VIS = 20.0 # Visibility in km
WMIN = 300.0 # Min. wavelength in nm
WMAX = 1500.0 # Max. wavelength in nm
MODTRAN_DATDIR = '/usr/local/Mod5.2.0.0/org/DATA'
MODTRAN_BINDIR = '/usr/local/Mod5.2.0.0/org'

def run_modtran(th_los=60.0,ph_los=0.0,th_sun=TH_SUN,ph_sun=PH_SUN,iaer=IAER,vis=VIS,wmin=WMIN,wmax=WMAX,fluxout=False):
    for fnam in ['modtran.7sc','modtran.flx','modtran.plt','modtran.psc','modtran.tp6','modtran.tp7']:
        if os.path.exists(fnam):
            os.remove(fnam)
    with open('mod5root.in','w') as fp:
        fp.write('modtran\n')
    with open('modtran.tp5','w') as fp:
        fp.write('mmf+2    3    2    1    0    0    0    0    0    0    0    0    1    .000 0.1000\n')
        fp.write('ft  16 0.0380.0000001.000000001.000000000f t f f\n')
        fp.write('01_2008\n')
        fp.write('  {:3d}    0    0    0    0    0{:10.5f}.000000000.000000000.000000000.000000000\n'.format(iaer,vis))
        fp.write('    0.0000    0.0000{:10.4f}    0.0000     0.000     0.000    0          0.000\n'.format(th_los))
        fp.write('    2    2   93    0\n')
        fp.write('{:10.4f}{:10.4f}     0.000     0.000     0.000     0.000     0.000     0.000\n'.format(ph_sun-ph_los,th_sun))
        fp.write('{:10.0f}{:10.0f}       1.0       2.0rw        {:10s}\n'.format(1.0e7/wmax,1.0e7/wmin,' t    f  1' if fluxout else ''))
        fp.write('    0\n')
    if not os.path.exists('DATA'):
        os.symlink(MODTRAN_DATDIR,'DATA')
    call(os.path.join(MODTRAN_BINDIR,'Mod90_5.2.0.0.exe'))

def get_flux():
    with open('modtran.flx','r') as fp:
        while True:
            line = fp.readline()
            if re.search('DIFFUSE',line):
                line = fp.readline()
                break
        lines = fp.read()
    indx = lines.index('=')
    f,fu,fd,fs = np.loadtxt(StringIO(lines[:indx]),usecols=(0,1,2,3),unpack=True)
    fact = f*f
    w = 1.0e7/f[::-1]
    return w,(fu*fact)[::-1],(fd*fact)[::-1],(fs*fact)[::-1]

def get_radiance():
    f,y = np.loadtxt('modtran.plt',unpack=True)
    fact = f*f
    w = 1.0e7/f[::-1]
    return w,(y*fact)[::-1]

th_pnl_r = np.radians(TH_PNL)
ph_pnl_r = np.radians(PH_PNL)
v_pnl = np.array([np.sin(th_pnl_r)*np.cos(ph_pnl_r),np.sin(th_pnl_r)*np.sin(ph_pnl_r),np.cos(th_pnl_r)])

dth = 90.0/NTH
dth_r = np.radians(dth)
dlen = 2.0*np.pi*np.sin(0.5*dth_r)
k = 0
for i in range(NTH):
    th_1 = dth_r*i
    th_2 = th_1+dth_r
    th_c = (th_1+th_2)*0.5
    nph = int(2.0*np.pi*np.sin(th_c)/dlen+0.5)
    if nph <= 1:
        nph = 1
    k += nph
kmax = k
k = 0
yi = 0.0
for i in range(NTH):
    th_1 = dth_r*i
    th_2 = th_1+dth_r
    th_c = (th_1+th_2)*0.5
    nph = int(2.0*np.pi*np.sin(th_c)/dlen+0.5)
    if nph <= 1:
        nph = 1
        dph = 0.0
        th_los = 0.0
    else:
        dph = 360.0/nph
        th_los = np.degrees(th_c)
    domg = 2.0*np.pi*(np.cos(th_1)-np.cos(th_2))/nph
    for j in range(nph):
        k += 1
        sys.stderr.write('{:4d} {:4d} {:4d}/{:4d}\n'.format(i,j,k,kmax))
        ph_los = dph*(float(j)+0.5)
        run_modtran(th_los,ph_los)
        wi,yr = get_radiance()
        th_los_r = np.radians(th_los)
        ph_los_r = np.radians(ph_los)
        v_los = np.array([np.sin(th_los_r)*np.cos(ph_los_r),np.sin(th_los_r)*np.sin(ph_los_r),np.cos(th_los_r)])
        yi += yr*np.dot(v_pnl,v_los)*domg
run_modtran(0.0,0.0,fluxout=True)
wf,fu,fd,fs = get_flux()


