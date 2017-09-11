      BLOCK DATA PRFDTA

!     PRFDTA CONTAINS AEROSOL PROFILE DATA.

!     COMMONS:

!     /PRFD/
!       ZHT     ALTITUDE GRID FOR AEROSOL PROFILE DATA [KM].
!       HZ2K    0-3KM HAZE W/ 50, 23, 10, 5 & 2 KM VIS [550NM EXT/KM].
!       FAWI50  2-11KM  FALL/WINTER  PROFILE W/ 50KM VIS [550NM EXT/KM].
!       FAWI23  2-11KM  FALL/WINTER  PROFILE W/ 23KM VIS [550NM EXT/KM].
!       SPSU50  2-11KM SPRING/SUMMER PROFILE W/ 50KM VIS [550NM EXT/KM].
!       SPSU23  2-11KM SPRING/SUMMER PROFILE W/ 23KM VIS [550NM EXT/KM].
!       BASTFW  10-35KM  FALL/WINTER  BACKGROUND [550NM EXT/KM].
!       VUMOFW  10-35KM  FALL/WINTER  MODERATE VOLCANIC [550NM EXT/KM].
!       HIVUFW  10-35KM  FALL/WINTER  HIGH VOLCANIC [550NM EXT/KM].
!       EXVUFW  10-35KM  FALL/WINTER  EXTREME VOLCANIC [550NM EXT/KM].
!       BASTSS  10-35KM SPRING/SUMMER BACKGROUND [550NM EXT/KM].
!       UPNATM  >30KM NORMAL UPPER ATMOSPHERIC PROFILE [550NM EXT/KM].
!       VUTONO  >30KM VOLCANIC TO NORMAL TRANSITION [550NM EXT/KM].
      REAL ZHT,HZ2K,FAWI50,FAWI23,SPSU50,SPSU23,BASTFW,VUMOFW,          &
     &  HIVUFW,EXVUFW,BASTSS,VUMOSS,HIVUSS,EXVUSS,UPNATM,VUTONO
      COMMON/PRFD/ZHT(34),HZ2K(34,5),                                   &
     &  FAWI50(34),FAWI23(34),SPSU50(34),SPSU23(34),                    &
     &  BASTFW(34),VUMOFW(34),HIVUFW(34),EXVUFW(34),BASTSS(34),         &
     &  VUMOSS(34),HIVUSS(34),EXVUSS(34),UPNATM(34),VUTONO(34)
      SAVE /PRFD/
      DATA ZHT/0.,  1.,  2.,  3.,  4.,  5.,  6.,  7.,  8.,  9., 10.,    &
     &   11., 12., 13., 14., 15., 16., 17., 18., 19., 20., 21., 22.,    &
     &   23., 24., 25., 30., 35., 40., 45., 50., 70.,100.,   99999./
      DATA HZ2K/ 0.0667, 0.0415, 0.0260, 31*0.,                         &
     &           0.1580, 0.0991, 0.0621, 31*0.,                         &
     &           0.3790, 0.3790, 0.0621, 31*0.,                         &
     &           0.7700, 0.7700, 0.0621, 31*0.,                         &
     &           1.9400, 1.9400, 0.0621, 31*0./
      DATA FAWI50/ 3*0., 0.011400, 0.006430, 0.004850, 0.003540,        &
     &                   0.002310, 0.001410, 0.000980, 0.000787, 23*0./
      DATA FAWI23/ 3*0., 0.027200, 0.012000, 0.004850, 0.003540,        &
     &                   0.002310, 0.001410, 0.000980, 0.000787, 23*0./
      DATA SPSU50/ 3*0., 0.014600, 0.010200, 0.009310, 0.007710,        &
     &                   0.006230, 0.003370, 0.001820, 0.001140, 23*0./
      DATA SPSU23/ 3*0., 0.034600, 0.018500, 0.009310, 0.007710,        &
     &                   0.006230, 0.003370, 0.001820, 0.001140, 23*0./
      DATA BASTFW/ 11*0., 7.14E-4, 6.64E-4, 6.23E-4, 6.45E-4,           &
     &  6.43E-4, 6.41E-4, 6.00E-4, 5.62E-4, 4.91E-4, 4.23E-4,           &
     &  3.52E-4, 2.95E-4, 2.42E-4, 1.90E-4, 1.50E-4, 3.32E-5, 7*0./
      DATA VUMOFW/ 11*0., 1.79E-3, 2.21E-3, 2.75E-3, 2.89E-3,           &
     &  2.92E-3, 2.73E-3, 2.46E-3, 2.10E-3, 1.71E-3, 1.35E-3,           &
     &  1.09E-3, 8.60E-4, 6.60E-4, 5.15E-4, 4.09E-4, 7.60E-5, 7*0./
      DATA HIVUFW/ 11*0., 2.31E-3, 3.25E-3, 4.52E-3, 6.40E-3,           &
     &  7.81E-3, 9.42E-3, 1.07E-2, 1.10E-2, 8.60E-3, 5.10E-3,           &
     &  2.70E-3, 1.46E-3, 8.90E-4, 5.80E-4, 4.09E-4, 7.60E-5 ,7*0./
      DATA EXVUFW/ 11*0., 2.31E-3, 3.25E-3, 4.52E-3, 6.40E-3,           &
     &  1.01E-2, 2.35E-2, 6.10E-2, 1.00E-1, 4.00E-2, 9.15E-3,           &
     &  3.13E-3, 1.46E-3, 8.90E-4, 5.80E-4, 4.09E-4, 7.60E-5 ,7*0./
      DATA BASTSS/ 11*0., 7.99E-4, 6.41E-4, 5.17E-4, 4.42E-4,           &
     &  3.95E-4, 3.82E-4, 4.25E-4, 5.20E-4, 5.81E-4, 5.89E-4,           &
     &  5.02E-4, 4.20E-4, 3.00E-4, 1.98E-4, 1.31E-4, 3.32E-5, 7*0./
      DATA VUMOSS/ 11*0., 2.12E-3, 2.45E-3, 2.80E-3, 2.89E-3,           &
     &  2.92E-3, 2.73E-3, 2.46E-3, 2.10E-3, 1.71E-3, 1.35E-3,           &
     &  1.09E-3, 8.60E-4, 6.60E-4, 5.15E-4, 4.09E-4, 7.60E-5, 7*0./
      DATA HIVUSS/ 11*0., 2.12E-3, 2.45E-3, 2.80E-3, 3.60E-3,           &
     &  5.23E-3, 8.11E-3, 1.20E-2, 1.52E-2, 1.53E-2, 1.17E-2,           &
     &  7.09E-3, 4.50E-3, 2.40E-3, 1.28E-3, 7.76E-4, 7.60E-5, 7*0./
      DATA EXVUSS/ 11*0., 2.12E-3, 2.45E-3, 2.80E-3, 3.60E-3,           &
     &  5.23E-3, 8.11E-3, 1.27E-2, 2.32E-2, 4.85E-2, 1.00E-1,           &
     &  5.50E-2, 6.10E-3, 2.40E-3, 1.28E-3, 7.76E-4, 7.60E-5, 7*0./
      DATA UPNATM/ 26*0., 3.32E-05, 4.30E-06, 1.67E-06,                 &
     &          8.00E-07, 4.20E-07, 3.20E-08, 1.90E-10, 0./
      DATA VUTONO/ 26*0., 7.60E-05, 8.00E-06, 1.67E-06,                 &
     &          8.00E-07, 4.20E-07, 3.20E-08, 1.90E-10, 0./
      END
