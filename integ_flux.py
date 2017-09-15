import numpy as np
import matplotlib.pyplot as plt
import matplotlib.cm as cm
from matplotlib.pylab import griddata
from matplotlib.colors import LogNorm

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

xdat = np.concatenate((th,th,th,th))
ydat = np.concatenate((dp,-dp,2.0*np.pi-dp,-2.0*np.pi+dp))
zdat = np.concatenate((radiance,radiance,radiance,radiance))
a,indx = np.unique(xdat+ydat*1.0j,return_index=True)
xdat = xdat[indx]
ydat = ydat[indx]
zdat = zdat[indx]

th_pnl_r = np.radians(TH_PNL)
ph_pnl_r = np.radians(PH_PNL)
th_pnl_c = np.cos(th_pnl_r)
th_pnl_s = np.sin(th_pnl_r)
ph_pnl_c = np.cos(ph_pnl_r)
ph_pnl_s = np.sin(ph_pnl_r)
v_pnl = np.array([th_pnl_s*ph_pnl_c,th_pnl_s*ph_pnl_s,th_pnl_c])

nth = 100
th_min = 0.0
th_max = 0.5*np.pi
dth = (th_max-th_min)/nth
nph = 100
ph_min = -np.pi
ph_max = np.pi
dph = (ph_max-ph_min)/nph
xg = np.arange(th_min+0.5*dth,th_max,dth)
yg = np.arange(ph_min+0.5*dph,ph_max,dph)
xe = np.arange(th_min,th_max+0.1*dth,dth)
ye = np.arange(ph_min,ph_max+0.1*dph,dph)
xm,ym = np.meshgrid(xg,yg)
dx,dy = np.meshgrid(-np.diff(np.cos(xe)),np.diff(ye))
zm = griddata(xdat,ydat,zdat[:,400],xm,ym)
domg = dx*dy

plt.interactive(True)
fig = plt.figure(1,facecolor='w',figsize=(6,3.5))
fig.clear()
plt.subplots_adjust(top=0.85,bottom=0.20,left=0.15,right=0.90)
ax1 = plt.subplot(111)
ax1.minorticks_on()
ax1.grid(True)
#ax1.pcolormesh(np.cos(xe),ye,zm,norm=LogNorm())
ax1.pcolormesh(np.cos(xe),ye,domg)
#ax1.set_xlim(0.0,1.0)
#ax1.set_ylim(0.0,1.0)
ax1.set_xlabel('')
ax1.set_ylabel('')
#ax1.xaxis.set_major_locator(plt.MultipleLocator(0.5))
#ax1.yaxis.set_major_locator(plt.MultipleLocator(0.5))
ax1.xaxis.set_tick_params(pad=7)
ax1.yaxis.set_label_coords(-0.10,0.5)
#ax1.legend(prop={'size':12},numpoints=1,ncol=4,loc=8,bbox_to_anchor=(0.5,1.01),frameon=False,columnspacing=1.0)
#plt.savefig('')
plt.draw()
