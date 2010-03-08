#ifndef RPCObFebAssmap_h
#define RPCObFebAssmap_h
#include <vector>
#include <string>


class RPCObFebAssmap {
    public:
      struct FebAssmap_Item {
        int dpid;
        int region;
        int ring;
        int station;
        int sector;
        int layer;
        int subsector;
        int chid;
    };
    RPCObFebAssmap(){}
    virtual ~RPCObFebAssmap(){}
    std::vector<FebAssmap_Item> ObFebAssmap_rpc;
   };

#endif

