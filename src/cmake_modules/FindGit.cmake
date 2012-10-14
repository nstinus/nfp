execute_process(
  COMMAND git describe --long
  COMMAND cut -d "-" -f 1
  COMMAND cut -b 2-
  COMMAND cut -d "." -f 1
  OUTPUT_VARIABLE NFP_MAJOR_VERSION
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
mark_as_advanced(NFP_MAJOR_VERSION)

execute_process(
  COMMAND git describe --long
  COMMAND cut -d "-" -f 1
  COMMAND cut -b 2-
  COMMAND cut -d "." -f 2
  OUTPUT_VARIABLE NFP_MINOR_VERSION
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
mark_as_advanced(NFP_MINOR_VERSION)

execute_process(
  COMMAND git describe --long
  COMMAND cut -d "." -f 3
  COMMAND cut -d "-" -f 1-2
  COMMAND tr "-" "."
  OUTPUT_VARIABLE NFP_PATCH_VERSION
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
mark_as_advanced(NFP_PATCH_VERSION)

execute_process(
  COMMAND git rev-list HEAD
  COMMAND head -n 1
  OUTPUT_VARIABLE NFP_GIT_SHA1
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
mark_as_advanced(NFP_GIT_SHA1)