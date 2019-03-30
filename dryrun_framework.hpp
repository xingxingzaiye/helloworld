#include <iostream>
#include <map>
#include <queue>
#include <boost/variant.hpp>

using namespace std;

enum class ProcedurePolicyAction
{
    process,
    ignore,
    queue
};

enum class IncomingMessageName
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

using IncomingEventToProcedureMap = std::map<IncomingMessageName, Procedure>;
using ConcurrentProcedure = std::pair<Procedure, IncomingMessageName>;
using ProcedurePolicy = std::map<ConcurrentProcedure, ProcedurePolicyAction>;

template<typename TProcess, typename TIgnore, typename TMessage>
class ConcurrentHandlingFramework
{
public:
	ConcurrentHandlingFramework(IncomingEventToProcedureMap& map,
	                            ProcedurePolicy& policy) : 
								incomingEventToProcedureMap(map), 
								concurrentPolicy(policy)
	{}

    ProcedurePolicyAction getPolicyAction(IncomingMessageName messageName)
    {
        if(Procedure::none == activeProcedure)
        {
      	    const auto procedureIterator = incomingEventToProcedureMap.find(messageName);
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
       	    const auto actionIterator = concurrentPolicy.find(std::make_pair(activeProcedure, messageName));
		    if (actionIterator != concurrentPolicy.end())
            {
       	      cout << "action: " << (int)actionIterator->second << endl;
              return actionIterator->second;
            }
            return ProcedurePolicyAction::ignore;
        }
    };
	
    //template <typename TMessage>
    void handleParallelProcedureEvent(TMessage message, IncomingMessageName messageName)
    {
    	ProcedurePolicyAction action = getPolicyAction(messageName);

    	switch(action)
		{
		    case ProcedurePolicyAction::process:
    		    boost::apply_visitor(process, message);
				break;
			case ProcedurePolicyAction::ignore:
                boost::apply_visitor(ignore, message);
				break;
		    case ProcedurePolicyAction::queue:
			    cout << "queue " << endl;
			    parallelProcedureQueue.push(std::make_pair(message,messageName));
			default:
			    break;
    	}
    };

	void markProcedureComplete()
	{
		activeProcedure = Procedure::none;
		if (not parallelProcedureQueue.empty())
        {
            auto queuedEvent = parallelProcedureQueue.front();
            parallelProcedureQueue.pop();
            handleParallelProcedureEvent(queuedEvent.first, queuedEvent.second);
        }
	};
	
    TProcess process{};
	TIgnore ignore{};

    Procedure activeProcedure = Procedure::none;
    IncomingEventToProcedureMap incomingEventToProcedureMap;
    ProcedurePolicy concurrentPolicy;
	
	std::queue<std::pair<TMessage,IncomingMessageName>> parallelProcedureQueue;
};


