#include <glog/logging.h>

#include <string>
#include <sys/shm.h>
//#include <sys/ipc.h>
#include <errno.h>

#include "ShmSegment.h"

NFP::ShmSegment::ShmSegment() :
    keyFileName_(""),
    key_(-1),
    shmid_(-1),
    size_(0),
    ptr_(NULL)
{}

NFP::ShmSegment::ShmSegment(std::string keyFileName, size_t size):
    keyFileName_(keyFileName),
    key_(-1),
    shmid_(-1),
    size_(size),
    ptr_(NULL)
{}

NFP::ShmSegment::~ShmSegment()
{
    detach();
}

int NFP::ShmSegment::create()
{
    int ret = 0;
    key_ = ftok(keyFileName_.c_str(), 'R');
    if (size_ == 0 || key_ == -1) {
        LOG(ERROR) << "Cannot create ShmSegment with "
            << "Size: " << size_ << " "
            << "Key: " << key_
            << ". Are you sure the shmkey file exists?";
        ret++;
    } else {
        shmid_ = shmget(key_, size_, 0600 | IPC_CREAT);
        if (shmid_ == -1) {
            LOG(ERROR) << "Segment creation failed - "
                << strerror(errno) << " [" << errno << "]";
            LOG(ERROR) << info();
            ret++;
        }
        else
            DLOG(INFO) << "ShmSegment created.";
    }
    return ret;
}

int NFP::ShmSegment::attach()
{
    int ret = 0;
    if (shmid_ != -1) {
        ptr_ = shmat(shmid_, (void *)0, 0);
        if ((char*)(ptr_) == (char*)(-1)) {
            LOG(ERROR) << "Shm attach failed for " << info();
            ret++;
        }
        DLOG(INFO) << "ShmSegment attached: " << ptr_;
    } else
        LOG(ERROR) << "Attach failed. Perhaps the segment has not been created.";
    return ret;
}

int NFP::ShmSegment::detach()
{
    int ret = 0;
    if(ptr_) {
        if (shmdt(ptr_)) {
            LOG(ERROR) << "Shm detach failed." << info();
            ret++;
        }
        else {
            ptr_ = NULL;
            DLOG(INFO) << "ShmSegment detached.";
        }
    }/* else {
        LOG(ERROR) << "Cannot detach: ptr_ == NULL.";
        LOG(ERROR) << info();
    }*/
    return ret;
}

int NFP::ShmSegment::remove()
{
    detach();
    int ret = shmctl(shmid_, IPC_RMID, NULL);
    if (ret == 0) {
        size_ = 0;
        shmid_ = -1;
            DLOG(INFO) << "ShmSegment removed.";
    } else {
        LOG(ERROR) << "Failed to remove ShmSegment. " << ret << " - "
            << strerror(errno) << " [" << errno << "]";
        LOG(ERROR) << info();
    }
    return ret;
}

std::string NFP::ShmSegment::info()
{
    char* msg = new char[255];
    sprintf(msg, "%s shmid: %8d shmkey: 0x%x size: %8d",
        keyFileName_.c_str(),
        (int)shmid_,
        (int)key_,
        (int)size_);
    return std::string(msg);
    delete[] msg;
}

