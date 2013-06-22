CC= gcc
CFLAGS= 
PROGS= collatz collatz_memo collatz_memo2 collatz_memo2_multwr collatz_memo2_vect collatz_memo2_vect2

.SUFFIXES: .0 .pg .g .2 .pg2 .g2 .3 .pg3 .g3 .3c2 .3ni .pgp .pca

PROGS_0 = ${PROGS:=.0}
PROGS_PG = ${PROGS:=.pg}
PROGS_G = ${PROGS:=.g}

PROGS_2 = ${PROGS:=.2}
PROGS_PG2 = ${PROGS:=.pg2}
PROGS_G2 = ${PROGS:=.g2}

PROGS_3 = ${PROGS:=.3}
PROGS_PG3 = ${PROGS:=.pg3}
PROGS_G3 = ${PROGS:=.g3}
PROGS_3C2 = ${PROGS:=.3c2}
PROGS_3NI = ${PROGS:=.3ni}

PROGS_PGP = ${PROGS:=.pgp}
PROGS_PCA = ${PROGS:=.pca}

PROGS_CLEAN = ${PROGS} ${PROGS_0} ${PROGS_PG} ${PROGS_G} ${PROGS_2} ${PROGS_PG2} ${PROGS_G2} ${PROGS_3} ${PROGS_PG3} ${PROGS_G3} ${PROGS_3C2} ${PROGS_3NI} ${PROGS_PGP} ${PROGS_PCA}
PROGS_ALL = ${PROGS} ${PROGS_0} ${PROGS_PG} ${PROGS_G} ${PROGS_2} ${PROGS_PG2} ${PROGS_G2} ${PROGS_3} ${PROGS_PG3} ${PROGS_G3} ${PROGS_3C2} ${PROGS_3NI} ${PROGS_PGP} ${PROGS_PCA}

all: ${PROGS_ALL}

zip: 
	make cleanall; dir=`basename $(PWD)`; cd .. ; rm -f $$dir.zip; zip -r $$dir $$dir

.c.0:
	$(CC) $(CFLAGS) $< -O0 $(LDLIBS) -o $@

.c.pg:
	$(CC) $(CFLAGS) $< -pg -g -static $(LDLIBS) -o $@

.c.g:
	$(CC) $(CFLAGS) $< -g $(LDLIBS) -o $@

.c.2:
	$(CC) $(CFLAGS) $< -O2 $(LDLIBS) -o $@

.c.pg2:
	$(CC) $(CFLAGS) $< -pg -g -O2 -static $(LDLIBS) -o $@

.c.g2:
	$(CC) $(CFLAGS) $< -g -O2 $(LDLIBS) -o $@

.c.3:
	$(CC) $(CFLAGS) $< -O3 $(LDLIBS) -o $@

.c.pg3:
	$(CC) $(CFLAGS) $< -pg -g -O3 -static $(LDLIBS) -o $@

.c.g3:
	$(CC) $(CFLAGS) $< -g -O3 -static $(LDLIBS) -o $@

.c.3c2:
	$(CC) $(CFLAGS) $< -O3 -march=native  $(LDLIBS) -o $@

.c.3ni:
	$(CC) $(CFLAGS) $< -O3 -fno-inline $(LDLIBS) -o $@

.c.pgp:
	$(CC) $(CFLAGS) $< -pg -g -lc_p -lm_p -static -o $@

.c.pca:
	$(CC) $(CFLAGS) $< -O3 -march=core2  -mfpmath=sse -ffloat-store -Wl,-s,-O1 -lm  -o $@


clean:
	$(RM) -f $(PROGS_CLEAN) core.* gmon.out *~

cleanall:
	@ for i in */. ; do  cd $$i; ${MAKE} clean; cd .. ; done
