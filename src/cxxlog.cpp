#include <utility>
#include "cxxlog.hpp"

using namespace th_util;

static std::vector<std::pair<tstring, std::shared_ptr<Log>>> __loggers__;
static std::shared_ptr<Log> __logger__;

void Log::set_log_file(const tstring &file_name)
{
    auto ptr = new Log(file_name);
    __logger__ = std::shared_ptr<Log>(ptr);
}

std::shared_ptr<Log> Log::get_logger()
{
    return __logger__;
}

std::shared_ptr<Log> Log::get(const tstring &file_name)
{
#ifdef _LOG_LOCK
    lock_guard<std::mutex> lock(__get_logger_mtx__);
#endif

    std::pair<tstring, std::shared_ptr<Log>> mkv;
    if (std::any_of(__loggers__.begin(),
                    __loggers__.end(),
                    [file_name, &mkv](std::pair<tstring, std::shared_ptr<Log>> &kv) {
                        mkv = kv;
                        return kv.first.compare(file_name) == 0;
                    }))
    {
        return mkv.second;
    }

    auto ptr = new Log(file_name);
    std::shared_ptr<Log> p(ptr);
    mkv = {file_name, p};
    __loggers__.push_back(mkv);

    return mkv.second;
}

std::shared_ptr<Log> Log::get(const std::ostream &os)
{
    auto ptr = new Log(os);
    std::shared_ptr<Log> p(ptr);
    return p;
}

Log::Log(const tstring &file_name)
{
#ifdef _LOG_LOCK
    std::lock_guard<std::mutex> lock(mtx);
#endif
    os = std::make_shared<std::ofstream>(file_name, std::ofstream::out | std::ofstream::app);
    os->setf(std::ios_base::boolalpha);
}

Log::Log(const std::ostream &o)
{
    os = std::make_shared<std::ostream>(o.rdbuf());
}

Log::~Log()
{
    os->flush();
    auto ofs = dynamic_cast<std::ofstream *>(os.get());
    if (ofs)
    {
        ofs->close();
    }
    os = nullptr;

    // TODO: remove it from __loggers__, remove __logger__, via event.
}

Log &Log::operator<<(std::ostream &(*endl)(std::ostream &))
{
    *os << endl;
    return *this;
}
