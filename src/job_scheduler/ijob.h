#pragma once

class IJob
{
public:
	virtual ~IJob() {}
	virtual void execute() = 0;
};