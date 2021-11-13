#include "job_handle.h"

namespace ProtonProbe
{
	JobHandle::JobHandle(IJob::IdType id, IJob& job)
		: id{ id }
		, job{ job }
	{
	}
}