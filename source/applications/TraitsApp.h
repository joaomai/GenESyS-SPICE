/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   TraitsApp.h
 * Author: rafael.luiz.cancian
 *
 * Created on 14 de Agosto de 2018, 19:36
 */

#ifndef TRAITSAPP_H
#define TRAITSAPP_H
#include "BaseGenesysTerminalApplication.h"

// application to compile
// #include "terminal/examples/spice/Half_Adder.h"
#include "terminal/examples/spice/Rectifier.h"


template <typename T>
struct TraitsApp {
};

/*!
 *  Configure the Genesys Application to be compiled and executed
 */
template <> struct TraitsApp<GenesysApplication_if> {
	//static const bool runTests = false;
	static const TraceManager::Level traceLevel = TraceManager::Level::L9_mostDetailed;
	static const bool runGraphicalUserInterface = false;

	//// SPICES
	// typedef Half_Adder Application;
	typedef Rectifier Application;

	//// SMART SMARTs ALPHA SORTED
	//typedef Smart_AssignWriteSeizes Application;
	//typedef Smart_BatchSeparate Application;
	//typedef Smart_Clone Application;
	//typedef Smart_CppForG Application;
	//typedef Smart_Delay Application;
	//typedef Smart_Dummy Application;
	//typedef Smart_Failures Application;
	//typedef Smart_FiniteStateMachine Application;
	//typedef Smart_HoldSearchRemove Application;
	//typedef Smart_ModelInfoModelSimulation Application;
	//typedef Smart_ODE Application;
	//typedef Smart_OnEvent Application;
	//typedef Smart_Parser Application;
	//typedef Smart_ParserModelFunctions Application;
	//typedef Smart_Plugin Application;
	//typedef Smart_Process Application;
	//typedef Smart_ProcessSet Application;
	//typedef Smart_RouteStation Application;
	// typedef Smart_SeizeDelayRelease Application;
	//typedef Smart_SeizeDelayReleaseNoDataDefs Application;
	//typedef Smart_SeizeDelayReleaseMany Application;
	//typedef Smart_Sequence Application;
	//typedef Smart_SynchronizingParallelEntities Application;
	//typedef Smart_SimulationControlResponse Application;
	//typedef Smart_WaitScanCondition Application;
	//typedef Smart_WaitSignal Application;

	//// ARENA SMARTS
	//typedef Smart_AddingResource Application;
	//typedef Smart_AlternatingEntityCreation Application;
	//typedef Smart_ArrivalsElementStopsEntitiesArrivingAfterASetTime Application;
	//typedef Smart_ArrivalsEntityTypeVsAttribute Application;
	//typedef Smart_AssignExample Application;
	//typedef Smart_AutomaticStatisticsCollection Application;
	//typedef Smart_BasicModeling Application;
	//typedef Smart_BatchAndSeparate Application;
	//typedef Smart_ContinuousFlowEntities Application;
	//typedef Smart_Create Application;
	//typedef Smart_DecideNWayByChance Application;
	//typedef Smart_DefiningAttributesAsStrings Application;
	//typedef Smart_DefiningControlLogic Application;
	//typedef Smart_DefiningResourceCapacity Application;
	//typedef Smart_DelayBasedOnReplication Application;
	//typedef Smart_EntitiesProcessedByPriority Application;
	//typedef Smart_EvaluatingConditionsBeforeEnteringQueue Application;
	//typedef Smart_Expression Application;
	//typedef Smart_InventoryAndHoldingCosts Application;
	//typedef Smart_MaxArrivalsField Application;
	//typedef Smart_ModelRunUntil1000Parts Application;
	//typedef Smart_ModuleDisplayVariables Application;
	//typedef Smart_OverlappingResources Application;
	//typedef Smart_ParallelProcessingOfEntities Application;
	//typedef Smart_PlacingEntitiesInQueueSets Application;
	//typedef Smart_PriorityExample Application;
	//typedef Smart_ProcessArena Application;
	//typedef Smart_Record Application;
	//typedef Smart_RemovingAndReorderingEntitiesInAQueue Application;
	//typedef Smart_ResourceCosting Application;
	//typedef Smart_ResourceScheduleCosting Application;
	//typedef Smart_ResourceSets Application;
	//typedef Smart_SeizingMultipleSimultaneosly Application;
	//typedef Smart_SelectingRouteBasedOnProbability Application;
	//typedef Smart_SelectingShorterQueue Application;
	//typedef Smart_SynchronizingParallelEntities Application;
	//typedef Smart_ValueAdded Application;
	//typedef Smart_WaitForSignal Application;

	//// ARENA EXAMPLES
	//typedef AirportSecurityExample Application;
	//typedef AirportSecurityExampleExtended Application;
	//typedef Airport_Extended1 Application;
	//typedef Assembly_Line Application;
	//typedef Banking_Transactions Application;
	//typedef BasicOrderShipping Application;
	//typedef Example_PortModel Application;
	//typedef Example_PublicTransport Application;


	//// TEACHING
	//typedef AnElectronicAssemblyAndTestSystem Application;
	//typedef OperatingSystem01 Application;
	//typedef OperatingSystem02 Application;
	//typedef OperatingSystem03 Application;
	//typedef BufferFIFO Application;

	//// BOOK
	//typedef Book_Cap02_Example01 Application;

	//// TERMINAL APPLICATIONS
	//typedef GenesysTerminalApp Application;

	//// UNDER DEVELPMENT
	//typedef TestingTerminalApp Application;
};

#endif /* TRAITSAPP_H */

