#include "dryrun_framework.hpp"
#include "dryrun_e1service.hpp"
#include "dryrun_f1service.hpp"

int main() {
    //E1 service initialize its own incomingEventToProcedure map and concurrent procedurePolicy map
	IncomingEventToProcedureMap e1map = {{IncomingMessageName::e1SetupReq,Procedure::e1Setup},
	                                     {IncomingMessageName::e1ResetReq,Procedure::e1Reset}};
	ProcedurePolicy e1Policy = {{std::make_pair(Procedure::e1Setup, IncomingMessageName::e1ResetReq), ProcedurePolicyAction::ignore}};
	
	E1SetupReq e1setupreq;
    E1ResetReq e1resetreq;
	//specialize E1 process and ignore visitors 
	ConcurrentHandlingFramework<E1_Process, E1_Ignore, E1ProcessableMessage> e1{e1map, e1Policy};
	//no ongoing procedure, e1setupreq trigger e1setup procedure
	e1.handleParallelProcedureEvent(E1ProcessableMessage{e1setupreq}, IncomingMessageName::e1SetupReq);
	//e1setup procedure ongoing, e1resetreq ignore
	e1.handleParallelProcedureEvent(E1ProcessableMessage{e1resetreq}, IncomingMessageName::e1ResetReq);

	//F1 service initialize its own incomingEventToProcedure map and concurrent procedurePolicy map
	IncomingEventToProcedureMap f1map = {{IncomingMessageName::f1SetupReq,Procedure::f1Setup},
                                         {IncomingMessageName::f1ResetReq,Procedure::f1Reset}};
	ProcedurePolicy f1Policy = {{std::make_pair(Procedure::f1Setup, IncomingMessageName::f1ResetReq), ProcedurePolicyAction::queue}};	
	F1SetupReq f1setupreq;
	F1ResetReq f1resetreq;
	//specialize F1 process and ignore visitors 
	ConcurrentHandlingFramework<F1_Process, F1_Ignore, F1ProcessableMessage> f1{f1map, f1Policy};
	//no ongoing procedure, f1setupreq trigger f1setup procedure
	f1.handleParallelProcedureEvent(F1ProcessableMessage{f1setupreq}, IncomingMessageName::f1SetupReq);
	f1.handleParallelProcedureEvent(F1ProcessableMessage{f1resetreq}, IncomingMessageName::f1ResetReq);

	return 0;
}

