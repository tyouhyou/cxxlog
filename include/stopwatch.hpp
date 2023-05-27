/* *
 * stopwatch for performance measuring purpose.
 *
 * @author  tyouhyou    github.com/tyouhyou
 * @license GPL
 * */

#pragma once

#include <chrono>
#include <functional>
#include <memory>
#include <mutex>
#include <unordered_map>

namespace zb
{

    class stopwatch
    {
    public:
        static std::shared_ptr<stopwatch> g_sw()
        {
            static std::shared_ptr<stopwatch> sw;
            static std::mutex mtx;
            std::lock_guard<std::mutex> lock(mtx);
            if (!sw)
            {
                sw = std::make_shared<stopwatch>();
            }
            return sw;
        }

        stopwatch &start()
        {
            mark_list.clear();
            startpoint = std::chrono::high_resolution_clock::now();
            lastpoint = startpoint;
            return *this;
        }

        long long elaspsed()
        {
            return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - startpoint).count();
        }

        long long wrap()
        {
            auto temppoint = std::chrono::high_resolution_clock::now();
            auto dur = std::chrono::duration_cast<std::chrono::microseconds>(temppoint - lastpoint);
            lastpoint = temppoint;
            return dur.count();
        }

        void mark(std::string m)
        {
            mark_list[m] = std::chrono::high_resolution_clock::now();
        }

        long long measure(std::string mark)
        {
            auto temppoint = std::chrono::high_resolution_clock::now();
            auto dur = std::chrono::duration_cast<std::chrono::microseconds>(temppoint - mark_list[mark]);
            return dur.count();
        }

        long long measure(std::string mark1, std::string mark2)
        {
            auto dur = std::chrono::duration_cast<std::chrono::microseconds>(mark_list[mark2] - mark_list[mark1]);
            return dur.count();
        }

        stopwatch &reset()
        {
            return start();
        }

        ~stopwatch()
        {
            mark_list.clear();
        }

    private:
        std::chrono::high_resolution_clock::time_point startpoint;
        std::chrono::high_resolution_clock::time_point lastpoint;
        std::unordered_map<std::string, std::chrono::high_resolution_clock::time_point> mark_list;
    };

} // namespace zb