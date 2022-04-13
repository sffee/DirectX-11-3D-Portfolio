#pragma once

class GJHGameUpdaterInterface
{
public:
	virtual void SceneChangeStart() { }
	virtual void SceneChangeEnd() { }

	virtual void Begin() { }
	virtual void Start() { }

	virtual void UpdatePrev() { }
	virtual void Update() { }
	virtual void UpdateNext() { }
	virtual void CollisionPrev() { }
	virtual void Collision() { }
	virtual void CollisionNext() { }
	virtual void RenderPrev() { }
	virtual void Render() { }
	virtual void DebugRender() { }
	virtual void RenderNext() { }
	virtual void Release() { }

	virtual void OnGUI() { }

protected:
	GJHGameUpdaterInterface() { }
	~GJHGameUpdaterInterface() { }
};
