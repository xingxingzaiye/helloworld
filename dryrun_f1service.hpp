#include <boost/variant.hpp>

using namespace std;

struct F1SetupReq
{
    int head;
};

struct F1ResetReq
{
    int head;
};

using F1ProcessableMessage = boost::variant<F1SetupReq, F1ResetReq>;

class F1_Process : public boost::static_visitor<void>
{
public:
    template <typename T> void operator()(T& p_t) { process(p_t); }
    void process(F1SetupReq) {std::cout << " process f1SetupReq" << endl;}
    void process(F1ResetReq) {std::cout << " process f1ResetReq" << endl;}
};

class F1_Ignore : public boost::static_visitor<void>
{
public:
    template <typename T> void operator()(T& p_t) { ignore(p_t); }
    void ignore(F1SetupReq) {std::cout << "ignore F1SetupReq" << endl;}
	void ignore(F1ResetReq) {std::cout << "ignore F1ResetReq" << endl;}

};
