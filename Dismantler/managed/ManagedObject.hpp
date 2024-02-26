#pragma once

class ManagedObject
{
public:
	ManagedObject()
	{
		m_Handle = this;
	}

	virtual ~ManagedObject() = default;

	ManagedObject(ManagedObject&&) = default;
	ManagedObject(const ManagedObject&) = delete;

	ManagedObject& operator=(ManagedObject&&) = default;
	ManagedObject& operator=(const ManagedObject&) = delete;

private:
	ManagedObject* m_Handle;
};