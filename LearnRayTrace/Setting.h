#pragma once
#include "Singleton.h"
#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/memory.hpp>

#define PHOTON_MAP_NEARST_SIZE 300
//#define  MAX_THREADS   std::thread::hardware_concurrency()
#define  MAX_THREADS  1

class Setting : public Singleton<Setting>
{
 private:
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar)
	{
		int type = traceType;
		ar(cereal::make_nvp("#0 path trace 1 direct", type));
		traceType = ( TraceType )type;

		ar(CEREAL_NVP(sample));
	}

 public:
	enum TraceType
	{
		ePathTrace,
		eDirect,
		ePhotonMap,
	};

	Setting();
	~Setting();

	int		  sample;
	int		  max_depth;
	TraceType traceType;
};
