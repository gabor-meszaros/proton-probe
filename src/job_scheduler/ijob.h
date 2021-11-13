#pragma once

namespace ProtonProbe
{
	class IJob
	{
	public:
		typedef int IdType;

		virtual ~IJob() {}
		virtual void execute() = 0;
	};
}