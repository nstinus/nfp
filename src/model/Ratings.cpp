#include <glog/logging.h>

#include <math.h>

#include "Ratings.h"

Ratings::Ratings(const uint &N)
{
    LOG(INFO) << "Creating a Ratings for " << N << " elements [" << this << "]";
    LOG_VAR(RATINGS__MOVIE_ID_SIZE);
    LOG_VAR(RATINGS__USER_ID_SIZE);
    LOG_VAR(RATINGS__RATE_SIZE);
    LOG_VAR(RATINGS__DATE_SIZE);
    LOG_VAR(RATINGS__SIZE);
    
    const uint data_size = getItemPos(N) + (uint)1;
    
    LOG(INFO) << "Now trying to allocate " << data_size
        << "B of memory (" << data_size * (uint)(8) << "b)";
    _data = (char*)malloc((int)data_size);
    if (_data == NULL) {
        LOG(ERROR) << "Unable to allocate memory";
    }
    else {
        LOG(INFO) << "Memory successfully allocated " << _data;
    }
}

Ratings::~Ratings()
{
    LOG(INFO) << "Killing a Ratings [" << this << "]. Freeing memory...";
    free(_data);
}

uint Ratings::getItemPos(const uint &i) const
{
    return i * (uint)(RATINGS__SIZE) / (uint)(8);
}

uint Ratings::getItemOffset(const uint &i) const
{
    return (i * (uint)(RATINGS__SIZE)) % (uint)(8);
}



/*void Ratings::set(const Rating &rating)
{
    
}
*/
/*Rating Ratings::get(const uint &i) const
{
    const uint main_pos = getItemPos(i);
    const uint offset = getItemOffset(i);    
}*/