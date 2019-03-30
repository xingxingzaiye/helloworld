#include <boost/variant.hpp>

struct E1SetupReq
{
    int head;
};

struct E1ResetReq
{
    int head;
};

using VarE1Message = boost::variant<E1SetupReq, E1ResetReq>;

class E1_Process : public boost::static_visitor<void>
{
public:
    template <typename T> void operator()(T& p_t) { process(p_t); }
    void process(E1SetupReq) {std::cout << "e1SetupReq" << endl;}
    void process(E1ResetReq) {std::cout << "e1ResetReq" << endl;}
};

class E1_Ignore : public boost::static_visitor<void>
{
public:
    template <typename T> void operator()(T& p_t) { ignore(p_t); }
    void ignore(E1SetupReq) {std::cout << "ignore e1SetupReq" << endl;}
    void ignore(E1ResetReq) {std::cout << "ignore e1ResetReq" << endl;}
};
