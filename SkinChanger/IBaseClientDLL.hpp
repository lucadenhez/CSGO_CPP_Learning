#pragma once

#include "Virtuals.hpp"
#define CLIENT_DLL_INTERFACE_VERSION "VClient018"


enum ClientFrameStage_t : int {
	FRAME_UNDEFINED = -1,
	FRAME_START,
	FRAME_NET_UPDATE_START,
	FRAME_NET_UPDATE_POSTDATAUPDATE_START,
	FRAME_NET_UPDATE_POSTDATAUPDATE_END,
	FRAME_NET_UPDATE_END,
	FRAME_RENDER_START,
	FRAME_RENDER_END
};

class ClientClass;

class IBaseClientDLL {
public:
	inline ClientClass* GetAllClasses() {
		return GetVirtualFunction<ClientClass* (__thiscall*)(IBaseClientDLL*)>(this, 8)(this);
	}
};

// I spent 30 minutes trying to figure out why IBaseClientDLL was unresolved. It's because it wasn't capitalized. >_<
extern IBaseClientDLL* clientDLL;