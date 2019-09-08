#pragma once
#include "Singleton.h"
#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/memory.hpp>

class Setting : public Singleton<Setting>
{
 private:
	friend class cereal::access;
	template <class Archive>
	void serialize(Archive& ar)
	{
		int type = traceType;
		ar(cereal::make_nvp("#0 path trace 1 direct", type));
		traceType = (TraceType)type;

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

	int		  sample = 1;
	TraceType traceType;
};
