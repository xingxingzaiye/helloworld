#include "dryrun_framework.hpp"
#include "dryrun_e1service.hpp"
#include "dryrun_f1service.hpp"

int main() {
    //E1 service initialize its own incomingEventToProcedure map and concurrent procedurePolicy map
	IncomingEventToProcedureMap e1map = {{IncomingEvent::e1SetupReq,Procedure::e1Setup},
	                                     {IncomingEvent::e1ResetReq,Procedure::e1Reset}};
	ProcedurePolicy e1Policy = {{std::make_pair(Procedure::e1Setup, IncomingEvent::e1ResetReq), ProcedurePolicyAction::ignore}};
	
	E1SetupReq e1setupreq;
    E1ResetReq e1resetreq;
	//specialize E1 process and ignore visitors 
	ConcurrentHandlingFramework<E1_Process, E1_Ignore> e1{e1map, e1Policy};
	//no ongoing procedure, e1setupreq trigger e1setup procedure
	e1.handleParallelProcedureEvent(VarE1Message{e1setupreq}, IncomingEvent::e1SetupReq);
	//e1setup procedure ongoing, e1resetreq ignore
	e1.handleParallelProcedureEvent(VarE1Message{e1resetreq}, IncomingEvent::e1ResetReq);

	//F1 service initialize its own incomingEventToProcedure map and concurrent procedurePolicy map
	IncomingEventToProcedureMap f1map = {{IncomingEvent::f1SetupReq,Procedure::f1Setup}};
	ProcedurePolicy f1Policy = {{std::make_pair(Procedure::f1Setup, IncomingEvent::f1ResetReq), ProcedurePolicyAction::ignore}};	
	F1SetupReq f1setupreq;
	//specialize F1 process and ignore visitors 
	ConcurrentHandlingFramework<F1_Process, F1_Ignore> f1{f1map, f1Policy};
	//no ongoing procedure, f1setupreq trigger f1setup procedure
	f1.handleParallelProcedureEvent(VarF1Message{f1setupreq}, IncomingEvent::f1SetupReq);

	return 0;
}

