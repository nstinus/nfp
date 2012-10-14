%module nfp

%include "std_string.i"
%include "std_list.i"
%include "std_map.i"
%include "stdint.i"


%include "core/Rating.h"
%include "core/logging.h"
%include "core/RatingsManager.h"


%{
#include "core/Rating.h"
#include "core/logging.h"
#include "core/RatingsManager.h"
%}
