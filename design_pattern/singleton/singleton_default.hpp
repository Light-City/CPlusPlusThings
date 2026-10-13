/**
 * @file singleton_default.hpp
 * @brief 可复用的线程安全单例模板基类（CRTP）
 * 受 issue #303 启发：https://github.com/Light-City/CPlusPlusThings/issues/303
 * 参考 boost::container::singleton_default 的思路。
 *
 * 用法：
 *   class MyConfig : public SingletonDefault<MyConfig> {
 *       friend class SingletonDefault<MyConfig>;
 *   private:
 *       MyConfig() = default;          // 构造私有，交由基类创建
 *   public:
 *       int value = 42;
 *   };
 *   MyConfig::instance().value = 1;
 *
 * 编译：g++ -std=c++11 main.cpp
 * @author 光城
 * @version v1
 * @date 2026-10
 */

#pragma once

#include <mutex>

template <class T>
class SingletonDefault {
public:
    SingletonDefault(const SingletonDefault&) = delete;
    SingletonDefault& operator=(const SingletonDefault&) = delete;

    static T& instance() {
        // C++11 起，局部静态变量的初始化由标准保证线程安全（"magic statics"）
        static T inst;
        return inst;
    }

protected:
    SingletonDefault() = default;
    ~SingletonDefault() = default;
};

/**
 * 若需要 C++03 或对 magic statics 不放心的环境，可用 call_once 版本：
 *
 * template <class T>
 * class SingletonDefaultOnce {
 * public:
 *     static T& instance() {
 *         std::call_once(once_, [] { inst_.reset(new T); });
 *         return *inst_;
 *     }
 * private:
 *     static std::once_flag once_;
 *     static std::unique_ptr<T> inst_;
 * };
 */
