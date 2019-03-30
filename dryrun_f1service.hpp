#include <boost/variant.hpp>

using namespace std;

struct F1SetupReq
{
    int head;
};
using VarF1Message = boost::variant<F1SetupReq>;

class F1_Process : public boost::static_visitor<void>
{
public:
    template <typename T> void operator()(T& p_t) { process(p_t); }
    void process(F1SetupReq) {std::cout << "f1SetupReq" << endl;}
};

class F1_Ignore : public boost::static_visitor<void>
{
public:
    template <typename T> void operator()(T& p_t) { ignore(p_t); }
    void ignore(F1SetupReq) {std::cout << "ignore F1SetupReq" << endl;}
};
