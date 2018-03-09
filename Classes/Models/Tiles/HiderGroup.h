#pragma once
#include "HiderSegmentObject.h"

class HiderGroup
{
public:
	HiderGroup();
	~HiderGroup();

	std::string getChainName() const { return chainName; }
	HiderSegmentObject* getHead() const { return head; }
	void setHead(HiderSegmentObject* hd) { head = hd; segmentsCount++; }

	void addSegment(HiderSegmentObject* seg) { segments->push_back(seg); segmentsCount++; }
	HiderSegmentObject* getSegmentAt(char index) const;
	char getSegmentsCount() const { return segmentsCount; }
	void initSegmentTextures();
	void moveHiderGroup(CellsList* cells, Cell* startCell);
	bool checkHiderGroup();
	CellsList* getCoveredSegmentCells() const { return coveredCells; }

private:
	void moveSegmentToCell(HiderSegmentObject* segment, CellsList* cell);

	HiderSegmentObject* head = nullptr;
	std::vector<HiderSegmentObject*>* segments = nullptr;
	std::string chainName = "";
	char segmentsCount = 0;

	CellsList* coveredCells = nullptr;
};

