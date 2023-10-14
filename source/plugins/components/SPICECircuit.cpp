/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   SPICECircuit.h
 * Author: bnmfw & joaomai
 *
 * Created on 10 de outubro de 2023, 23:00
 */

#include "SPICECircuit.h"
#include "../../kernel/simulator/Model.h"



/// Externalize function GetPluginInformation to be accessible throught dynamic linked library
#ifdef PLUGINCONNECT_DYNAMIC
extern "C" StaticGetPluginInformation GetPluginInformation() {
	return &SPICECircuit::GetPluginInformation;
}
#endif

std::vector<std::string> split(std::string word, char split_char = ' '){
    std::vector<std::string> tokenized_string;
    std::string token = "";
    for (char character: word){
        if (character == split_char){
            if (token.size()) tokenized_string.push_back(token);
            token = "";
            continue;
        }
        token += character;
    }
    if (token.size()) tokenized_string.push_back(token);
    return tokenized_string;
}

unsigned int SPICECircuit::counter = 0;
unsigned int Resistor::counter = 0;
unsigned int Capacitor::counter = 0;
unsigned int Vpulse::counter = 0;
unsigned int Vsource::counter = 0;
unsigned int Vsine::counter = 0;
unsigned int Diode::counter = 0;
unsigned int PMOS::counter = 0;
unsigned int NMOS::counter = 0;
unsigned int NAND::counter = 0;
unsigned int AND::counter = 0;
unsigned int NOR::counter = 0;
unsigned int OR::counter = 0;
unsigned int XOR::counter = 0;
unsigned int XNOR::counter = 0;

//
// public: /// constructors
//

SPICECircuit::SPICECircuit(Model* model, std::string name)
: ModelComponent(model, Util::TypeOf<SPICECircuit>(), name) { }


//
// public: /// new public user methods for this component
//

void SPICECircuit::init(SPICERunner* the_compiler, std::string description, unsigned int id, std::vector<std::string> the_params, std::string the_model, std::string the_model_file) {
	compiler = the_compiler;
	params = the_params;
	model = the_model;
	model_file = the_model_file;
	spice_template = description;
	std::vector<std::string> lines = split(description,'\n');
	spice_instance = new std::string;
	plain_circuit = (lines.size() == 1);
	// Parses the description to get the circuit name and interface nodes

	// Subcircuit declaration
	if (!plain_circuit) {
		for (std::string line: lines){
			std::vector<std::string> tokens = split(line);
			if (tokens[0] != ".subckt") continue;
			spice_name = tokens[1];
			tokens.erase(tokens.begin());
			tokens.erase(tokens.begin());
			for (std::string token: tokens)
				pins.push_back("nullpin");
			break;
		}
		//Generates the circuit instance
		*spice_instance = "x"+spice_name+std::to_string(id)+" ";
		for (std::string pin: pins) *spice_instance += pin+" ";
		*spice_instance += spice_name;
		//Sends instance and subcircuit template to compiler
		compiler->SendComponent(spice_instance, spice_template, model_file);

	// Plain circuit declaration
	} else {
		std::vector<std::string> tokens = split(lines[0]);
		spice_name = tokens[0];
		*spice_instance = spice_name+std::to_string(id)+" ";
		tokens.erase(tokens.begin());
		for (std::string token: tokens) pins.push_back("nullpin");
		for (std::string pin: pins) *spice_instance += pin + " ";
		if (model.size()) *spice_instance += model + " ";
		for (std::string param: params) *spice_instance += param + " ";
		// std::cout << "im sending this model to compile: " << model << "}" << std::endl;
		compiler->SendComponent(spice_instance, "", model_file);
	}
}

void SPICECircuit::insertAtRank(int pin, SPICENode* node) {
    pins[pin] = node->getNodeName();
        
    if (!plain_circuit) *spice_instance = "x"+spice_name+std::to_string(id)+" ";
    else *spice_instance = spice_name+std::to_string(id)+" ";
    
    for (std::string pin: pins) *spice_instance += pin+" ";
    if (plain_circuit && model.size()) *spice_instance += model + " ";
    
    if (!plain_circuit) *spice_instance += spice_name;
    else for (std::string param: params) *spice_instance += param + " ";
}

SPICERunner* SPICECircuit::getCompiler() {
    return compiler;
}

//
// public: /// virtual methods
//

std::string SPICECircuit::show() {
	return ModelComponent::show() + "";
}


//
// public: /// static methods that must have implementations (Load and New just the same. GetInformation must provide specific infos for the new component
//

PluginInformation* SPICECircuit::GetPluginInformation() {
	PluginInformation* info = new PluginInformation(Util::TypeOf<SPICECircuit>(), &SPICECircuit::LoadInstance, &SPICECircuit::NewInstance);
	info->setCategory("Electronics simulation");
	return info;
}

PluginInformation* Resistor::GetPluginInformation() {
	PluginInformation* info = new PluginInformation(Util::TypeOf<Resistor>(), &Resistor::LoadInstance, &Resistor::NewInstance);
	info->setCategory("Electronics simulation");
	return info;
}

PluginInformation* Vsource::GetPluginInformation() {
	PluginInformation* info = new PluginInformation(Util::TypeOf<Vsource>(), &Vsource::LoadInstance, &Vsource::NewInstance);
	info->setCategory("Electronics simulation");
	return info;
}

PluginInformation* Vpulse::GetPluginInformation() {
	PluginInformation* info = new PluginInformation(Util::TypeOf<Vpulse>(), &Vpulse::LoadInstance, &Vpulse::NewInstance);
	info->setCategory("Electronics simulation");
	return info;
}

PluginInformation* Vsine::GetPluginInformation() {
	PluginInformation* info = new PluginInformation(Util::TypeOf<Vsine>(), &Vsine::LoadInstance, &Vsine::NewInstance);
	info->setCategory("Electronics simulation");
	return info;
}

PluginInformation* Capacitor::GetPluginInformation() {
	PluginInformation* info = new PluginInformation(Util::TypeOf<Capacitor>(), &Capacitor::LoadInstance, &Capacitor::NewInstance);
	info->setCategory("Electronics simulation");
	return info;
}

PluginInformation* Diode::GetPluginInformation() {
	PluginInformation* info = new PluginInformation(Util::TypeOf<Diode>(), &Diode::LoadInstance, &Diode::NewInstance);
	info->setCategory("Electronics simulation");
	return info;
}

PluginInformation* PMOS::GetPluginInformation() {
	PluginInformation* info = new PluginInformation(Util::TypeOf<PMOS>(), &PMOS::LoadInstance, &PMOS::NewInstance);
	info->setCategory("Electronics simulation");
	return info;
}

PluginInformation* NMOS::GetPluginInformation() {
	PluginInformation* info = new PluginInformation(Util::TypeOf<NMOS>(), &NMOS::LoadInstance, &NMOS::NewInstance);
	info->setCategory("Electronics simulation");
	return info;
}

PluginInformation* NOT::GetPluginInformation() {
	PluginInformation* info = new PluginInformation(Util::TypeOf<NOT>(), &NOT::LoadInstance, &NOT::NewInstance);
	info->setCategory("Electronics simulation");
	return info;
}

PluginInformation* NAND::GetPluginInformation() {
	PluginInformation* info = new PluginInformation(Util::TypeOf<NAND>(), &NAND::LoadInstance, &NAND::NewInstance);
	info->setCategory("Electronics simulation");
	return info;
}

PluginInformation* AND::GetPluginInformation() {
	PluginInformation* info = new PluginInformation(Util::TypeOf<AND>(), &AND::LoadInstance, &AND::NewInstance);
	info->setCategory("Electronics simulation");
	return info;
}

PluginInformation* NOR::GetPluginInformation() {
	PluginInformation* info = new PluginInformation(Util::TypeOf<NOR>(), &NOR::LoadInstance, &NOR::NewInstance);
	info->setCategory("Electronics simulation");
	return info;
}

PluginInformation* OR::GetPluginInformation() {
	PluginInformation* info = new PluginInformation(Util::TypeOf<OR>(), &OR::LoadInstance, &OR::NewInstance);
	info->setCategory("Electronics simulation");
	return info;
}

PluginInformation* XOR::GetPluginInformation() {
	PluginInformation* info = new PluginInformation(Util::TypeOf<XOR>(), &XOR::LoadInstance, &XOR::NewInstance);
	info->setCategory("Electronics simulation");
	return info;
}

PluginInformation* XNOR::GetPluginInformation() {
	PluginInformation* info = new PluginInformation(Util::TypeOf<XNOR>(), &XNOR::LoadInstance, &XNOR::NewInstance);
	info->setCategory("Electronics simulation");
	return info;
}

ModelComponent* SPICECircuit::LoadInstance(Model* model, PersistenceRecord *fields) {
	SPICECircuit* newComponent = new SPICECircuit(model);
	try {
		newComponent->_loadInstance(fields);
	} catch (const std::exception& e) {

	}
	return newComponent;
}

ModelDataDefinition* SPICECircuit::NewInstance(Model* model, std::string name) {
	return new SPICECircuit(model, name);
}

//
// protected: /// virtual method that must be overriden
//

bool SPICECircuit::_loadInstance(PersistenceRecord *fields) {
	bool res = ModelComponent::_loadInstance(fields);
	if (res) {
		// @TODO: not implemented yet
	}
	return res;
}

void SPICECircuit::_saveInstance(PersistenceRecord *fields, bool saveDefaultValues) {
	ModelComponent::_saveInstance(fields, saveDefaultValues);
	// @TODO: not implemented yet
}

void SPICECircuit::_onDispatchEvent(Entity* entity, unsigned int inputPortNumber) {
	_parentModel->getTracer()->traceSimulation(this, "I'm just an electric circuit model and I'm not event oriented.");
	this->_parentModel->sendEntityToComponent(entity, this->getConnections()->getFrontConnection());
}
