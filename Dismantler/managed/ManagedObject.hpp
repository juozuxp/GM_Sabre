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
	ManagedObject(const ManagedObject&) = delete;

	ManagedObject& operator=(ManagedObject&&) = delete;
	ManagedObject& operator=(const ManagedObject&) = delete;

private:
	ManagedObject* m_Handle;
};