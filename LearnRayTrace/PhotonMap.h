#pragma once
#include "Singleton.h"
#include "Object.h"
#include "nanoflann.hpp"

struct Photon
{
	Vec3 pos;
	Vec3 dir;
	Vec3 color;
};

template <typename T>
class PhotonMapTemplate : public Singleton<PhotonMapTemplate<T>>, public Object
{
 public:
	 __class__(PhotonMapTemplate);
	inline size_t kdtree_get_point_count() const { return photons_.size(); }

	inline T kdtree_get_pt(const size_t idx, const size_t dim) const
	{
		if (dim == 0)
			return photons_[idx].pos.x();
		else if (dim == 1)
			return photons_[idx].pos.y();
		else
			return photons_[idx].pos.z();
	}

	template <class BBOX>
	bool kdtree_get_bbox(BBOX& /* bb */) const
	{
		return false;
	}

	void addPhoton(const Photon& photon) 
	{ 
		std::lock_guard<std::mutex> lg(photons_mutex_);
		photons_.push_back(photon); 
	}

	void build_kd_tree();

	template <int num_results>
	vector<Photon> find_nearst(Vec3 pt, float& max_distance)
	{
		vector<Photon> results;
		size_t						   ret_index[num_results];
		T						   out_dist_sqr[num_results];
		nanoflann::KNNResultSet<T> resultSet(num_results);
		resultSet.init(ret_index, out_dist_sqr);

		index_->findNeighbors(resultSet, pt._v, nanoflann::SearchParams(10));

		for (size_t i = 0; i < resultSet.size(); ++i)
		{
			int index = ret_index[i];
			max_distance = out_dist_sqr[i];
			results.push_back(photons_[index]);
		}

		return results;
	}

	PhotonMapTemplate() {}
	~PhotonMapTemplate() {}

 private:
	
	// construct a kd-tree index:
	typedef nanoflann::KDTreeSingleIndexDynamicAdaptor<
		nanoflann::L2_Simple_Adaptor<T, PhotonMapTemplate<T>>,
		PhotonMapTemplate<T>,
		3 /* dim */
		>
		my_kd_tree_t;

	std::mutex photons_mutex_;
	std::shared_ptr<my_kd_tree_t>   index_;
	vector<Photon> photons_;
};

using PhotonMap = PhotonMapTemplate<float>;

template <typename T>
void PhotonMapTemplate<T>::build_kd_tree()
{
	index_ = std::make_shared<my_kd_tree_t>(3 /*dim*/, *this, nanoflann::KDTreeSingleIndexAdaptorParams(10 /* max leaf */));
	cout << "PHOTON SIZE " << photons_.size() << endl;
}