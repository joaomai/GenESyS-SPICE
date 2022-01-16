/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Smart_ProcessSet.cpp
 * Author: rlcancian
 * 
 * Created on 3 de Setembro de 2019, 18:34
 */

#include "Smart_ProcessSet.h"

// you have to included need libs

// GEnSyS Simulator
#include "../../kernel/simulator/Simulator.h"

// Model Components
#include "../../plugins/components/Create.h"
#include "../../plugins/components/Process.h"
#include "../../plugins/components/Dispose.h"

// Model elements
#include "../../kernel/simulator/EntityType.h"

Smart_ProcessSet::Smart_ProcessSet() {
}

/**
 * This is the main function of the application. 
 * It instanciates the simulator, builds a simulation model and then simulate that model.
 */
int Smart_ProcessSet::main(int argc, char** argv) {
	Simulator* genesys = new Simulator();
	this->insertFakePluginsByHand(genesys);
	this->setDefaultTraceHandlers(genesys->getTracer());
	genesys->getTracer()->setTraceLevel(Util::TraceLevel::L8_detailed);
	Model* model = genesys->getModels()->newModel();
	Create *create = new Create(model);
	create->setEntityType(new EntityType(model, "Client"));
	create->setTimeBetweenCreationsExpression("1");
	Set* set = new Set(model);
	set->setSetOfType("Resource");
	set->getElementSet()->insert(new Resource(model));
	set->getElementSet()->insert(new Resource(model));
	set->getElementSet()->insert(new Resource(model));
	set->getElementSet()->insert(new Resource(model));
	set->getElementSet()->insert(new Resource(model));
	Process* process = new Process(model);
	process->getSeizeRequests()->insert(new SeizableItem(set)); //, SeizableItem::SeizableType::SET));
	process->setQueueableItem(new QueueableItem(new Queue(model)));
	process->setDelayExpression("unif(0.8,1.2)");
	Dispose* dispose = new Dispose(model);
	create->getConnections()->insert(process);
	process->getConnections()->insert(dispose);
	model->getSimulation()->setReplicationLength(10);
	model->save("./models/Smart_ProcessSet.gen");
	do {
		model->getSimulation()->step();
		//		std::cin.ignore(std::numeric_limits <std::streamsize> ::max(), '\n');
	} while (model->getSimulation()->isPaused());
	genesys->~Simulator();
	return 0;
};
