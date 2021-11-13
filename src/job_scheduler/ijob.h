#pragma once

namespace ProtonProbe
{
	class IJob
	{
	public:
		typedef int IdType;
		const static IJob::IdType INVALID_JOB_ID{ -1 };

		virtual ~IJob() {}
		virtual bool execute(const bool& stopExecution) = 0;
	};
}