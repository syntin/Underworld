#include "scriptSystem.h"
#include "componentManager.h"

class BaseScript
{
public:
	virtual void Update() {}
	virtual ~BaseScript() {}
};

void ScriptSystem::Update(ComponentManager& components)
{
	auto& entities = components.GetScriptEntities();

	for (auto& e : entities)
	{
		Script* s = components.GetScript(e);
		if (!s || !s->instance)
			continue;

		BaseScript* script = reinterpret_cast<BaseScript*>(s->instance);
		script->Update();

		s->dirty = false;
	}
}