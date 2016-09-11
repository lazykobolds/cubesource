#pragma once

class BaseComponent
{
private:

protected:
	BaseComponent() { _active = false; }

public:
	inline void Activate() { _active = true; }
	inline void Deactivate() { _active = false; }

	unsigned int _entityId;
	bool _active;
};
