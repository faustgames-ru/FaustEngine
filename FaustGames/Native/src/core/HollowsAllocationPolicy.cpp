#include "HollowsAllocationPolicy.h"

namespace core
{
	int StandartAllocationPolicy::AllocatedSize(0);
	int HollowsAllocationPolicy::AllocatedSize(0);
	SmallBlocksContainer HollowsAllocationPolicy::_smallBlocks;
	LargeBlocksContainer HollowsAllocationPolicy::_largeBlocks;

	int HollowsAllocationBlock::AllocationBlocksSize(0);
}
