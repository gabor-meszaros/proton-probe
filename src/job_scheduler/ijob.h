#pragma once

namespace ProtonProbe
{
	class IJob
	{
	public:
		virtual ~IJob() {}
		virtual void execute() = 0;
	};
}