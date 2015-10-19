#pragma once
#include "library/sp.h"
#include "game/tileview/voxel.h"

namespace OpenApoc
{

class IFile;

class LOFTemps
{
  private:
	std::vector<sp<VoxelSlice>> slices;

  public:
	LOFTemps(IFile &datFile, IFile &tabFile);
	sp<VoxelSlice> getSlice(unsigned int idx);
};
}; // namespace OpenApoc
