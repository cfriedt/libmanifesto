SUBDIRS =    \
	src  \
	test \
	docs

AM_CPPFLAGS = -Isrc

if HAVE_CXX
manifestoxx = manifestoxx

manifestoxx_SOURCES = \
	src/manifecto/parse-args.c \
	manifestoxx.cpp
manifestoxx_LDADD = \
	src/libmanifesto.la \
	src/libmanifestoxx.la
endif

noinst_PROGRAMS = \
	manifecto \
	$(manifestoxx)

manifecto_SOURCES = \
	src/manifecto/parse-args.c \
	manifecto.c
manifecto_LDADD = \
	src/libmanifesto.la

sampledir = $(datadir)/samples
sample_DATA = \
	$(manifecto_SOURCES) \
	$(manifestoxx_SOURCES)

.PHONY: format

format:
	if [ x"`which clang-format`" != x"" ]; then \
		clang-format -i `find @top_srcdir@ -name '*.c' -o -name '*.cpp' -o -name '*.h'`; \
	fi

C_EXE = manifecto
CXX_EXE = $(manifestoxx)
PY_EXE = src/manifesto/manifesto
EXAMPLES = $(shell find src/manifesto/examples/ -name '*.mnfs')

check: $(C_EXE) $(CXX_EXE)
	@NFAIL=0; \
	TOTAL=0; \
	if [ ! -x $(PY_EXE) ]; then exit 0; fi; \
	for tool in $(C_EXE) $(CXX_EXE); do \
			echo "================================================================"; \
			if [ "$${tool}" = "$(C_EXE)" ]; then \
			echo "                          TESTING C"; \
			else \
			echo "                          TESTING CXX"; \
			fi; \
			echo "================================================================"; \
			for mnfs in $(EXAMPLES); do \
					TOTAL=$$((TOTAL + 1)); \
					echo "----------------------------------------------------------------"; \
					echo "input: $${mnfs}"; \
					echo "----------------------------------------------------------------"; \
					mnfb=`echo $${mnfs} | sed -e 's/\.mnfs/.mnfb/'`; \
					rm -f $${mnfb}; \
					./$(PY_EXE) -I mnfs -o $${mnfb} -O mnfb $${mnfs}; \
					if [ $$? -ne 0 ]; then \
							echo "$(PY_EXE) $${mnfs} failed"; \
							NFAIL=$$((NFAIL + 1)); \
							continue; \
					fi; \
					mnfb_ref=$${mnfb}.ref; \
					mv $${mnfb} $${mnfb_ref}; \
					./$${tool} -I mnfs -o $${mnfb} -O mnfb $${mnfs}; \
					if [ $$? -ne 0 ]; then \
							echo "$${tool} $${mnfs} failed"; \
							NFAIL=$$((NFAIL + 1)); \
							continue; \
					fi; \
					md5sum_ref=`md5sum -b $${mnfb_ref} | awk '{print $$1}'`; \
					md5sum=`md5sum -b $${mnfb} 2>/dev/null | awk '{print $$1}'`; \
					echo "expected md5sum: $${md5sum_ref}"; \
					echo "actual   md5sum: $${md5sum}"; \
					if [ "$${md5sum}" != "$${md5sum_ref}" ]; then \
							echo "expected: $${mnfb_ref}"; \
							od -x $${mnfb_ref}; \
							echo "actual: $${mnfb}"; \
							od -x $${mnfb} 2>/dev/null; \
							NFAIL=$$((NFAIL + 1)); \
							continue; \
					fi; \
			done; \
	done; \
	if [ $$NFAIL -ne 0 ]; then exit 1; fi