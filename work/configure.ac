AC_INIT([ABMSBenchmarkRepast], [1.0])

AC_PREFIX_DEFAULT([.])
AC_PROG_CC([gcc g++ cc])
AC_LANG([C++])
AC_PROG_CXX([[g++ gcc cc]])

#FFTW3

function test_fftw3 {
  AC_MSG_NOTICE([checking for fft])
  if test "x$FFTW3_INC_DIR" == "x" ; then
    AC_MSG_WARN([fftw3 include directory not specified])
  fi

  if test "x$FFTW3_LIB_DIR" == "x" ; then
    AC_MSG_WARN([fftw3 library directory not specified])
  fi
}


#Boost

function test_boost {
 AC_MSG_NOTICE([checking for Boost])
  if test "x$BOOST_INC_DIR" == "x" ; then
    AC_MSG_WARN([boost include directory not specified])
  fi

  if test "x$BOOST_LIB_DIR" == "x" ; then
    AC_MSG_WARN([boost library directory not specified])
  fi
}


#repast HPC

function test_repast {
 AC_MSG_NOTICE([checking for repast])
  if test "x$REPAST_INC_DIR" == "x" ; then
    AC_MSG_WARN([repast include directory not specified])
  fi

  if test "x$REPAST_LIB_DIR" == "x" ; then
    AC_MSG_WARN([repast library directory not specified])
  fi
}

############################################################
#                         ARGS                             #
############################################################

                               
# TAU
AC_ARG_WITH(tau,
            [AS_HELP_STRING([--with-tau],[TAU compilation])],
[WITH_TAU="yes";],)


AC_ARG_WITH(tau-bindir,
            [AS_HELP_STRING([--with-tau-bindir],[TAU bin directory])],
[TAU_BINDIR="${withval}/";],)


# FFTW3
AC_ARG_WITH(fftw3,
            [AS_HELP_STRING([--with-fftw3],[FFTW3 directory (must contain ./lib and ./include)])],
            [FFTW3_INC_DIR="${withval}/include/";
FFTW3_LIB_DIR="${withval}/lib/";],)


AC_ARG_WITH(fftw3-incdir,
            [AS_HELP_STRING([--with-fftw3-incdir],[FFTW3 include directory])],
            [FFTW3_INC_DIR="${withval}/";],)
AC_ARG_WITH(fftw3-libdir,
            [AS_HELP_STRING([--with-fftw3-libdir],[FFTW3 library directory])],
[FFTW3_LIB_DIR="${withval}/";],)


#Boost

AC_ARG_WITH(boost,
            [AS_HELP_STRING([--with-boost],[BOOST directory (must contain ./lib and ./include)])],
            [BOOST_INC_DIR="${withval}/include/";
BOOST_LIB_DIR="${withval}/lib/";],)


AC_ARG_WITH(boost-incdir,
            [AS_HELP_STRING([--with-boost-incdir],[BOOST include directory])],
            [BOOST_INC_DIR="${withval}/";],)
AC_ARG_WITH(boost-libdir,
            [AS_HELP_STRING([--with-boost-libdir],[BOOST library directory])],
[BOOST_LIB_DIR="${withval}/";],)

# repastHPC

AC_ARG_WITH(repast,
            [AS_HELP_STRING([--with-repast],[REPAST directory (must contain ./lib and ./include)])],
            [REPAST_INC_DIR="${withval}/include/";
REPAST_LIB_DIR="${withval}/lib/";],)

AC_ARG_WITH(repast-incdir,
            [AS_HELP_STRING([--with-repast-incdir],[REPAST include directory])],
            [REPAST_INC_DIR="${withval}";],)

AC_ARG_WITH(repast-libdir,
            [AS_HELP_STRING([--with-repast-libdir],[REPAST library directory])],
[REPAST_LIB_DIR="${withval}/";],)

# openmpi

AC_ARG_WITH(openmpi-libdir,
            [AS_HELP_STRING([--with-openmpi-libdir],[OpenMPI library directory])],
[OPENMPI_LIB_DIR="${withval}/";],)




############################################################
#                        FLAGS                             #
############################################################

# --------------------- TAU -------------------------- #
if test "x$WITH_TAU" != "x" ; then
	MPICXX="tau_cxx.sh -std=c++11"
	REPAST_HPC_DEFINES="-tau_makefile=/home/caos/amoreno/sfw/tau-2.26.3/x86_64/lib/Makefile.tau-mpi-pdt -tau_options=-optCompInst"
else
	MPICXX="mpicc -std=c++11"
fi


# --------------------- FFTW3 -------------------------- # 
if test "x$FFTW3_INC_DIR" != "x" ; then
	CXXFLAGS="-I${FFTW3_INC_DIR} ${CXXFLAGS}"
fi

if test "x$FFTW3_LIB_DIR" != "x" ; then
	LDFLAGS="-L${FFTW3_LIB_DIR} ${LDFLAGS}"
fi


# --------------------- BOOST -------------------------- # 
if test "x$BOOST_INC_DIR" != "x" ; then
	CXXFLAGS="-I${BOOST_INC_DIR} ${CXXFLAGS}"
fi

if test "x$BOOST_LIB_DIR" != "x" ; then
	LDFLAGS="-L${BOOST_LIB_DIR} ${LDFLAGS}"
fi


# --------------------- repastHPC -------------------------- # 
if test "x$REPAST_INC_DIR" != "x" ; then
	CXXFLAGS="-I${REPAST_INC_DIR} ${CXXFLAGS}"
fi

if test "x$REPAST_LIB_DIR" != "x" ; then
	LDFLAGS="-L${REPAST_LIB_DIR} ${LDFLAGS}"
fi


# --------------------- openmpi -------------------------- # 

if test "x$OPENMPI_LIB_DIR" != "x" ; then
	LDFLAGS="-L${OPENMPI_LIB_DIR} ${LDFLAGS}"
fi

############################################################
#                      CHECKING                            #
############################################################


# Checking headers

AC_CHECK_HEADERS([${FFTW3_INC_DIR}fftw3.h], [], [test_fftw3 ; AC_MSG_ERROR([Cannot find fftw3 library include files.])],
	[// Do not use default includes])

AC_CHECK_HEADERS([${BOOST_INC_DIR}/boost/regex.h], [], [test_boost ; AC_MSG_ERROR([Cannot find boost library include files.])],
        [// Do not use default includes])


# Checking libraries:

AC_CHECK_LIB([fftw3],fftw_plan_dft_r2c, [], [test_fftw3; AC_MSG_ERROR([FFTW3 library not found])])

#checking for tau_cxx.sh

AS_IF([ test "$WITH_TAU" = "yes" ],
[AC_PATH_PROG(TAU_CHECK, tau_cxx.sh, ["no"])], [])

AS_IF([ test "$WITH_TAU" = "yes"   && test "$TAU_CHECK" = "no"],
	[AC_MSG_ERROR([tau_cxx file not found])], [])
  

AC_SUBST([MPICXX], ["${MPICXX}"])
AC_SUBST([REPAST_HPC_DEFINES], ["${REPAST_HPC_DEFINES}"])
AC_SUBST([LIBS], ["${LIBS}"])
AC_SUBST([BOOST_INC_DIR], ["${BOOST_INC_DIR}"])
AC_SUBST([BOOST_LIB_DIR], ["${BOOST_LIB_DIR}"])
AC_SUBST([REPAST_INC_DIR], ["${REPAST_INC_DIR}"])
AC_SUBST([REPAST_LIB_DIR], ["${REPAST_LIB_DIR}"])
AC_SUBST([OPENMPI_LIB_DIR], ["${OPENMPI_LIB_DIR}"])
AC_SUBST([SHELL], [$SHELL])
AC_SUBST([LDFLAGS], ["${LDFLAGS}"])
AC_SUBST([CXXFLAGS], ["${CXXFLAGS}"])
AC_CONFIG_FILES([make.configure])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT()

#./configure --with-tau --with-repast=/home/caos/amoreno/sfw --with-boost-libdir=/home/caos/amoreno/sfw/Boost/Boost_1.61/lib  --with-boost-incdir=/home/caos/amoreno/sfw/Boost/Boost_1.61/include --with-fftw3-incdir=/home/caos/amoreno/sfw/include --with-fftw3-libdir=/home/caos/amoreno/sfw/lib

