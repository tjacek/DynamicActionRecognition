#pragma once

class CDepthMap
{
public:
	CDepthMap(void);
	~CDepthMap(void);

	int GetNRows() const {return m_nrows;}
	int GetNCols() const {return m_ncols;}

	void SetSize(int ncols, int nrows); //allocate space

	void SetItem(int r, int c, float val);
	float GetItem(int r, int c) const;

	void CopyDepthMapTo(CDepthMap & dstMap) const;

	void Scale(float s); //scale the depth
	void ScaleSizeNN(float scaleFactor); //ncols = floor(scaleFactor * ncols), nrows = floor(scaleFactor * nrows)

	int NumberOfNonZeroPoints() const;
	float AvgNonZeroDepth() const;


protected:
	int m_ncols; //ncols
	int m_nrows; //nrows
	float * m_depthVals; //nrows * ncols
};

typedef std::vector<CDepthMap*> Action;
typedef std::vector<CDepthMap*> Instant;

#define numberOfInstants 2
extern vector<Instant> getInstants(Action action);
extern void freeAction(Action action);