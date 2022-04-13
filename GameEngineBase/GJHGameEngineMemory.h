#pragma once
#include <memory>

template<typename T>
using SharedPtrThis = std::enable_shared_from_this<T>;

template<typename T>
using SharedPtr = std::shared_ptr<T>;