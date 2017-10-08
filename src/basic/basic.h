#ifndef BASIC_H
#define BASIC_H
#include <QString>
#include <exception>
#include <memory>

class IOError: public std::runtime_error {
public:
    IOError(const std::string& mes) :std::runtime_error(mes) { }
};

void errorExit(const char *str, const char *arg);
void throwRuntimeError(const char *str, const char *arg);
void throwIOError(const char *str, const char *arg);
void showCriticalMessage(const char *str, const char *arg);
void showCriticalMessage(QString);


//NOTE: delete for up-to-date compilers

namespace std {
template<typename T, typename ...Ts>
std::unique_ptr<T> make_unique(Ts&&...args) {
    return std::unique_ptr<T>(new T (std::forward<Ts>(args)...));
}
}



template <typename T>
class DoInDestructor {
public:
    DoInDestructor(T &f): m_f(f) { }
    ~DoInDestructor() { m_f(); }
private:
    T &m_f;
};

#define TOKEN_PASTE_(a, b)  a ## b
#define TOKEN_PASTE(a, b) TOKEN_PASTE_(a,b)

#define DO_AT_SCOPE_EXIT_GUTS_2(lambda_name, raii_object_name, ...) \
        auto lambda_name = [&] () { __VA_ARGS__;}; \
        DoInDestructor<decltype(lambda_name)> raii_object_name(lambda_name);

#define DO_AT_SCOPE_EXIT_GUTS_1(counter, ...) DO_AT_SCOPE_EXIT_GUTS_2(TOKEN_PASTE(callable_lambda_, counter),\
                                                               TOKEN_PASTE(raii_object_, counter),\
                                                                __VA_ARGS__)

#define DoAtScopeExit(...)  DO_AT_SCOPE_EXIT_GUTS_1(__COUNTER__, __VA_ARGS__)








#endif // BASIC_H
