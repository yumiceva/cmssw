#include "CondFormats/EcalObjects/interface/EcalTPGCrystalStatus.h"
//#include "CondTools/Ecal/interface/EcalTPGCrystalStatusXMLTranslator.h"
#include "CondTools/Ecal/interface/EcalCondHeader.h"
#include "TROOT.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TColor.h"
#include "TLine.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"

#include "CondCore/Utilities/interface/PayLoadInspector.h"
#include "CondCore/Utilities/interface/InspectorPythonWrapper.h"

#include <string>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <boost/ref.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/iterator/transform_iterator.hpp>

#include <fstream>

#include "CondCore/EcalPlugins/plugins/EcalPyWrapperFunctions.h"

namespace cond {

  namespace ecalcond {

    typedef EcalTPGCrystalStatus::Items  Items;
    typedef EcalTPGCrystalStatus::value_type  value_type;

    enum How { singleChannel, bySuperModule, all};

    int bad(Items const & cont) {
      return  std::count_if(cont.begin(),cont.end(),
			    boost::bind(std::greater<int>(),
					boost::bind(&value_type::getStatusCode,_1),0)
			    );
    }

    void extractBarrel(EcalTPGCrystalStatus const & cont, std::vector<int> const &,  std::vector<float> & result) {
      result.resize(1);
      result[0] =  bad(cont.barrelItems());
    }
    
    void extractEndcap(EcalTPGCrystalStatus const & cont, std::vector<int> const &,  std::vector<float> & result) {
      result.resize(1);
      result[0] = bad(cont.endcapItems());
    }
    void extractAll(EcalTPGCrystalStatus const & cont, std::vector<int> const &,  std::vector<float> & result) {
      result.resize(1);
      result[0] = bad(cont.barrelItems())+bad(cont.endcapItems());
    }

    void extractSuperModules(EcalTPGCrystalStatus const & cont, std::vector<int> const & which,  std::vector<float> & result) {
      // bho...
    }

    void extractSingleChannel(EcalTPGCrystalStatus const & cont, std::vector<int> const & which,  std::vector<float> & result) {
      result.reserve(which.size());
      for (unsigned int i=0; i<which.size();i++) {
	result.push_back(cont[which[i]].getStatusCode());
      }
    }

	typedef boost::function<void(EcalTPGCrystalStatus const & cont, std::vector<int> const & which,  std::vector<float> & result)> CondExtractor;
  }  // namespace ecalcond

  template<>
  struct ExtractWhat<EcalTPGCrystalStatus> {

    ecalcond::How m_how;
    std::vector<int> m_which;

    ecalcond::How const & how() const { return m_how;}
    std::vector<int> const & which() const { return m_which;}
 
    void set_how(ecalcond::How i) {m_how=i;}
    void set_which(std::vector<int> & i) { m_which.swap(i);}
  };


  template<>
  class ValueExtractor<EcalTPGCrystalStatus>: public  BaseValueExtractor<EcalTPGCrystalStatus> {
  public:

    static ecalcond::CondExtractor & extractor(ecalcond::How how) {
      static  ecalcond::CondExtractor fun[3] = { 
	ecalcond::CondExtractor(ecalcond::extractSingleChannel),
	ecalcond::CondExtractor(ecalcond::extractSuperModules),
	ecalcond::CondExtractor(ecalcond::extractAll)
      };
      return fun[how];
    }

    typedef EcalTPGCrystalStatus Class;
    typedef ExtractWhat<Class> What;
    static What what() { return What();}

    ValueExtractor(){}
    ValueExtractor(What const & what)
      : m_what(what)
    {
      // here one can make stuff really complicated... 
    }

    void compute(Class const & it){
      std::vector<float> res;
      extractor(m_what.how())(it,m_what.which(),res);
      swap(res);
    }

  private:
    What  m_what;  
  };


  //template<>
  //std::string PayLoadInspector<EcalTPGCrystalStatus>::dump() const {
  //  std::stringstream ss;
  //  EcalCondHeader h;
  //  ss << EcalTPGCrystalStatusXMLTranslator::dumpXML(h,object());
  //  return ss.str();
  //}

 	class EcalTPGCrystalStatusHelper: public EcalPyWrapperHelper<EcalTPGCrystalStatusCode>{
	public:
		//change me
		EcalTPGCrystalStatusHelper():EcalPyWrapperHelper<EcalObject>(1, STATUS, "-Errors total: "){}
	protected:

		//change me
		typedef EcalTPGCrystalStatusCode EcalObject;

		type_vValues getValues( const std::vector<EcalObject> & vItems)
		{
			//change me
			//unsigned int totalValues = 2; 

			type_vValues vValues(total_values);
			
			//change us
			vValues[0].first = "[0]StatusCode";

			
			vValues[0].second = .0;
	
			//get info:
			unsigned int shift = 0, mask = 1;
			unsigned int statusCode;
			for(std::vector<EcalObject>::const_iterator iItems = vItems.begin(); iItems != vItems.end(); ++iItems){
				//change us
				statusCode =  iItems->getStatusCode();
				for (shift = 0; shift < total_values; ++shift){
					mask = 1 << (shift);
					//std::cout << "; statuscode: " << statusCode;
					if (statusCode & mask){
						vValues[shift].second += 1;
					}
				}
			}
			return vValues;
		}
	};

   template<>
   std::string PayLoadInspector<EcalTPGCrystalStatus>::summary() const {
	std::stringstream ss;
	EcalTPGCrystalStatusHelper helper;
	ss << helper.printBarrelsEndcaps(object().barrelItems(), object().endcapItems());
	return ss.str();
   }

	template<>
	std::string PayLoadInspector<EcalTPGCrystalStatus>::plot(std::string const & filename,
		std::string const &, 
		std::vector<int> const&, 
		std::vector<float> const& ) const {
			gStyle->SetPalette(1);
			const int TOTAL_IMAGES = 1;
			const int TOTAL_PADS = 3;

			//    TCanvas canvas("CC map","CC map",840,600);
			const float IMG_SIZE = 1.5;
			TCanvas canvas("CC map","CC map",800*IMG_SIZE, 200 * TOTAL_IMAGES*IMG_SIZE);//800, 1200

			float xmi[3] = {0.0 , 0.22, 0.78};
			float xma[3] = {0.22, 0.78, 1.00};


			TPad*** pad = new TPad**[TOTAL_IMAGES];
			for (int gId = 0; gId < TOTAL_IMAGES; gId++) {
				pad[gId] = new TPad*[TOTAL_PADS];
				for (int obj = 0; obj < TOTAL_PADS; obj++) {
					float yma = 1. - 0;//1.- (0.17 * gId);
					float ymi = yma - 1;//0.15;
					pad[gId][obj] = new TPad(Form("p_%i_%i", obj, gId),Form("p_%i_%i", obj, gId),
						xmi[obj], ymi, xma[obj], yma);
					pad[gId][obj]->Draw();
				}
			}

			const int kGains       = 3;
			const int gainValues[3] = {12, 6, 1};
			const int kSides       = 2;
			const int kBarlRings   = EBDetId::MAX_IETA;
			const int kBarlWedges  = EBDetId::MAX_IPHI;
			const int kEndcWedgesX = EEDetId::IX_MAX;
			const int kEndcWedgesY = EEDetId::IY_MAX;

			TH2F** barrel_m = new TH2F*[3];
			TH2F** endc_p_m = new TH2F*[3];
			TH2F** endc_m_m = new TH2F*[3];
			//TH2F** barrel_r = new TH2F*[3];
			//TH2F** endc_p_r = new TH2F*[3];
			//TH2F** endc_m_r = new TH2F*[3];
			std::string variableName = "TPGCrystalStatus";
			for (int gainId = 0; gainId < kGains; gainId++) {
				barrel_m[gainId] = new TH2F(Form("EBm%i",gainId),Form((variableName + " %i EB").c_str(),gainValues[gainId]),360,0,360, 170, -85,85);
				endc_p_m[gainId] = new TH2F(Form("EE+m%i",gainId),Form((variableName + " %i EE+").c_str(),gainValues[gainId]),100,1,101,100,1,101);
				endc_m_m[gainId] = new TH2F(Form("EE-m%i",gainId),Form((variableName + " %i EE-").c_str(),gainValues[gainId]),100,1,101,100,1,101);
				//barrel_r[gainId] = new TH2F(Form("EBr%i",gainId),Form("rms %i EB",gainValues[gainId]),360,0,360, 170, -85,85);
				//endc_p_r[gainId] = new TH2F(Form("EE+r%i",gainId),Form("rms %i EE+",gainValues[gainId]),100,1,101,100,1,101);
				//endc_m_r[gainId] = new TH2F(Form("EE-r%i",gainId),Form("rms %i EE-",gainValues[gainId]),100,1,101,100,1,101);
			}

			for (int sign=0; sign < kSides; sign++) {
				int thesign = sign==1 ? 1:-1;

				for (int ieta=0; ieta<kBarlRings; ieta++) {
					for (int iphi=0; iphi<kBarlWedges; iphi++) {
						EBDetId id((ieta+1)*thesign, iphi+1);
						float y = -1 - ieta;
						if(sign == 1) y = ieta;
						barrel_m[0]->Fill(iphi, y, object()[id.rawId()].getStatusCode());
						//barrel_r[0]->Fill(iphi, y, object()[id.rawId()].rms_x12);
						//barrel_m[1]->Fill(iphi, y, object()[id.rawId()].mean_x6);
						//barrel_r[1]->Fill(iphi, y, object()[id.rawId()].rms_x6);
						//barrel_m[2]->Fill(iphi, y, object()[id.rawId()].mean_x1);
						//barrel_r[2]->Fill(iphi, y, object()[id.rawId()].rms_x1);
					}  // iphi
				}   // ieta

				for (int ix=0; ix<kEndcWedgesX; ix++) {
					for (int iy=0; iy<kEndcWedgesY; iy++) {
						if (! EEDetId::validDetId(ix+1,iy+1,thesign)) continue;
						EEDetId id(ix+1,iy+1,thesign);
						if (thesign==1) {
							endc_p_m[0]->Fill(ix+1,iy+1,object()[id.rawId()].getStatusCode());
							//endc_p_r[0]->Fill(ix+1,iy+1,object()[id.rawId()].rms_x12);
							//endc_p_m[1]->Fill(ix+1,iy+1,object()[id.rawId()].mean_x6);
							//endc_p_r[1]->Fill(ix+1,iy+1,object()[id.rawId()].rms_x6);
							//endc_p_m[2]->Fill(ix+1,iy+1,object()[id.rawId()].mean_x1);
							//endc_p_r[2]->Fill(ix+1,iy+1,object()[id.rawId()].rms_x1);
						}
						else{ 
							endc_m_m[0]->Fill(ix+1,iy+1,object()[id.rawId()].getStatusCode());
							//endc_m_r[0]->Fill(ix+1,iy+1,object()[id.rawId()].rms_x12);
							//endc_m_m[1]->Fill(ix+1,iy+1,object()[id.rawId()].mean_x6);
							//endc_m_r[1]->Fill(ix+1,iy+1,object()[id.rawId()].rms_x6);
							//endc_m_m[2]->Fill(ix+1,iy+1,object()[id.rawId()].mean_x1);
							//endc_m_r[2]->Fill(ix+1,iy+1,object()[id.rawId()].rms_x1);
						}
					}  // iy
				}   // ix
			}    // side

			//canvas.cd(1);
			//float bmin[3] ={0.7, 0.5, 0.4};
			//float bmax[3] ={1.7, 1.0, 0.8};
			//float emin[3] ={1.5, 0.8, 0.4};
			//float emax[3] ={2.5, 1.5, 0.8};
			TLine* l = new TLine(0., 0., 0., 0.);
			l->SetLineWidth(1);
			int ixSectorsEE[202] = {
				62, 62, 61, 61, 60, 60, 59, 59, 58, 58, 56, 56, 46, 46, 44, 44, 43, 43, 42, 42, 
				41, 41, 40, 40, 41, 41, 42, 42, 43, 43, 44, 44, 46, 46, 56, 56, 58, 58, 59, 59, 
				60, 60, 61, 61, 62, 62,  0,101,101, 98, 98, 96, 96, 93, 93, 88, 88, 86, 86, 81, 
				81, 76, 76, 66, 66, 61, 61, 41, 41, 36, 36, 26, 26, 21, 21, 16, 16, 14, 14,  9,
				9,  6,  6,  4,  4,  1,  1,  4,  4,  6,  6,  9,  9, 14, 14, 16, 16, 21, 21, 26, 
				26, 36, 36, 41, 41, 61, 61, 66, 66, 76, 76, 81, 81, 86, 86, 88, 88, 93, 93, 96, 
				96, 98, 98,101,101,  0, 62, 66, 66, 71, 71, 81, 81, 91, 91, 93,  0, 62, 66, 66, 
				91, 91, 98,  0, 58, 61, 61, 66, 66, 71, 71, 76, 76, 81, 81,  0, 51, 51,  0, 44, 
				41, 41, 36, 36, 31, 31, 26, 26, 21, 21,  0, 40, 36, 36, 11, 11,  4,  0, 40, 36, 
				36, 31, 31, 21, 21, 11, 11,  9,  0, 46, 46, 41, 41, 36, 36,  0, 56, 56, 61, 61, 66, 66};

				int iySectorsEE[202] = {
					51, 56, 56, 58, 58, 59, 59, 60, 60, 61, 61, 62, 62, 61, 61, 60, 60, 59, 59, 58, 
					58, 56, 56, 46, 46, 44, 44, 43, 43, 42, 42, 41, 41, 40, 40, 41, 41, 42, 42, 43, 
					43, 44, 44, 46, 46, 51,  0, 51, 61, 61, 66, 66, 76, 76, 81, 81, 86, 86, 88, 88, 
					93, 93, 96, 96, 98, 98,101,101, 98, 98, 96, 96, 93, 93, 88, 88, 86, 86, 81, 81, 
					76, 76, 66, 66, 61, 61, 41, 41, 36, 36, 26, 26, 21, 21, 16, 16, 14, 14,  9,  9, 
					6,  6,  4,  4,  1,  1,  4,  4,  6,  6,  9,  9, 14, 14, 16, 16, 21, 21, 26, 26, 
					36, 36, 41, 41, 51,  0, 46, 46, 41, 41, 36, 36, 31, 31, 26, 26,  0, 51, 51, 56, 
					56, 61, 61,  0, 61, 61, 66, 66, 71, 71, 76, 76, 86, 86, 88,  0, 62,101,  0, 61, 
					61, 66, 66, 71, 71, 76, 76, 86, 86, 88,  0, 51, 51, 56, 56, 61, 61,  0, 46, 46, 
					41, 41, 36, 36, 31, 31, 26, 26,  0, 40, 31, 31, 16, 16,  6,  0, 40, 31, 31, 16, 16,  6};

					for (int gId = 0; gId < TOTAL_IMAGES; gId++) {//was 3
						pad[gId][0]->cd();
						endc_m_m[gId]->SetStats(0);
						//endc_m_m[gId]->SetMaximum(225);
						//endc_m_m[gId]->SetMinimum(175);
						endc_m_m[gId]->Draw("colz");
						for ( int i=0; i<201; i=i+1) {
							if ( (ixSectorsEE[i]!=0 || iySectorsEE[i]!=0) && 
								(ixSectorsEE[i+1]!=0 || iySectorsEE[i+1]!=0) ) {
									l->DrawLine(ixSectorsEE[i], iySectorsEE[i], 
										ixSectorsEE[i+1], iySectorsEE[i+1]);
									l->SetLineWidth(0.2);
							}
						}

						//pad[gId + 3][0]->cd();
						//endc_m_r[gId]->SetStats(0);
						//endc_m_r[gId]->SetMaximum(emax[gId]);
						//endc_m_r[gId]->SetMinimum(emin[gId]);
						//endc_m_r[gId]->Draw("colz");
						//for ( int i=0; i<201; i=i+1) {
						//	if ( (ixSectorsEE[i]!=0 || iySectorsEE[i]!=0) && 
						//		(ixSectorsEE[i+1]!=0 || iySectorsEE[i+1]!=0) ) {
						//			l->DrawLine(ixSectorsEE[i], iySectorsEE[i], 
						//				ixSectorsEE[i+1], iySectorsEE[i+1]);
						//	}
						//}

						//canvas.cd(2);
						pad[gId][1]->cd();
						barrel_m[gId]->SetStats(0);
						//barrel_m[gId]->SetMaximum(225);
						//barrel_m[gId]->SetMinimum(175);
						barrel_m[gId]->Draw("colz");
						for(int i = 0; i <17; i++) {
							Double_t x = 20.+ (i *20);
							l = new TLine(x,-85.,x,86.);
							l->Draw();
						}
						l = new TLine(0.,0.,360.,0.);
						l->Draw();

						//pad[gId + 3][1]->cd();
						//barrel_r[gId]->SetStats(0);
						//barrel_r[gId]->SetMaximum(bmax[gId]);
						//barrel_r[gId]->SetMinimum(bmin[gId]);
						//barrel_r[gId]->Draw("colz");
						//for(int i = 0; i <17; i++) {
						//	Double_t x = 20.+ (i *20);
						//	l = new TLine(x,-85.,x,86.);
						//	l->Draw();
						//}
						//l = new TLine(0.,0.,360.,0.);
						//l->Draw();

						//canvas.cd(3);
						pad[gId][2]->cd();
						endc_p_m[gId]->SetStats(0);
						//endc_p_m[gId]->SetMaximum(225);
						//endc_p_m[gId]->SetMinimum(175);
						endc_p_m[gId]->Draw("colz");
						for ( int i=0; i<201; i=i+1) {
							if ( (ixSectorsEE[i]!=0 || iySectorsEE[i]!=0) && 
								(ixSectorsEE[i+1]!=0 || iySectorsEE[i+1]!=0) ) {
									l->DrawLine(ixSectorsEE[i], iySectorsEE[i], 
										ixSectorsEE[i+1], iySectorsEE[i+1]);
							}
						}

					//	pad[gId + 3][2]->cd();
					//	endc_p_r[gId]->SetStats(0);
					//	endc_p_r[gId]->SetMaximum(emax[gId]);
					//	endc_p_r[gId]->SetMinimum(emin[gId]);
					//	endc_p_r[gId]->Draw("colz");
					//	for ( int i=0; i<201; i=i+1) {
					//		if ( (ixSectorsEE[i]!=0 || iySectorsEE[i]!=0) && 
					//			(ixSectorsEE[i+1]!=0 || iySectorsEE[i+1]!=0) ) {
					//				l->DrawLine(ixSectorsEE[i], iySectorsEE[i], 
					//					ixSectorsEE[i+1], iySectorsEE[i+1]);
					//		}
					//	}
					}

					canvas.SaveAs(filename.c_str());
					return filename;
	}  // plot
}

namespace condPython {
  template<>
  void defineWhat<EcalTPGCrystalStatus>() {
    enum_<cond::ecalcond::How>("How")
      .value("singleChannel",cond::ecalcond::singleChannel)
      .value("bySuperModule",cond::ecalcond::bySuperModule) 
      .value("all",cond::ecalcond::all)
      ;

    typedef cond::ExtractWhat<EcalTPGCrystalStatus> What;
    class_<What>("What",init<>())
      .def("set_how",&What::set_how)
      .def("set_which",&What::set_which)
      .def("how",&What::how, return_value_policy<copy_const_reference>())
      .def("which",&What::which, return_value_policy<copy_const_reference>())
      ;
  }
}

PYTHON_WRAPPER(EcalTPGCrystalStatus,EcalTPGCrystalStatus);
