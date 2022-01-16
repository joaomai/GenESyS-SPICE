/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Enter.cpp
 * Author: rlcancian
 * 
 * Created on 03 de Junho de 2019, 15:14
 */

#include "Enter.h"
#include "../../kernel/simulator/Model.h"
#include "../../kernel/simulator/Counter.h"

Enter::Enter(Model* model, std::string name) : ModelComponent(model, Util::TypeOf<Enter>(), name) {
}

std::string Enter::show() {
	return ModelComponent::show() + ",station=" + this->_station->getName();
}

ModelComponent* Enter::LoadInstance(Model* model, std::map<std::string, std::string>* fields) {
	Enter* newComponent = new Enter(model);
	try {
		newComponent->_loadInstance(fields);
	} catch (const std::exception& e) {

	}
	return newComponent;
}

void Enter::setStation(Station* _station) {
	this->_station = _station;
	_station->setEnterIntoStationComponent(this);
}

Station* Enter::getStation() const {
	return _station;
}

void Enter::_execute(Entity* entity) {
	if (_reportStatistics)
		_numberIn->incCountValue();
	_station->enter(entity);
	_parentModel->sendEntityToComponent(entity, this->getConnections()->getFrontConnection());
}

bool Enter::_loadInstance(std::map<std::string, std::string>* fields) {
	bool res = ModelComponent::_loadInstance(fields);
	if (res) {
		std::string stationName = LoadField(fields, "station", "");
		Station* station = dynamic_cast<Station*> (_parentModel->getData()->getData(Util::TypeOf<Station>(), stationName));
		this->_station = station;
	}
	return res;
}

//void Enter::_initBetweenReplications() {}

std::map<std::string, std::string>* Enter::_saveInstance(bool saveDefaultValues) {
	std::map<std::string, std::string>* fields = ModelComponent::_saveInstance(saveDefaultValues);
	std::string text = "";
	if (_station != nullptr) {
		text = _station->getName();
	}
	SaveField(fields, "station", text, "", saveDefaultValues);
	return fields;
}

bool Enter::_check(std::string* errorMessage) {
	bool resultAll = true;
	resultAll &= _parentModel->getData()->check(Util::TypeOf<Station>(), _station, "Station", errorMessage);
	return resultAll;
}

PluginInformation* Enter::GetPluginInformation() {
	PluginInformation* info = new PluginInformation(Util::TypeOf<Enter>(), &Enter::LoadInstance);
	info->setReceiveTransfer(true);
	info->setCategory("Material Handling");
	info->insertDynamicLibFileDependence("station.so");
	info->setDescriptionHelp("//@TODO");
	return info;
}

void Enter::_createInternalData() {
	if (_reportStatistics) {
		if (_numberIn == nullptr) {
			_numberIn = new Counter(_parentModel, getName() + "." + "CountNumberIn", this);
			_internalData->insert({"CountNumberIn", _numberIn});
		}
	} else
		if (_numberIn != nullptr) {
		_removeInternalDatas();
	}
}