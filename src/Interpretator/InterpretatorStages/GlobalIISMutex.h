#pragma once
#ifdef MULTIUSERS
#include <mutex>

std::mutex globalMutex;
#endif // MULTIUSERS
