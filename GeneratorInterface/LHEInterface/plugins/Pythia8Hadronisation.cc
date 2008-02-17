#include <iostream>
#include <sstream>
#include <string>
#include <memory>
#include <assert.h>

#include <boost/shared_ptr.hpp>

#include <HepMC/GenEvent.h>
#include <HepMC/GenParticle.h>

#define PYTHIA_VERSION	8070

#include <Pythia.h>
#include <LesHouches.h>
#if PYTHIA_VERSION >= 8100
#	include <HepMCInterface.h>
#else
#	include <I_Pythia8.h>
#endif

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"

#include "GeneratorInterface/LHEInterface/interface/LesHouches.h"
#include "GeneratorInterface/LHEInterface/interface/LHECommon.h"
#include "GeneratorInterface/LHEInterface/interface/LHEEvent.h"
#include "GeneratorInterface/LHEInterface/interface/Hadronisation.h"

using namespace Pythia8;

namespace lhef {

class Pythia8Hadronisation : public Hadronisation {
    public:
	Pythia8Hadronisation(const edm::ParameterSet &params);
	~Pythia8Hadronisation();

    private:
	std::auto_ptr<HepMC::GenEvent> hadronize();
	double getCrossSection() const;
	void newCommon(const boost::shared_ptr<LHECommon> &common);

	const int				pythiaPylistVerbosity;
	int					maxEventsToPrint;

	class LHAinitLesHouches;
	class LHAevntLesHouches;

	std::auto_ptr<Pythia>			pythia;
	std::auto_ptr<LHAinitLesHouches>	lhaInit;
	std::auto_ptr<LHAevntLesHouches>	lhaEvent;
	std::auto_ptr<HepMC::I_Pythia8>		conv;
};

class Pythia8Hadronisation::LHAinitLesHouches : public LHAinit {
    public:
	void load(const boost::shared_ptr<LHECommon> &common)
	{ this->common = common; }

    private:
	bool set();

	boost::shared_ptr<LHECommon>	common;
};

class Pythia8Hadronisation::LHAevntLesHouches : public LHAevnt {
    public:
	void load(const boost::shared_ptr<LHEEvent> &event)
	{ this->event = event; }

    private:
#if PYTHIA_VERSION >= 8100
	bool set(int dummy);
#else
	bool set();
#endif

	boost::shared_ptr<LHEEvent>	event;
};

bool Pythia8Hadronisation::LHAinitLesHouches::set()
{
	if (!common)
		return false;
	const HEPRUP &heprup = *common->getHEPRUP();

	beamA(heprup.IDBMUP.first, heprup.EBMUP.first,
	      heprup.PDFGUP.first, heprup.PDFSUP.first);
	beamB(heprup.IDBMUP.second, heprup.EBMUP.second,
	      heprup.PDFGUP.second, heprup.PDFSUP.second);
	strategy(heprup.IDWTUP);

	for(int i = 0; i < heprup.NPRUP; i++)
		process(heprup.LPRUP[i], heprup.XSECUP[i],
		        heprup.XERRUP[i], heprup.XMAXUP[i]);

	common.reset();
	return true;
}

#if PYTHIA_VERSION >= 8100
bool Pythia8Hadronisation::LHAevntLesHouches::set(int dummy)
#else
bool Pythia8Hadronisation::LHAevntLesHouches::set()
#endif
{
	if (!event)
		return false;
	const HEPEUP &hepeup = *event->getHEPEUP();

	process(hepeup.IDPRUP, hepeup.XWGTUP, hepeup.SCALUP,
	        hepeup.AQEDUP, hepeup.AQCDUP);

	for(int i = 0; i < hepeup.NUP; i++)
		particle(hepeup.IDUP[i], hepeup.ISTUP[i],
		         hepeup.MOTHUP[i].first, hepeup.MOTHUP[i].second,
		         hepeup.ICOLUP[i].first, hepeup.ICOLUP[i].second,
		         hepeup.PUP[i][0], hepeup.PUP[i][1], hepeup.PUP[i][2],
		         hepeup.PUP[i][3], hepeup.PUP[i][4], hepeup.VTIMUP[i],
		         hepeup.SPINUP[i]);

	const LHEEvent::PDF *pdf = event->getPDF();
	if (pdf)
		this->pdf(pdf->id.first, pdf->id.second,
		          pdf->x.first, pdf->x.second,
		          pdf->scalePDF,
		          pdf->xPDF.first, pdf->xPDF.second);

	event.reset();
	return true;
}

Pythia8Hadronisation::Pythia8Hadronisation(const edm::ParameterSet &params) :
	Hadronisation(params),
	pythiaPylistVerbosity(params.getUntrackedParameter<int>("pythiaPylistVerbosity", 0)),
	maxEventsToPrint(params.getUntrackedParameter<int>("maxEventsToPrint", 0))
{
	std::vector<std::string> setNames =
		params.getParameter<std::vector<std::string> >("parameterSets");

	pythia.reset(new Pythia);
	lhaInit.reset(new LHAinitLesHouches);
	lhaEvent.reset(new LHAevntLesHouches);
	conv.reset(new HepMC::I_Pythia8);

	for(std::vector<std::string>::const_iterator iter = setNames.begin();
	    iter != setNames.end(); ++iter) {
		std::vector<std::string> lines =
			params.getParameter< std::vector<std::string> >(*iter);

		edm::LogInfo("Generator|LHEInterface") << "----------------------------------------------";
		edm::LogInfo("Generator|LHEInterface") << "Read PYTHIA8 parameter set " << *iter;
		edm::LogInfo("Generator|LHEInterface") << "----------------------------------------------";

		for(std::vector<std::string>::const_iterator line = lines.begin();
		    line != lines.end(); ++line ) {
#if PYTHIA_VERSION >= 8100
			if (line->substr(0, 14) == "Random:setSeed" ||
			    line->substr(0, 11) == "Random:seed")
#else
			if (line->substr(0, 11) == "Pythia:seed")
#endif
				throw cms::Exception("PythiaError")
					<< "Attempted to set random number"
					   " using Pythia command 'MRPY(1)'."
					   " Please use the"
					   " RandomNumberGeneratorService."
					<< std::endl;
			if (!pythia->readString(*line))
				throw cms::Exception("PythiaError")
					<< "Pythia did not accept \""
					<< *line << "\"." << std::endl;
		}
	}

	edm::Service<edm::RandomNumberGenerator> rng;
	std::ostringstream ss;
#if PYTHIA_VERSION >= 8100
	ss << "Random:seed = " << rng->mySeed();
	pythia->readString(ss.str());
	pythia->readString("Random:setSeed = on");
#else
	ss << "Pythia:seed = " << rng->mySeed();
	pythia->readString(ss.str());
#endif
}

Pythia8Hadronisation::~Pythia8Hadronisation()
{
}

// naive Pythia8 HepMC status fixup
static int getStatus(const HepMC::GenParticle *p)
{
	int status = p->status();
	if (status > 0)
		return status;
	else if (status > -30 && status < 0)
		return 3;
	else
		return 2;
}

std::auto_ptr<HepMC::GenEvent> Pythia8Hadronisation::hadronize()
{
	lhaEvent->load(getRawEvent());
	if (!pythia->next())
		throw cms::Exception("PythiaError")
			<< "Pythia did not want to process event."
			<< std::endl;

	std::auto_ptr<HepMC::GenEvent> event(new HepMC::GenEvent);
	conv->fill_next_event(pythia->event, event.get());

	for(HepMC::GenEvent::particle_iterator iter = event->particles_begin();
	    iter != event->particles_end(); iter++)
		(*iter)->set_status(getStatus(*iter));

	event->set_signal_process_id(pythia->info.code());
	event->set_event_scale(pythia->info.pTHat());

	if (maxEventsToPrint > 0) {
		maxEventsToPrint--;
		if (pythiaPylistVerbosity)
			pythia->event.list(std::cout);
	}

	return event;
}

double Pythia8Hadronisation::getCrossSection() const
{
	return pythia->info.sigmaGen();
}

void Pythia8Hadronisation::newCommon(const boost::shared_ptr<LHECommon> &common)
{
	lhaInit->load(common);
	pythia->init(lhaInit.get(), lhaEvent.get());
}

DEFINE_LHE_HADRONISATION_PLUGIN(Pythia8Hadronisation);

} // namespace lhef
