set(CADABRA_VERSION_MAJOR 2)
set(CADABRA_VERSION_MINOR 1)
set(CADABRA_VERSION_PATCH 9)
set(COPYRIGHT_YEARS "2001-2018")
execute_process(COMMAND git rev-parse --short HEAD OUTPUT_VARIABLE GIT_SHORT_SHA     OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND git rev-list --count HEAD  OUTPUT_VARIABLE GIT_COMMIT_SERIAL OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND git log -1 --date=short --pretty=format:%cd  OUTPUT_VARIABLE GIT_COMMIT_DATE OUTPUT_STRIP_TRAILING_WHITESPACE)
if(GIT_SHORT_SHA)
  set(CADABRA_VERSION_BUILD "${GIT_COMMIT_SERIAL}.${GIT_SHORT_SHA}")
else()
  set(CADABRA_VERSION_BUILD "private")
endif()
if(GIT_COMMIT_DATE)
  set(CADABRA_VERSION_DATE "${GIT_COMMIT_DATE}")
else()
  string(TIMESTAMP THE_DATE "%Y-%m-%d" UTC)
  set(CADABRA_VERSION_DATE "${THE_DATE}") 
endif()
