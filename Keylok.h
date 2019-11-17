#pragma once

//#include <utility>
#include <memory>


namespace boost {
namespace interprocess {
template <typename T>
class scoped_lock;
//class file_lock;
class named_mutex;
}
}

struct LockedKeylok;

struct Keylok
{
    typedef boost::interprocess::named_mutex InterprocessMutex;
    typedef boost::interprocess::scoped_lock<InterprocessMutex> InterprocessLock;
    typedef std::unique_ptr<InterprocessMutex> PMutex;
    typedef std::unique_ptr<InterprocessLock> PLock;
    //typedef std::pair<PMutex, PLock> MutexLockPair;

    struct IpSync
    {
        IpSync() = default;
        IpSync(IpSync&& other) noexcept;
        IpSync& operator=(IpSync&& other) noexcept;
        ~IpSync();

        PMutex ipMutex;
        PLock ipLock;
    };

    static LockedKeylok getInstance();

private:
    //static MutexLockPair getMutexLockPair();
    static IpSync getIpSync();
};

struct LockedKeylok
{
    //LockedKeylok(Keylok::MutexLockPair&& mutexLockPair, Keylok* keylokInstance) noexcept;
    LockedKeylok(Keylok::IpSync&& ipSync, Keylok* keylokInstance) noexcept;
    LockedKeylok(LockedKeylok&& other) noexcept;
    ~LockedKeylok();
    //Keylok* operator->();
    //explicit operator bool();
private:
    //Keylok::MutexLockPair mutexLockPair;
    Keylok::IpSync ipSync;
    Keylok* keylokInstance;
};

