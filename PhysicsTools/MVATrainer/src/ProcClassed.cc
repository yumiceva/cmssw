#include <algorithm>
#include <iterator>
#include <iostream>
#include <cstring>
#include <vector>
#include <string>
#include <set>

#include <xercesc/dom/DOM.hpp>

#include "FWCore/Utilities/interface/Exception.h"

#include "PhysicsTools/MVAComputer/interface/AtomicId.h"

#include "PhysicsTools/MVATrainer/interface/XMLSimpleStr.h"
#include "PhysicsTools/MVATrainer/interface/XMLDocument.h"
#include "PhysicsTools/MVATrainer/interface/MVATrainer.h"
#include "PhysicsTools/MVATrainer/interface/TrainProcessor.h"

XERCES_CPP_NAMESPACE_USE

using namespace PhysicsTools;

namespace { // anonymous

class ProcClassed : public TrainProcessor {
    public:
	typedef TrainProcessor::Registry<ProcClassed>::Type Registry;

	ProcClassed(const char *name, const AtomicId *id, MVATrainer *trainer);
	virtual ~ProcClassed();

	virtual void configure(DOMElement *elem);
	virtual Calibration::VarProcessor *getCalibration() const;

    private:
	unsigned int	count;
};

static ProcClassed::Registry registry("ProcClassed");

ProcClassed::ProcClassed(const char *name, const AtomicId *id,
                         MVATrainer *trainer) :
	TrainProcessor(name, id, trainer)
{
}

ProcClassed::~ProcClassed()
{
}

void ProcClassed::configure(DOMElement *elem)
{
	DOMNode *node = elem->getFirstChild();
	while(node && node->getNodeType() != DOMNode::ELEMENT_NODE)
		node = node->getNextSibling();

	if (!node ||
	    std::strcmp(XMLSimpleStr(node->getNodeName()), "classes") != 0)
		throw cms::Exception("ProcClassed")
			<< "Expected classes tag in config section."
			<< std::endl;

	elem = static_cast<DOMElement*>(node);

	count = XMLDocument::readAttribute<unsigned int>(elem, "count");

	node = node->getNextSibling();
	while(node && node->getNodeType() != DOMNode::ELEMENT_NODE)
		node = node->getNextSibling();

	if (node)
		throw cms::Exception("ProcClassed")
			<< "Superfluous tags in config section."
			<< std::endl;

	trained = true;
}

Calibration::VarProcessor *ProcClassed::getCalibration() const
{
	Calibration::ProcClassed *calib = new Calibration::ProcClassed;
	calib->nClasses = count;
	return calib;
}

} // anonymous namespace
