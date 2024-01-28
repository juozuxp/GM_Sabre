#pragma once

class ManagedObject
{
public:
	ManagedObject()
	{
		m_Handle = this;
	}

	virtual ~ManagedObject() = default;

	ManagedObject(ManagedObject&&) = delete;
	ManagedObject& operator=(ManagedObject&&) = delete;

private:
	ManagedObject* m_Handle;
};