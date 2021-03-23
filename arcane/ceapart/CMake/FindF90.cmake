#
# Find the Fortran90 and libraries
#
# This module defines
# F90_LIBRARIES, the libraries to link against to use F90
# F90_FOUND, If false, do not try to use F90.
 
FIND_LIBRARY(F90_IFPORT_LIBRARY ifport
  PATHS
  ${ARCANE_PACKAGE_F90_ROOT}
)

FIND_LIBRARY(F90_IFCORE_LIBRARY ifcore
  PATHS
  ${ARCANE_PACKAGE_F90_ROOT}
)

FIND_LIBRARY(F90_IRC_LIBRARY irc
  PATHS
  ${ARCANE_PACKAGE_F90_ROOT}
)

FIND_LIBRARY(F90_IRNG_LIBRARY irng
  PATHS
  ${ARCANE_PACKAGE_F90_ROOT}
)

message(statuc "ARCANE_PACKAGE_F90_ROOT = ${ARCANE_PACKAGE_F90_ROOT}")
MESSAGE(STATUS "F90_IFPORT_LIBRARY = ${F90_IFPORT_LIBRARY}")
MESSAGE(STATUS "F90_IFCORE_LIBRARY = ${F90_IFCORE_LIBRARY}")
MESSAGE(STATUS "F90_IRC_LIBRARY = ${F90_IRC_LIBRARY}")
MESSAGE(STATUS "F90_IRNG_LIBRARY = ${F90_IRNG_LIBRARY}")

SET( F90_FOUND "NO" )
if(F90_IFPORT_LIBRARY AND F90_IFCORE_LIBRARY AND F90_IRC_LIBRARY AND F90_IRNG_LIBRARY)
  set( F90_FOUND "YES" )
  set( F90_LIBRARIES ${F90_IFPORT_LIBRARY} ${F90_IFCORE_LIBRARY} ${F90_IRC_LIBRARY} ${F90_IRNG_LIBRARY} dl)
  get_filename_component(F90_LIB_PATH ${F90_IFPORT_LIBRARY} PATH)
  arcane_add_package_library(f90 F90)
endif ()
