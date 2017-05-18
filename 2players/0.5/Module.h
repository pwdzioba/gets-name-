#ifndef __MODULE_H__
#define __MODULE_H__

struct Collider;

class Module
{
private:
	bool enabled = true;

public:
	virtual ~Module() {}

	virtual bool Init()					{ return true; }
	virtual bool Start()					{ return true; }
	virtual update_status PreUpdate()		{ return update_status::UPDATE_CONTINUE; }
	virtual update_status Update()		{ return update_status::UPDATE_CONTINUE; }
	virtual update_status PostUpdate() 	{ return update_status::UPDATE_CONTINUE; }
	virtual bool CleanUp()				{ return true; }
		
	bool IsEnabled() const				{ return enabled; }

	void Enable()
	{
		if(enabled == false)
		{
			enabled = true;
			Start();
		}
	}

	void Disable()
	{
		if (enabled == true)
		{
		CleanUp();
		
		enabled = false;
		}

	}
	virtual void OnCollision(Collider*, Collider*) {}
	virtual void OnCollisionWall(Collider*, Collider*) {}
	virtual void OnCollisionItem(Collider*, Collider*) {}
	virtual void OnCollisionWater(Collider*, Collider*) {}
	virtual void OnCollisionEnemy(Collider*, Collider*) {}
};

#endif // __MODULE_H__