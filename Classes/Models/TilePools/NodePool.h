#pragma once
#include "base/CCVector.h"
#include "General/Constants.h"

template <typename T>
class NodePool
{
public:
	NodePool();
	~NodePool();

	T* getNode();
	void recycleNode(T* obj);

	int size() const
	{
		return repo.size();
	}

private:
	cocos2d::Vector<T*> repo;
};

template <typename T>
NodePool<T>::NodePool()
	:repo(MaxTileCount)
{
}

template <typename T>
NodePool<T>::~NodePool()
{
}

template <typename T>
T* NodePool<T>::getNode()
{
	if (repo.size() > 0)
	{
		const auto tile = repo.back();
		repo.popBack();
		return tile;
	}
	else
	{
		return nullptr;
	}

}

template <typename T>
void NodePool<T>::recycleNode(T* obj)
{
	obj->recycle();
	repo.pushBack(obj);
}
