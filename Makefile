CC				= gcc -O2
FC				= gfortran -O2
LD				= g++
CFLAGS				= -g -O2
BINDIR				= ../bin
GCCVER				:= $(shell gcc -dumpversion)
INCDIR				= -I/usr/local/include
LIBDIR				= -L/usr/local/lib
LIB0				= -lm
LIB1				= -lcfitsio
LIB2				= -ljpeg
LIB3				= -lgsl -lgslcblas
OPT				= -O3
OBJ1				= $(BINDIR)/strutil.o
OBJ2				= $(BINDIR)/numutil.o
OBJ3				= $(BINDIR)/aerprf.o $(BINDIR)/prfdta.o
OBJ4				= $(BINDIR)/MIEV0.o $(BINDIR)/ErrPack.o
OBJ5				= $(BINDIR)/tmx.o $(BINDIR)/lpd.o
OBJS				= $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ4) $(OBJ5)
WARNING				= -Wall -Wno-misleading-indentation
PROGRAM				= run_modtran run_modtran_org solar_radiation write_tape5 write_tape5_org

#==========================================
all				: $(PROGRAM)
				rm -f $(OBJS)
clean				:
				rm -f $(OBJS)
write_tape5			: write_tape5.c $(OBJ1)
				$(CC) $@.c $(OBJ1) -o $(BINDIR)/$@ $(LIB0) $(WARNING)
write_tape5_org			: write_tape5.c $(OBJ1)
				$(CC) write_tape5.c $(OBJ1) -o $(BINDIR)/$@ $(LIB0) $(WARNING) -DOLDMODE
pfunc				: pfunc.c $(OBJ1)
				$(CC) $@.c $(OBJ1) -o $(BINDIR)/$@ $(LIB0) $(WARNING)
run_modtran			: run_modtran.c $(OBJ1) $(OBJ3) $(OBJ4) $(OBJ5)
				$(CC) -c -o $(BINDIR)/$@.o $@.c
				$(FC) -o $(BINDIR)/$@ $(BINDIR)/$@.o $(OBJ1) $(OBJ3) $(OBJ4) $(OBJ5) $(LIB0) $(CFLAGS) $(WARNING) -mcmodel=large
				rm -f $(BINDIR)/$@.o
run_modtran_org			: run_modtran.c $(OBJ1) $(OBJ3) $(OBJ4) $(OBJ5)
				$(CC) -c -o $(BINDIR)/$@.o run_modtran.c -DOLDMODE
				$(FC) -o $(BINDIR)/$@ $(BINDIR)/$@.o $(OBJ1) $(OBJ3) $(OBJ4) $(OBJ5) $(LIB0) $(CFLAGS) $(WARNING) -mcmodel=large
				rm -f $(BINDIR)/$@.o
run_modtran5			: run_modtran5.c $(OBJ1) $(OBJ3)
				$(CC) -o $(BINDIR)/$@ $@.c $(OBJ1) $(OBJ3) $(LIB0) $(WARNING)
solar_radiation			: solar_radiation.c $(OBJ1) $(OBJ3) $(OBJ4)
				$(CC) -c $@.c -o $(BINDIR)/$@.o $(WARNING)
				$(FC) -o $(BINDIR)/$@ $@.c $(OBJ1) $(OBJ3) $(OBJ4) $(LIB0) $(WARNING)
$(BINDIR)/strutil.o		: strutil.c
				$(CC) -c strutil.c -o $@ $(WARNING)
$(BINDIR)/numutil.o		: numutil.c
				$(CC) -c numutil.c -o $@ $(WARNING)
$(BINDIR)/aerprf.o		: aerprf.f
				$(FC) -c -o $(BINDIR)/$@ aerprf.f $(WARNING)
$(BINDIR)/prfdta.o		: prfdta.f
				$(FC) -c -o $(BINDIR)/$@ prfdta.f $(WARNING)
$(BINDIR)/MIEV0.o		: MIEV0.f
				$(FC) -c MIEV0.f -o $@ $(WARNING)
$(BINDIR)/ErrPack.o		: ErrPack.f
				$(FC) -c ErrPack.f -o $@ $(WARNING)
$(BINDIR)/tmx.o			: tmx.f tmd.par.f
				$(FC) -c tmx.f -o $@ -mcmodel=large
$(BINDIR)/lpd.o			: lpd.f
				$(FC) -c lpd.f -o $@
