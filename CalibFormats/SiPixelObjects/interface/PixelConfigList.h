#ifndef PixelConfigList_h
#define PixelConfigList_h

class PixelConfigList {

 public:

  void writefile(){

    std::string directory=getenv("PIXELCONFIGURATIONBASE");
    directory+="/pixel/PixelConfigDataExamples/";
    
    std::string filename=directory+"configurations.txt";

    std::ofstream out(filename.c_str());

    for (unsigned int i=0;i<configs.size();i++){
      out << "key "<<i<<std::endl;
      configs[i].write(out);
      out <<std::endl;
    }


  }


  void readfile(std::string filename){

    std::ifstream in(filename.c_str());

    std::string tag;
	    
    in >> tag;
    while(tag.substr(0,1) == "#") {
      in.ignore(4096, '\n'); //skips to endl;
      in >> tag;
    }

    unsigned int version=0;

    do{
      if (tag!="key"){
	std::cout << "PixelConfigDB: tag="<<tag<<std::endl;
	assert(0);
      }
      unsigned int tmp_version;
      in >> tmp_version;
      if (version!=tmp_version){
	std::cout << "PixelConfigDB: read version:"<<tmp_version<<" while expected "
		  << version << std::endl;
	assert(0);
      }

      in >> tag;
      while(tag.substr(0,1) == "#") {
	in.ignore(4096, '\n'); //skips to endl;
	in >> tag;
      }

      PixelConfig aConfig;
      while (tag!="key"&&in.good()){
	unsigned int tmp;
	in >>tmp;
	//std::cout << "adding: "<<tag<<" "<<tmp<<std::endl;
	aConfig.add(tag,tmp);
	in >> tag;
	while(tag.substr(0,1) == "#") {
	  in.ignore(4096, '\n'); //skips to endl;
	  in >> tag;
	}
      }
      
      configs.push_back(aConfig);
      version++;
      
    }while (in.good());
		
    in.close();

  }

  //Method will return new key
  unsigned int clone(unsigned int oldkey, std::string path, unsigned int version){
    PixelConfig aConfig=configs[oldkey];
    
    unsigned int oldversion;
    
    if (-1==aConfig.update(path,oldversion,version)){
      std::cout << "Old version not found for path="<<path<<" in config "<<oldkey<<std::endl;
      assert(0);
    }

    configs.push_back(aConfig);
    
    return configs.size()-1;

  }


  //Method will return new key
  unsigned int add(PixelConfig& aConfig){

    configs.push_back(aConfig);
    
    return configs.size()-1;

  }

  unsigned int size(){ return configs.size(); }

  PixelConfig& operator[](unsigned int i) {return configs[i];}

 private:

  std::vector<PixelConfig> configs;

};

#endif
