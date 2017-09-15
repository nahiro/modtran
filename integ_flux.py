#!/usr/bin/env python
import sys
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.cm as cm
from matplotlib.pylab import griddata
from matplotlib.colors import LogNorm
from scipy.interpolate import splrep,splev

PI = np.pi
PI2 = 2.0*PI
PI_2 = 0.5*PI
NTH = 100
NPH = 100
TH_SUN = 20.0 # Solar zenith angle in deg
PH_SUN = 110.0 # Solar azimuth angle in deg
TH_PNL = 0.0 # Zenith angle of solar panel's normal direction in deg
PH_PNL = 0.0 # Azimuth angle of solar panel's normal direction in deg

data = np.load('flux.npz')
th = data['th']
ph = data['ph']
dp = data['dph']
radiance = data['radiance']
wlen = data['wlen']
wlen_flux = data['wlen_flux']
flux_up = data['flux_up']
flux_dwn = data['flux_dwn']
flux_sun = data['flux_sun']

vdat = []
for i in range(len(radiance)):
    vdat.append(splev(wlen_flux,splrep(wlen,radiance[i])))
vdat = np.array(vdat)

xdat = np.concatenate((th,th,th,th))
ydat = np.concatenate((dp,-dp,PI2-dp,-PI2+dp))
zdat = np.concatenate((vdat,vdat,vdat,vdat))
dummy,indx = np.unique(xdat+ydat*1.0j,return_index=True)
xdat = xdat[indx]
ydat = ydat[indx]
zdat = zdat[indx]

th_min = 0.0
th_max = PI_2
dth = (th_max-th_min)/NTH
ph_min = -PI
ph_max = PI
dph = (ph_max-ph_min)/NPH
xg = np.arange(th_min+0.5*dth,th_max,dth)
yg = np.arange(ph_min+0.5*dph,ph_max,dph)
xe = np.arange(th_min,th_max+0.1*dth,dth)
ye = np.arange(ph_min,ph_max+0.1*dph,dph)
xm,ym = np.meshgrid(xg,yg)
dx,dy = np.meshgrid(-np.diff(np.cos(xe)),np.diff(ye)) # dcos(th), dph
domg = dx*dy # Sum(domg) = PI2

th_sun_r = np.radians(TH_SUN)
ph_sun_r = np.radians(PH_SUN)
th_pnl_r = np.radians(TH_PNL)
ph_pnl_r = np.radians(PH_PNL)
th_los_r = xm
ph_los_r = ph_sun_r+ym
th_pnl_c = np.cos(th_pnl_r)
th_pnl_s = np.sin(th_pnl_r)
ph_pnl_c = np.cos(ph_pnl_r)
ph_pnl_s = np.sin(ph_pnl_r)
th_los_c = np.cos(th_los_r)
th_los_s = np.sin(th_los_r)
ph_los_c = np.cos(ph_los_r)
ph_los_s = np.sin(ph_los_r)
v_pnl = np.array([th_pnl_s*ph_pnl_c,th_pnl_s*ph_pnl_s,th_pnl_c]) # solar panel's normal vector
v_los = np.array([th_los_s*ph_los_c,th_los_s*ph_los_s,th_los_c]) # LOS directional vector
fact = (v_pnl.reshape(3,1,1)*v_los).sum(axis=0)
fact[fact<0.0] = 0.0 # no backside illumination
fact *= domg

flux_sum = []
for i in range(wlen_flux.size):
    if i==0 or (i+1)%100==0 or i==wlen_flux.size-1:
        sys.stderr.write('{:5d}/{:5d}\n'.format(i+1,wlen_flux.size))
    zm = griddata(xdat,ydat,zdat[:,i],xm,ym)*fact
    flux_sum.append(zm.sum())
flux_sum = np.array(flux_sum)

with open('flux.dat','w') as fp:
    fp.write('# {:>8s} {:>13s} {:>13s} {:>13s} {:>13s}\n'.format('wlen(nm)','flux(W/m2/um)','flux_up','flux_dwn','flux_sun'))
    for i in range(wlen_flux.size):
        fp.write('{:10.2f} {:13.6e} {:13.6e} {:13.6e} {:13.6e}\n'.format(wlen_flux[i],flux_sum[i],flux_up[i],flux_dwn[i],flux_sun[i]))

"""
plt.interactive(True)
fig = plt.figure(1,facecolor='w',figsize=(6,3.5))
fig.clear()
plt.subplots_adjust(top=0.85,bottom=0.20,left=0.15,right=0.90)
ax1 = plt.subplot(111)
ax1.minorticks_on()
ax1.grid(True)
#ax1.set_xlim(0.0,90.0)
ax1.set_xlim(0.0,1.0)
ax1.set_ylim(ph_min,ph_max)
ax1.set_xlabel(r'cos($\theta$)')
ax1.set_ylabel('$\phi$')
ax1.xaxis.set_tick_params(pad=7)
ax1.yaxis.set_label_coords(-0.10,0.5)
ax1.pcolormesh(np.cos(xe),ye,zm,norm=LogNorm())
#ax1.pcolormesh(np.cos(xe),ye,domg)
#ax1.pcolormesh(xe*180.0/PI,ye,fact)
plt.draw()
"""
