#--------------------------------------------------------
# The CMakeLists.txt for:                       paREST
# Author(s):                              
#--------------------------------------------------------

SET(SRC
  aREST.cpp
  aREST_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(paREST ${SRC})

TARGET_LINK_LIBRARIES(paREST
   ${MOOS_LIBRARIES}
   apputil
   mbutil
   m
   pthread)

