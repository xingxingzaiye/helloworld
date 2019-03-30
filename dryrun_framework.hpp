#include <iostream>
#include <map>
#include <boost/variant.hpp>

using namespace std;

enum class ProcedurePolicyAction
{
    process,
    ignore,
    queue
};

enum class IncomingEvent
{
    e1SetupReq,
	e1ResetReq,
    f1SetupReq,
	f1ResetReq
};

enum class Procedure
{
    none,
    e1Setup,
	e1Reset,
    f1Setup,
	f1Reset
};

using IncomingEventToProcedureMap = std::map<IncomingEvent, Procedure>;
using ConcurrentProcedure = std::pair<Procedure, IncomingEvent>;
using ProcedurePolicy = std::map<ConcurrentProcedure, ProcedurePolicyAction>;

template<typename TProcess, typename TIgnore>
class ConcurrentHandlingFramework
{
public:
	ConcurrentHandlingFramework(IncomingEventToProcedureMap& map,
	                            ProcedurePolicy& policy) : 
								incomingEventToProcedureMap(map), 
								concurrentPolicy(policy)
	{}

    ProcedurePolicyAction getPolicyAction(IncomingEvent event)
    {
        if(Procedure::none == activeProcedure)
        {
      	    const auto procedureIterator = incomingEventToProcedureMap.find(event);
		    if (procedureIterator != incomingEventToProcedureMap.end())
		    {
			    activeProcedure = procedureIterator->second;				
       	        cout << "activeProcedure: " << (int)activeProcedure << endl;
       	        return ProcedurePolicyAction::process;
		    }
       	    return ProcedurePolicyAction::ignore;
        } 
        else
        {
       	    const auto actionIterator = concurrentPolicy.find(std::make_pair(activeProcedure, event));
		    if (actionIterator != concurrentPolicy.end())
            {
       	      cout << "action: " << (int)actionIterator->second << endl;
              return actionIterator->second;
            }
            return ProcedurePolicyAction::ignore;
        }
    };
	
    template <typename TMessage>
    void handleParallelProcedureEvent(TMessage message, IncomingEvent event)
    {
    	ProcedurePolicyAction action = getPolicyAction(event);

    	switch(action)
		{
		    case ProcedurePolicyAction::process:
    		    boost::apply_visitor(process, message);
				break;
			case ProcedurePolicyAction::ignore:
                boost::apply_visitor(ignore, message);
				break;
			default:
			    break;
    	}
    };

    TProcess process{};
	TIgnore ignore{};

    Procedure activeProcedure = Procedure::none;
    IncomingEventToProcedureMap incomingEventToProcedureMap;
    ProcedurePolicy concurrentPolicy;
};


