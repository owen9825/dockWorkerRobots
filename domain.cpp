#include "domain.h"

void Action::fillPredicates() {
	switch(this->identity) {
	case actType::NoAType:
		break;
	case actType::Move:
		this->emptyPreconditions.push(Predicate(predType::Adjacent,"from","to"));
		this->emptyPreconditions.push(Predicate(predType::At,"r","from"));
		this->emptyPreconditions.push(Predicate(predType::Free,"to"));
		this->effects.push_back(Predicate(predType::At,"r","to"));
		this->effects.push_back(Predicate(predType::Free,"from"));
		//negative effects are just left out; not handled explicitly
		break;
	case actType::Load:
		this->emptyPreconditions.push(Predicate(predType::At,"r","l"));
		this->emptyPreconditions.push(Predicate(predType::Belong,"k","l"));
		this->emptyPreconditions.push(Predicate(predType::Holding,"k","c"));
		this->emptyPreconditions.push(Predicate(predType::Unloaded,"r"));
		this->effects.push_back(Predicate(predType::Loaded,"r","c"));
		this->effects.push_back(Predicate(predType::Empty,"k"));
		//some negative effects
		break;
	case actType::Unload:
		break;
		this->emptyPreconditions.push(Predicate(predType::Belong,"k","l"));
		this->emptyPreconditions.push(Predicate(predType::At,"r","l"));
		this->emptyPreconditions.push(Predicate(predType::Loaded,"r","c"));
		this->emptyPreconditions.push(Predicate(predType::Empty,"k"));
		this->effects.push_back(Predicate(predType::Unloaded,"r"));
		this->effects.push_back(Predicate(predType::Holding,"k","c"));
		//some negative effects
	case actType::Take:
		this->emptyPreconditions.push(Predicate(predType::Belong,"k","l"));
		this->emptyPreconditions.push(Predicate(predType::Attached,"p","l"));
		this->emptyPreconditions.push(Predicate(predType::Empty,"k"));
		this->emptyPreconditions.push(Predicate(predType::In,"c","p"));
		this->emptyPreconditions.push(Predicate(predType::Top,"c","p"));
		this->emptyPreconditions.push(Predicate(predType::On,"c","else"));
		this->effects.push_back(Predicate(predType::Holding,"k","c"));
		this->effects.push_back(Predicate(predType::Top,"else","p"));
		//some negative effects
		break;
	case actType::Put:
		this->emptyPreconditions.push(Predicate(predType::Belong,"k","l"));
		this->emptyPreconditions.push(Predicate(predType::Attached,"p","l"));
		this->emptyPreconditions.push(Predicate(predType::Holding,"k","c"));
		this->emptyPreconditions.push(Predicate(predType::Top,"else","p"));
		this->effects.push_back(Predicate(predType::In,"c","p"));
		this->effects.push_back(Predicate(predType::On,"c","else"));
		this->effects.push_back(Predicate(predType::Empty,"k"));
		break;
	default:
		assert(false); //invalid type of action submitted
	}
}

const std::string Action::key() const {
	if(this->keyVal.length() > 0) {
		//key has already been calculated
		return this->keyVal;
	} else if(this->identity == actType::NoOp) {
		this->keyVal = std::string(1,(char)identity);
		this->keyVal += effects[0].key();
		return this->keyVal;
	} else if(emptyPreconditions.size() > 0) {
		//key is not yet ready
		return NULL;
	} else {
		this->keyVal = std::string(1,(char)identity);
		for(std::map<std::string,Predicate*>::const_iterator it=preLinks.begin(); it!=preLinks.end();it++) {
			keyVal += it->second->getNameOne();
			if(it->second->getTypeTwo() != predType::NoPType) {
				keyVal += it->second->getNameTwo();
			}
		}
		return std::string(this->keyVal);
	}
}

const std::string Predicate::key() const {
	if(this->keyVal.length() > 0) {
			//key has already been calculated
			return this->keyVal;
	} else {
		if(this->p2type != predType::NoPType && this->p2Name.length() <= 0 ||
			this->p1type != predType::NoPType && this->p1Name.length() <= 0)
		{
			//not ready
			return NULL;
		} else {
			//calculate key
			this->keyVal = std::string(1,(char)identity);
			this->keyVal += p1Name;
			this->keyVal += p2Name; //if it's empty, nothing gets appended here
			return this->keyVal;
		}
	}
}

Predicate Action::nextMissingLink() {
	//return the next item from emptyPreconditions with names filled in where possible
	//Don't delete that from the queue until a replacement has been given
	assert(emptyPreconditions.size() != 0);
	Predicate pred = emptyPreconditions.front();
	std::string name1 = pred.getNameOne();
	assert(name1.length() > 0);
	if(σ.count(name1)) {
		pred.setNameOne(σ[name1]);
	} else {
		pred.setNameOne("");
	}
	if(pred.getTypeTwo() != predType::NoPType) {
		std::string name2 = pred.getNameTwo();
		assert(name2.length() > 0);
		if(σ.count(name2)) {
			pred.setNameTwo(σ[name2]);
		} else {
			pred.setNameOne("");
		}
	}

	return pred;
}

bool Action::submitPreLink(Predicate* pred) {
	//check for this predicate type and check the relevance of the names
	//update the mapping if a new name has validly been submitted
	return true;
}

bool Action::submitEffectLink(Predicate* pred) {
	//check vector<Predicate> effects for any Predicates with the same names
	//if there is an exact match, keep pred, otherwise return false and ignore it

	return true;
}

std::vector<Predicate> Action::getEffects(bool onlyPositive) {
	//go through vector<Predicate> effects and change all the names as per the mapping, then return the vector

	return this->effects;
}

void PredicateLayer::addToLayer(const Predicate &pred) {
	if(this->predicates.count(pred.key()))
		return;
	this->predicates[pred.key()] = new Predicate(pred); //make a heap copy and manage it
	return;
}

PredicateLayer::~PredicateLayer() {
	//delete all the predicates on the heap - they have pointers in map
}

ActionLayer::~ActionLayer() {
	//delete all the actions on the heap - they have pointers in map

}

GraphPlanner::GraphPlanner(PredicateLayer &initialState, const std::vector<Predicate> &goalSet) : goalStates(goalSet){
	this->predLayers.push_back(initialState);
}

std::vector<Action> GraphPlanner::plan() {
	std::vector<Action> graphSteps;
	int i = 0;
	int η = 0; //fixed point
	while(!predLayers[i].checkGoal(this->goalStates) || predLayers[i].checkμSet(this->goalStates)) {
		i++;
		this->expand();
		if(!predLayers[i].checkGoal(this->goalStates) || predLayers[i].checkμSet(this->goalStates)) {
			graphSteps.clear();
			return graphSteps;
		}
		η = fixedPoint(i)? noGood[i].size() : 0;

	}
	return graphSteps;
}

void GraphPlanner::expand() {
	//applyActions;
	//generateActions;
}

GraphPlanner::~GraphPlanner() {

}