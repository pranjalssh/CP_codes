template <class vecType>
struct xorSpace	{
	int dimension, rank;
	vector<vecType> basis, basisVectors;
 
	xorSpace(int dim = 0)	{
		dimension = dim;
		rank = 0;
		basis.resize(dim);
		basisVectors.resize(0);
	}
 
	//need to define this
	inline int getValAtDim(vecType v, int dim)	{
		return (((1LL << dim) & v) != 0);
	}
 
	bool addToBasis(vecType num)	{
		int pos = -1;
		for(int i = dimension - 1; i >= 0; i--)	{
			if(getValAtDim(num, i) != 0)	{
				if(basis[i] == 0)	{
					pos = i;
					break;
				}	else	{
					num ^= basis[i];
				}
			}
		}
 
		if(pos != -1)	{
			basis[pos] = num;
			basisVectors.push_back(num);
			rank++;
			return true;
		}
		return false;
	}
};