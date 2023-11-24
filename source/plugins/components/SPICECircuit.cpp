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
unsigned int NOT::counter = 0;
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

void SPICECircuit::BuildCircuit(std::string description, unsigned int id, std::vector<std::string> the_params, std::string the_model, std::string the_model_file) {
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

void SPICECircuit::UpdateConnections() {
	build();

	for (auto [pin, connection] : *getConnections()->connections()) {
		pins[pin] = static_cast<SPICENode*>(connection->component)->getNodeName();

		if (!plain_circuit) *spice_instance = "x"+spice_name+std::to_string(id)+" ";
		else *spice_instance = spice_name+std::to_string(id)+" ";
		
		for (std::string pin : pins) *spice_instance += pin+" ";
		if (plain_circuit && model.size()) *spice_instance += model + " ";
		
		if (!plain_circuit) *spice_instance += spice_name;
		else for (std::string param: params) *spice_instance += param + " ";
	}
}

void SPICECircuit::setRunner(SPICERunner* runner) {
	compiler = runner;

	compiler->SendCallback([this]() {this->UpdateConnections(); });
}

SPICERunner* SPICECircuit::getRunner() {
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

ModelComponent* Resistor::LoadInstance(Model* model, PersistenceRecord *fields) {
	Resistor* newComponent = new Resistor(model);
	try {
		// newComponent->_loadInstance(fields);
	} catch (const std::exception& e) {

	}
	return newComponent;
}

ModelDataDefinition* Resistor::NewInstance(Model* model, std::string name) {
	return new Resistor(model, name);
}


void Resistor::setResistance(float resistance) {
	this->resistance = resistance;
}

void Resistor::Build() {

	SPICECircuit::BuildCircuit("R a b", id, {uc(resistance)});
}

PluginInformation* Vsource::GetPluginInformation() {
	PluginInformation* info = new PluginInformation(Util::TypeOf<Vsource>(), &Vsource::LoadInstance, &Vsource::NewInstance);
	info->setCategory("Electronics simulation");
	return info;
}

ModelComponent* Vsource::LoadInstance(Model* model, PersistenceRecord *fields) {
	Vsource* newComponent = new Vsource(model);
	try {
		// newComponent->_loadInstance(fields);
	} catch (const std::exception& e) {

	}
	return newComponent;
}

ModelDataDefinition* Vsource::NewInstance(Model* model, std::string name) {
	return new Vsource(model, name);
}


void Vsource::setVoltage(float voltage) {
	this->voltage = voltage;
}

void Vsource::Build() {

	SPICECircuit::BuildCircuit("Vd a b", id, {uc(voltage)});
}

PluginInformation* Vpulse::GetPluginInformation() {
	PluginInformation* info = new PluginInformation(Util::TypeOf<Vpulse>(), &Vpulse::LoadInstance, &Vpulse::NewInstance);
	info->setCategory("Electronics simulation");
	return info;
}

ModelComponent* Vpulse::LoadInstance(Model* model, PersistenceRecord *fields) {
	Vpulse* newComponent = new Vpulse(model);
	try {
		// newComponent->_loadInstance(fields);
	} catch (const std::exception& e) {

	}
	return newComponent;
}

ModelDataDefinition* Vpulse::NewInstance(Model* model, std::string name) {
	return new Vpulse(model, name);
}

void Vpulse::setVoltage(float voltage) {
	this->voltage = voltage;
}

void Vpulse::setFreq(float freq) {
	this->freq = freq;
}

void Vpulse::setSlope(float slope) {
	this->slope = slope;
}

void Vpulse::Build() {

	SPICECircuit::BuildCircuit("Vp a b", id, {"PULSE (0 "+uc(voltage)+uc(freq/2)+uc(slope)+uc(slope)+uc(freq/2)+uc(freq)+")"});
}

PluginInformation* Vsine::GetPluginInformation() {
	PluginInformation* info = new PluginInformation(Util::TypeOf<Vsine>(), &Vsine::LoadInstance, &Vsine::NewInstance);
	info->setCategory("Electronics simulation");
	return info;
}

ModelComponent* Vsine::LoadInstance(Model* model, PersistenceRecord *fields) {
	Vsine* newComponent = new Vsine(model);
	try {
		// newComponent->_loadInstance(fields);
	} catch (const std::exception& e) {

	}
	return newComponent;
}

ModelDataDefinition* Vsine::NewInstance(Model* model, std::string name) {
	return new Vsine(model, name);
}


void Vsine::setVoltage(float voltage) {
	this->voltage = voltage;
}

void Vsine::setFreq(float freq) {
	this->freq = freq;
}

void Vsine::Build() {

	SPICECircuit::BuildCircuit("Vs a b", id, {"sin(0 "+uc(voltage)+uc(freq)+")"});
}

PluginInformation* Capacitor::GetPluginInformation() {
	PluginInformation* info = new PluginInformation(Util::TypeOf<Capacitor>(), &Capacitor::LoadInstance, &Capacitor::NewInstance);
	info->setCategory("Electronics simulation");
	return info;
}

ModelComponent* Capacitor::LoadInstance(Model* model, PersistenceRecord *fields) {
	Capacitor* newComponent = new Capacitor(model);
	try {
		// newComponent->_loadInstance(fields);
	} catch (const std::exception& e) {

	}
	return newComponent;
}

ModelDataDefinition* Capacitor::NewInstance(Model* model, std::string name) {
	return new Capacitor(model, name);
}

void Capacitor::setCapacitance(float capacitance) {
	this->capacitance = capacitance;
}

void Capacitor::Build() {

	SPICECircuit::BuildCircuit("C a b", id, {uc(capacitance)});
}

PluginInformation* Diode::GetPluginInformation() {
	PluginInformation* info = new PluginInformation(Util::TypeOf<Diode>(), &Diode::LoadInstance, &Diode::NewInstance);
	info->setCategory("Electronics simulation");
	return info;
}

ModelComponent* Diode::LoadInstance(Model* model, PersistenceRecord *fields) {
	Diode* newComponent = new Diode(model);
	try {
		// newComponent->_loadInstance(fields);
	} catch (const std::exception& e) {

	}
	return newComponent;
}

ModelDataDefinition* Diode::NewInstance(Model* model, std::string name) {
	return new Diode(model, name);
}

void Diode::setElectricalModel(std::string electricalModel) {
	this->electricalModel = electricalModel;
}

void Diode::Build() {

	SPICECircuit::BuildCircuit("D a b", id, {}, electricalModel, electricalModel);
}

PluginInformation* PMOS::GetPluginInformation() {
	PluginInformation* info = new PluginInformation(Util::TypeOf<PMOS>(), &PMOS::LoadInstance, &PMOS::NewInstance);
	info->setCategory("Electronics simulation");
	return info;
}

ModelComponent* PMOS::LoadInstance(Model* model, PersistenceRecord *fields) {
	PMOS* newComponent = new PMOS(model);
	try {
		// newComponent->_loadInstance(fields);
	} catch (const std::exception& e) {

	}
	return newComponent;
}

ModelDataDefinition* PMOS::NewInstance(Model* model, std::string name) {
	return new PMOS(model, name);
}

void PMOS::setWidth(float width) {
	this->width = width;
}

void PMOS::setLength(float length) {
	this->length = length;
}

void PMOS::setElectricalModel(std::string electricalModel) {
	this->electricalModel = electricalModel;
}

void PMOS::Build() {

	SPICECircuit::BuildCircuit("Mp source gate drain bulk", id, {"w="+uc(width),"l="+uc(length)}, "pmos"+electricalModel, electricalModel);
}

PluginInformation* NMOS::GetPluginInformation() {
	PluginInformation* info = new PluginInformation(Util::TypeOf<NMOS>(), &NMOS::LoadInstance, &NMOS::NewInstance);
	info->setCategory("Electronics simulation");
	return info;
}

ModelComponent* NMOS::LoadInstance(Model* model, PersistenceRecord *fields) {
	NMOS* newComponent = new NMOS(model);
	try {
		// newComponent->_loadInstance(fields);
	} catch (const std::exception& e) {

	}
	return newComponent;
}

ModelDataDefinition* NMOS::NewInstance(Model* model, std::string name) {
	return new NMOS(model, name);
}

void NMOS::setWidth(float width) {
	this->width = width;
}

void NMOS::setLength(float length) {
	this->length = length;
}

void NMOS::setElectricalModel(std::string electricalModel) {
	this->electricalModel = electricalModel;
}

void NMOS::Build() {

	SPICECircuit::BuildCircuit("Mn source gate drain bulk", id, {"w="+uc(width),"l="+uc(length)}, "nmos"+electricalModel, electricalModel);
}

PluginInformation* NOT::GetPluginInformation() {
	PluginInformation* info = new PluginInformation(Util::TypeOf<NOT>(), &NOT::LoadInstance, &NOT::NewInstance);
	info->setCategory("Electronics simulation");
	return info;
}

ModelComponent* NOT::LoadInstance(Model* model, PersistenceRecord *fields) {
	NOT* newComponent = new NOT(model);
	try {
		// newComponent->_loadInstance(fields);
	} catch (const std::exception& e) {

	}
	return newComponent;
}

ModelDataDefinition* NOT::NewInstance(Model* model, std::string name) {
	return new NOT(model, name);
}

void NOT::setPMOSWidth(float PMOSWidth) {
	this->PMOSWidth = PMOSWidth;
}

void NOT::setPMOSLength(float PMOSLength) {
	this->PMOSLength = PMOSLength;
}

void NOT::setNMOSWidth(float NMOSWidth) {
	this->NMOSWidth = NMOSWidth;
}

void NOT::setNMOSLength(float NMOSLength) {
	this->NMOSLength = NMOSLength;
}

void NOT::setElectricalModel(std::string electricalModel) {
	this->electricalModel = electricalModel;
}

void NOT::Build() {
	SPICECircuit::BuildCircuit(
				".subckt NOT vp a s\n"
				"Mp1 vp a s vp PMOS"+electricalModel+" w="+uc(PMOSWidth)+" l="+uc(PMOSLength)+"\n"
																							  "Mn1 s a vm vm NMOS"+electricalModel+" w="+uc(NMOSWidth)+" l="+uc(NMOSLength)+"\n"
																																											".ends\n",
				id, {}, electricalModel, electricalModel);
}

PluginInformation* NAND::GetPluginInformation() {
	PluginInformation* info = new PluginInformation(Util::TypeOf<NAND>(), &NAND::LoadInstance, &NAND::NewInstance);
	info->setCategory("Electronics simulation");
	return info;
}

ModelComponent* NAND::LoadInstance(Model* model, PersistenceRecord *fields) {
	NAND* newComponent = new NAND(model);
	try {
		// newComponent->_loadInstance(fields);
	} catch (const std::exception& e) {

	}
	return newComponent;
}

ModelDataDefinition* NAND::NewInstance(Model* model, std::string name) {
	return new NAND(model, name);
}

void NAND::setPMOSWidth(float PMOSWidth) {
	this->PMOSWidth = PMOSWidth;
}

void NAND::setPMOSLength(float PMOSLength) {
	this->PMOSLength = PMOSLength;
}

void NAND::setNMOSWidth(float NMOSWidth) {
	this->NMOSWidth = NMOSWidth;
}

void NAND::setNMOSLength(float NMOSLength) {
	this->NMOSLength = NMOSLength;
}

void NAND::setElectricalModel(std::string electricalModel) {
	this->electricalModel = electricalModel;
}

void NAND::Build() {

	SPICECircuit::BuildCircuit(
				".subckt NAND vp vm a b s\n"
				"Mp1 vp a s vp PMOS"+electricalModel+" w="+uc(PMOSWidth)+" l="+uc(PMOSLength)+"\n"
																							  "Mp2 vp b s vp PMOS"+electricalModel+" w="+uc(PMOSWidth)+" l="+uc(PMOSLength)+"\n"
																																											"Mn1 s a i1 vm NMOS"+electricalModel+" w="+uc(NMOSWidth)+" l="+uc(NMOSLength)+"\n"
																																																														  "Mn2 i1 b vm vm NMOS"+electricalModel+" w="+uc(NMOSWidth)+" l="+uc(NMOSLength)+"\n"
																																																																																		 ".ends\n",
				id, {}, electricalModel, electricalModel);
}

PluginInformation* AND::GetPluginInformation() {
	PluginInformation* info = new PluginInformation(Util::TypeOf<AND>(), &AND::LoadInstance, &AND::NewInstance);
	info->setCategory("Electronics simulation");
	return info;
}

ModelComponent* AND::LoadInstance(Model* model, PersistenceRecord *fields) {
	AND* newComponent = new AND(model);
	try {
		// newComponent->_loadInstance(fields);
	} catch (const std::exception& e) {

	}
	return newComponent;
}

ModelDataDefinition* AND::NewInstance(Model* model, std::string name) {
	return new AND(model, name);
}

void AND::setPMOSWidth(float PMOSWidth) {
	this->PMOSWidth = PMOSWidth;
}

void AND::setPMOSLength(float PMOSLength) {
	this->PMOSLength = PMOSLength;
}

void AND::setNMOSWidth(float NMOSWidth) {
	this->NMOSWidth = NMOSWidth;
}

void AND::setNMOSLength(float NMOSLength) {
	this->NMOSLength = NMOSLength;
}

void AND::setElectricalModel(std::string electricalModel) {
	this->electricalModel = electricalModel;
}

void AND::Build() {

	SPICECircuit::BuildCircuit(
				".subckt AND vp vm a b s\n"
				"Mp1 vp a ns vp PMOS"+electricalModel+" w="+uc(PMOSWidth)+" l="+uc(PMOSLength)+"\n"
																							   "Mp2 vp b ns vp PMOS"+electricalModel+" w="+uc(PMOSWidth)+" l="+uc(PMOSLength)+"\n"
																																											  "Mn1 ns a i1 vm NMOS"+electricalModel+" w="+uc(NMOSWidth)+" l="+uc(NMOSLength)+"\n"
																																																															 "Mn2 i1 b vm vm NMOS"+electricalModel+" w="+uc(NMOSWidth)+" l="+uc(NMOSLength)+"\n"
																																																																																			"Mp3 vp ns s vp PMOS"+electricalModel+" w="+uc(PMOSWidth)+" l="+uc(PMOSLength)+"\n"
																																																																																																						   "Mn3 s ns vm vm NMOS"+electricalModel+" w="+uc(NMOSWidth)+" l="+uc(NMOSLength)+"\n"
																																																																																																																										  ".ends\n",
				id, {}, electricalModel, electricalModel);
}

PluginInformation* NOR::GetPluginInformation() {
	PluginInformation* info = new PluginInformation(Util::TypeOf<NOR>(), &NOR::LoadInstance, &NOR::NewInstance);
	info->setCategory("Electronics simulation");
	return info;
}

ModelComponent* NOR::LoadInstance(Model* model, PersistenceRecord *fields) {
	NOR* newComponent = new NOR(model);
	try {
		// newComponent->_loadInstance(fields);
	} catch (const std::exception& e) {

	}
	return newComponent;
}

ModelDataDefinition* NOR::NewInstance(Model* model, std::string name) {
	return new NOR(model, name);
}

void NOR::setPMOSWidth(float PMOSWidth) {
	this->PMOSWidth = PMOSWidth;
}

void NOR::setPMOSLength(float PMOSLength) {
	this->PMOSLength = PMOSLength;
}

void NOR::setNMOSWidth(float NMOSWidth) {
	this->NMOSWidth = NMOSWidth;
}

void NOR::setNMOSLength(float NMOSLength) {
	this->NMOSLength = NMOSLength;
}

void NOR::setElectricalModel(std::string electricalModel) {
	this->electricalModel = electricalModel;
}

void NOR::Build() {

	SPICECircuit::BuildCircuit(
				".subckt NOR vp vm a b s\n"
				"Mp1 vp a i1 vp PMOS"+electricalModel+" w="+uc(PMOSWidth)+" l="+uc(PMOSLength)+"\n"
																							   "Mp2 i1 b s vp PMOS"+electricalModel+" w="+uc(PMOSWidth)+" l="+uc(PMOSLength)+"\n"
																																											 "Mn1 s a vm vm NMOS"+electricalModel+" w="+uc(NMOSWidth)+" l="+uc(NMOSLength)+"\n"
																																																														   "Mn2 s b vm vm NMOS"+electricalModel+" w="+uc(NMOSWidth)+" l="+uc(NMOSLength)+"\n"
																																																																																		 ".ends\n",
				id, {}, electricalModel, electricalModel);
}

PluginInformation* OR::GetPluginInformation() {
	PluginInformation* info = new PluginInformation(Util::TypeOf<OR>(), &OR::LoadInstance, &OR::NewInstance);
	info->setCategory("Electronics simulation");
	return info;
}

ModelComponent* OR::LoadInstance(Model* model, PersistenceRecord *fields) {
	OR* newComponent = new OR(model);
	try {
		// newComponent->_loadInstance(fields);
	} catch (const std::exception& e) {

	}
	return newComponent;
}

ModelDataDefinition* OR::NewInstance(Model* model, std::string name) {
	return new OR(model, name);
}

void OR::setPMOSWidth(float PMOSWidth) {
	this->PMOSWidth = PMOSWidth;
}

void OR::setPMOSLength(float PMOSLength) {
	this->PMOSLength = PMOSLength;
}

void OR::setNMOSWidth(float NMOSWidth) {
	this->NMOSWidth = NMOSWidth;
}

void OR::setNMOSLength(float NMOSLength) {
	this->NMOSLength = NMOSLength;
}

void OR::setElectricalModel(std::string electricalModel) {
	this->electricalModel = electricalModel;
}

void OR::Build() {

	SPICECircuit::BuildCircuit(
				".subckt OR vp vm a b s\n"
				"Mp1 vp a i1 vp PMOS"+electricalModel+" w="+uc(PMOSWidth)+" l="+uc(PMOSLength)+"\n"
																							   "Mp2 i1 b ns vp PMOS"+electricalModel+" w="+uc(PMOSWidth)+" l="+uc(PMOSLength)+"\n"
																																											  "Mn1 ns a vm vm NMOS"+electricalModel+" w="+uc(NMOSWidth)+" l="+uc(NMOSLength)+"\n"
																																																															 "Mn2 ns b vm vm NMOS"+electricalModel+" w="+uc(NMOSWidth)+" l="+uc(NMOSLength)+"\n"
																																																																																			"Mp3 vp ns s vp PMOS"+electricalModel+" w="+uc(PMOSWidth)+" l="+uc(PMOSLength)+"\n"
																																																																																																						   "Mn3 s ns vm vm NMOS"+electricalModel+" w="+uc(NMOSWidth)+" l="+uc(NMOSLength)+"\n"
																																																																																																																										  ".ends\n",
				id, {}, electricalModel, electricalModel);
}

PluginInformation* XOR::GetPluginInformation() {
	PluginInformation* info = new PluginInformation(Util::TypeOf<XOR>(), &XOR::LoadInstance, &XOR::NewInstance);
	info->setCategory("Electronics simulation");
	return info;
}

ModelComponent* XOR::LoadInstance(Model* model, PersistenceRecord *fields) {
	XOR* newComponent = new XOR(model);
	try {
		// newComponent->_loadInstance(fields);
	} catch (const std::exception& e) {

	}
	return newComponent;
}

ModelDataDefinition* XOR::NewInstance(Model* model, std::string name) {
	return new XOR(model, name);
}

void XOR::setPMOSWidth(float PMOSWidth) {
	this->PMOSWidth = PMOSWidth;
}

void XOR::setPMOSLength(float PMOSLength) {
	this->PMOSLength = PMOSLength;
}

void XOR::setNMOSWidth(float NMOSWidth) {
	this->NMOSWidth = NMOSWidth;
}

void XOR::setNMOSLength(float NMOSLength) {
	this->NMOSLength = NMOSLength;
}

void XOR::setElectricalModel(std::string electricalModel) {
	this->electricalModel = electricalModel;
}

void XOR::Build() {

	SPICECircuit::BuildCircuit(
				".subckt XOR vp vm a b s\n"
				"Mp1 vp a na vp PMOS"+electricalModel+" w="+uc(PMOSWidth)+" l="+uc(PMOSLength)+"\n"
																							   "Mn1 na a vm vm NMOS"+electricalModel+" w="+uc(NMOSWidth)+" l="+uc(NMOSLength)+"\n"

																																											  "Mp2 vp b nb vp PMOS"+electricalModel+" w="+uc(PMOSWidth)+" l="+uc(PMOSLength)+"\n"
																																																															 "Mn2 nb b vm vm NMOS"+electricalModel+" w="+uc(NMOSWidth)+" l="+uc(NMOSLength)+"\n"

																																																																																			"Mp3 vp na i1 vp PMOS"+electricalModel+" w="+uc(PMOSWidth)+" l="+uc(PMOSLength)+"\n"
																																																																																																							"Mp4 i1 b s vp PMOS"+electricalModel+" w="+uc(PMOSWidth)+" l="+uc(PMOSLength)+"\n"
																																																																																																																										  "Mp5 vp a i2 vp PMOS"+electricalModel+" w="+uc(PMOSWidth)+" l="+uc(PMOSLength)+"\n"
																																																																																																																																														 "Mp6 i2 nb s vp PMOS"+electricalModel+" w="+uc(PMOSWidth)+" l="+uc(PMOSLength)+"\n"
																																																																																																																																																																		"Mn3 s nb i3 vm NMOS"+electricalModel+" w="+uc(NMOSWidth)+" l="+uc(NMOSLength)+"\n"
																																																																																																																																																																																					   "Mn4 i3 na vm vm NMOS"+electricalModel+" w="+uc(NMOSWidth)+" l="+uc(NMOSLength)+"\n"
																																																																																																																																																																																																									   "Mn5 s b i4 vm NMOS"+electricalModel+" w="+uc(NMOSWidth)+" l="+uc(NMOSLength)+"\n"
																																																																																																																																																																																																																													 "Mn6 i4 a vm vm NMOS"+electricalModel+" w="+uc(NMOSWidth)+" l="+uc(NMOSLength)+"\n"
																																																																																																																																																																																																																																																	".ends\n",
				id, {}, electricalModel, electricalModel);
}

PluginInformation* XNOR::GetPluginInformation() {
	PluginInformation* info = new PluginInformation(Util::TypeOf<XNOR>(), &XNOR::LoadInstance, &XNOR::NewInstance);
	info->setCategory("Electronics simulation");
	return info;
}

ModelComponent* XNOR::LoadInstance(Model* model, PersistenceRecord *fields) {
	XNOR* newComponent = new XNOR(model);
	try {
		// newComponent->_loadInstance(fields);
	} catch (const std::exception& e) {

	}
	return newComponent;
}

ModelDataDefinition* XNOR::NewInstance(Model* model, std::string name) {
	return new XNOR(model, name);
}

void XNOR::setPMOSWidth(float PMOSWidth) {
	this->PMOSWidth = PMOSWidth;
}

void XNOR::setPMOSLength(float PMOSLength) {
	this->PMOSLength = PMOSLength;
}

void XNOR::setNMOSWidth(float NMOSWidth) {
	this->NMOSWidth = NMOSWidth;
}

void XNOR::setNMOSLength(float NMOSLength) {
	this->NMOSLength = NMOSLength;
}

void XNOR::setElectricalModel(std::string electricalModel) {
	this->electricalModel = electricalModel;
}

void XNOR::Build() {

	SPICECircuit::BuildCircuit(
				".subckt XNOR vp vm a b s\n"
				"Mp1 vp a na vp PMOS"+electricalModel+" w="+uc(PMOSWidth)+" l="+uc(PMOSLength)+"\n"
																							   "Mn1 na a vm vm NMOS"+electricalModel+" w="+uc(NMOSWidth)+" l="+uc(NMOSLength)+"\n"

																																											  "Mp2 vp b nb vp PMOS"+electricalModel+" w="+uc(PMOSWidth)+" l="+uc(PMOSLength)+"\n"
																																																															 "Mn2 nb b vm vm NMOS"+electricalModel+" w="+uc(NMOSWidth)+" l="+uc(NMOSLength)+"\n"

																																																																																			"Mp3 vp na i1 vp PMOS"+electricalModel+" w="+uc(PMOSWidth)+" l="+uc(PMOSLength)+"\n"
																																																																																																							"Mp4 i1 b ns vp PMOS"+electricalModel+" w="+uc(PMOSWidth)+" l="+uc(PMOSLength)+"\n"
																																																																																																																										   "Mp5 vp a i2 vp PMOS"+electricalModel+" w="+uc(PMOSWidth)+" l="+uc(PMOSLength)+"\n"
																																																																																																																																														  "Mp6 i2 nb ns vp PMOS"+electricalModel+" w="+uc(PMOSWidth)+" l="+uc(PMOSLength)+"\n"
																																																																																																																																																																		  "Mn3 ns nb i3 vm NMOS"+electricalModel+" w="+uc(NMOSWidth)+" l="+uc(NMOSLength)+"\n"
																																																																																																																																																																																						  "Mn4 i3 na vm vm NMOS"+electricalModel+" w="+uc(NMOSWidth)+" l="+uc(NMOSLength)+"\n"
																																																																																																																																																																																																										  "Mn5 ns b i4 vm NMOS"+electricalModel+" w="+uc(NMOSWidth)+" l="+uc(NMOSLength)+"\n"
																																																																																																																																																																																																																														 "Mn6 i4 a vm vm NMOS"+electricalModel+" w="+uc(NMOSWidth)+" l="+uc(NMOSLength)+"\n"

																																																																																																																																																																																																																																																		"Mp7 vp ns s vp PMOS"+electricalModel+" w="+uc(PMOSWidth)+" l="+uc(PMOSLength)+"\n"
																																																																																																																																																																																																																																																																					   "Mn7 s ns vm vm NMOS"+electricalModel+" w="+uc(NMOSWidth)+" l="+uc(NMOSLength)+"\n"

																																																																																																																																																																																																																																																																																									  ".ends\n",
				id, {}, electricalModel, electricalModel);
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
